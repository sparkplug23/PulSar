#!/usr/bin/env python3
"""PulSar Preset Designer — local library, JSON editor, playlist builder and LAN devices."""
from __future__ import annotations
import copy
import datetime as dt
import json
import os
import pathlib
import queue
import re
import threading
import hashlib
import sys
import subprocess
import webbrowser
import tkinter as tk
from tkinter import filedialog, messagebox, simpledialog, ttk
from urllib.parse import urlparse
from urllib.request import Request, urlopen
from uuid import uuid4
from effect_catalog import fetch_device_effects, find_effects_cpp, read_source_effects, preset_compatible, normalize_effect
from palette_catalog import find_palette_sources, read_source_palettes, fetch_device_palettes, palette_names_from_library

BASE = pathlib.Path(__file__).resolve().parent
LIBRARY = BASE / 'library'
EXPORTS = BASE / 'exports'
CATEGORIES = ('1d', '2d', 'audio_reactive')
SERVICE = '_pulsar._tcp.local.'
MAX_PRESET_ID = 250  # compatible with current firmware's 1..250 preset-name scanner


def write_json(path, value):
    path = pathlib.Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + '.tmp')
    temporary.write_text(json.dumps(value, indent=2, ensure_ascii=False) + '\n', encoding='utf-8')
    os.replace(temporary, path)


def read_json(path):
    return json.loads(pathlib.Path(path).read_text(encoding='utf-8-sig'))


def item_id():
    return uuid4().hex[:12]


def file_key(text):
    return re.sub(r'[^a-zA-Z0-9_-]+', '_', text).strip('_')[:48] or 'entry'


def unique_key(store, desired):
    key = file_key(desired)
    while key in store:
        key = f'{file_key(desired)}_{item_id()[:4]}'
    return key


def entry_type(obj):
    return 'playlist' if isinstance(obj.get('playlist'), dict) else 'preset'


def new_entry(name, kind):
    if kind == 'playlist':
        return {'n': name, 'playlist': {'psn': [], 'dur': [], 'repeat': 0, 'end': 0, 'r': 1}, 'on': True}
    return {'n': name, 'Pal': 'Snowy 02', 'Effects': {'FX': 'Static Palette', 'SX': 127, 'IX': 127, 'GP': 1, 'DC': 0, 'RA': 20}, 'BrightnessRGB': 100}


class Catalog:
    def __init__(self):
        self.entries = {}
        self.id_store = LIBRARY / '_imported_ids.json'
        try:
            self.original_ids = read_json(self.id_store)
        except (FileNotFoundError, ValueError, json.JSONDecodeError):
            self.original_ids = {}
        self.refresh()

    def refresh(self):
        self.entries = {}
        for kind in ('preset', 'playlist'):
            for cat in CATEGORIES:
                folder = LIBRARY / ('presets' if kind == 'preset' else 'playlists') / cat
                folder.mkdir(parents=True, exist_ok=True)
                for path in sorted(folder.glob('*.json')):
                    try:
                        doc = read_json(path)
                        if not isinstance(doc, dict) or not isinstance(doc.get('n'), str):
                            continue
                        key = f'{kind}/{cat}/{path.stem}'
                        self.entries[key] = {'kind': kind, 'category': cat, 'data': doc, 'path': path, 'source_id': self.original_ids.get(key)}
                    except Exception as exc:
                        print(f'Skipping invalid library file {path}: {exc}')

    def save(self, key):
        write_json(self.entries[key]['path'], self.entries[key]['data'])

    def create(self, kind, category, name, data=None):
        if category not in CATEGORIES:
            raise ValueError(f'Unknown category {category}')
        if not name.strip():
            raise ValueError('Name must not be empty')
        folder = LIBRARY / ('presets' if kind == 'preset' else 'playlists') / category
        folder.mkdir(parents=True, exist_ok=True)
        stem = file_key(name)
        path = folder / f'{stem}.json'
        while path.exists():
            path = folder / f'{stem}_{item_id()[:5]}.json'
        doc = copy.deepcopy(data if data is not None else new_entry(name, kind))
        doc['n'] = name
        key = f'{kind}/{category}/{path.stem}'
        self.entries[key] = {'kind': kind, 'category': category, 'data': doc, 'path': path, 'source_id': None}
        self.save(key)
        return key

    def save_original_ids(self):
        write_json(self.id_store, self.original_ids)

    def delete(self, key):
        self.entries[key]['path'].unlink()
        del self.entries[key]
        self.original_ids.pop(key, None)
        self.save_original_ids()

    def names(self, kind, category):
        return [(key, rec['data'].get('n', key)) for key, rec in self.entries.items() if rec['kind'] == kind and rec['category'] == category]

    def import_file(self, path, category, replace=False):
        obj = read_json(path)
        if not isinstance(obj, dict):
            raise ValueError('Expected presets.json with an object keyed by numeric preset IDs')
        imported, skipped = 0, 0
        id_to_name = {str(k): str(v.get('n')) for k, v in obj.items() if isinstance(v, dict) and v.get('n') and 'playlist' not in v}
        for old_id, data in sorted(obj.items(), key=lambda pair: int(pair[0]) if pair[0].isdigit() else -1):
            if not isinstance(data, dict) or not data.get('n'):
                skipped += 1
                continue
            data = copy.deepcopy(data)
            kind = entry_type(data)
            if kind == 'playlist':
                pl = data['playlist']
                if not pl.get('psn') and isinstance(pl.get('ps'), list):
                    # Convert numeric-only historical playlists to editable names.
                    pl['psn'] = [id_to_name.get(str(v), f'UNRESOLVED_PRESET_{v}') for v in pl['ps']]
            name = str(data['n'])
            matches = [(key, rec) for key, rec in self.entries.items() if rec['category'] == category and rec['kind'] == kind and rec['data'].get('n') == name]
            if matches and replace:
                key, rec = matches[0]
                rec['data'] = copy.deepcopy(data)
                rec['source_id'] = int(old_id) if old_id.isdigit() else None
                self.save(key)
            else:
                key = self.create(kind, category, name, data)
                self.entries[key]['source_id'] = int(old_id) if old_id.isdigit() else None
            self.original_ids[key] = int(old_id) if old_id.isdigit() else None
            imported += 1
        self.save_original_ids()
        return imported, skipped


def compile_export(catalog, category, preset_keys, playlist_keys):
    """Pure compile: names are truth; ps arrays are generated. Fail on ambiguity."""
    selected = list(preset_keys) + list(playlist_keys)
    if not selected:
        raise ValueError('Select at least one preset or playlist for export')
    refs = [catalog.entries[k] for k in selected]
    names = {}
    for k, rec in zip(selected, refs):
        name = rec['data'].get('n', '')
        if not name:
            raise ValueError(f'Missing preset name: {k}')
        if len(name.encode('utf-8')) > 32:
            raise ValueError(f'Name exceeds firmware 32-byte name limit: {name!r}')
        folded = name.casefold()
        if folded in names:
            raise ValueError(f'Duplicate name in export: {name!r} ({k} / {names[folded]})')
        names[folded] = k
    if len(selected) > MAX_PRESET_ID:
        raise ValueError(f'At most {MAX_PRESET_ID} entries supported by current firmware')
    # Preserve IDs when they were imported from a device, if possible. Fill other IDs sequentially.
    keys_to_ids = {}
    used = set()
    for k in selected:
        old = catalog.entries[k].get('source_id')
        if isinstance(old, int) and 1 <= old <= MAX_PRESET_ID and old not in used:
            keys_to_ids[k] = old
            used.add(old)
    next_id = 1
    for k in selected:
        if k not in keys_to_ids:
            while next_id in used:
                next_id += 1
            keys_to_ids[k] = next_id
            used.add(next_id)
    out = {'0': {}}
    for k in selected:
        rec = catalog.entries[k]
        val = copy.deepcopy(rec['data'])
        if rec['kind'] == 'playlist':
            obj = val['playlist']
            names_requested = obj.get('psn', [])
            if not isinstance(names_requested, list) or not names_requested:
                raise ValueError(f'Playlist {val["n"]!r} has no psn entries')
            if len(names_requested) > 100:
                raise ValueError(f'Playlist {val["n"]!r} exceeds the runtime 100-entry limit')
            resolved = []
            for target in names_requested:
                reference = names.get(str(target).casefold())
                if reference is None:
                    raise ValueError(f'Playlist {val["n"]!r}: unresolved preset {target!r}. Include that preset in export.')
                if catalog.entries[reference]['kind'] != 'preset':
                    raise ValueError(f'Playlist {val["n"]!r}: {target!r} refers to another playlist')
                resolved.append(keys_to_ids[reference])
            durations = obj.get('dur', [])
            if isinstance(durations, list) and len(durations) != len(resolved):
                raise ValueError(f'Playlist {val["n"]!r}: {len(durations)} durations for {len(resolved)} entries')
            obj['ps'] = resolved
        out[str(keys_to_ids[k])] = val
    out = dict(sorted(out.items(), key=lambda kv: int(kv[0])))
    return out, keys_to_ids


def http_get_presets_bytes(ip, port=80, timeout=8):
    url = f'http://{ip}:{port}/presets.json'
    with urlopen(Request(url, headers={'Cache-Control':'no-cache'}), timeout=timeout) as resp:
        payload = resp.read(5_000_001)
        if len(payload) > 5_000_000:
            raise ValueError('Device preset file exceeds 5 MB')
        return payload


def http_get_presets(ip, port=80, timeout=8):
    url = f'http://{ip}:{port}/presets.json'
    payload = http_get_presets_bytes(ip, port, timeout)
    value = json.loads(payload.decode('utf-8-sig'))
    if not isinstance(value, dict):
        raise ValueError('Device returned non-object JSON')
    return value


def http_upload_presets(ip, port, doc, timeout=25):
    # Matches PulSar /upload and source/index.js form field: data, filename /presets.json.
    from urllib.request import urlopen
    boundary = '----PulSarPresetDesigner' + item_id()
    payload = json.dumps(doc, separators=(',', ':'), ensure_ascii=False).encode('utf-8')
    parts = [f'--{boundary}\r\nContent-Disposition: form-data; name="data"; filename="/presets.json"\r\nContent-Type: application/json\r\n\r\n'.encode(), payload, f'\r\n--{boundary}--\r\n'.encode()]
    req = Request(f'http://{ip}:{port}/upload', data=b''.join(parts), headers={'Content-Type': f'multipart/form-data; boundary={boundary}'}, method='POST')
    with urlopen(req, timeout=timeout) as resp:
        message = resp.read(4000).decode('utf-8', 'replace')
        return resp.status, message


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('PulSar — Preset & Playlist Designer')
        self.geometry('1470x870')
        self.minsize(1120, 670)
        self.catalog = Catalog()
        self.category = tk.StringVar(value='1d')
        self.current_key = None
        self.selected_export = {}
        self.worker_events = queue.Queue()
        self.devices = {}
        self.device_by_row = {}
        self.device_effect_cache = {}  # host:port -> {ID: name}
        self.source_effects = []
        self.target_device = tk.StringVar(value='')
        self.only_supported = tk.BooleanVar(value=False)
        self.source_palettes = []
        self.device_palette_cache = {}
        self.dark_theme()
        self.effects_source_label = tk.StringVar(value='No source catalog loaded')
        self.make_ui()
        self.refresh_library()
        self.after(125, self.poll_workers)
        self.after(350, self.load_local_palettes_if_available)

    def status(self, message):
        self.status_var.set(message)

    def make_ui(self):
        root = ttk.Frame(self, padding=10)
        root.pack(fill='both', expand=True)
        title = ttk.Frame(root)
        title.pack(fill='x', pady=(0, 8))
        ttk.Label(title, text='PulSar Preset & Playlist Designer', font=('Segoe UI', 17, 'bold')).pack(side='left')
        profilebar = ttk.Frame(title)
        profilebar.pack(side='left', padx=(24, 8))
        for text, value in [('●  1D', '1d'), ('▦  2D', '2d'), ('♫  Audio reactive', 'audio_reactive')]:
            ttk.Radiobutton(profilebar, text=text, value=value, variable=self.category, command=self.refresh_library, style='Profile.TRadiobutton').pack(side='left', padx=3)
        ttk.Button(title, text='Build / export', command=lambda: self.tabs.select(self.export_tab)).pack(side='right', padx=4)
        ttk.Button(title, text='Open presets.json', command=self.import_local).pack(side='right', padx=4)
        networkbar = ttk.Frame(root)
        networkbar.pack(fill='x', pady=(0, 7))
        ttk.Label(networkbar, text='ESP32 IP / hostname:').pack(side='left', padx=(0, 5))
        self.quick_ip = tk.StringVar()
        ttk.Entry(networkbar, textvariable=self.quick_ip, width=26).pack(side='left', padx=(0, 4))
        ttk.Button(networkbar, text='Connect by IP', command=self.quick_connect).pack(side='left', padx=3)
        ttk.Button(networkbar, text='Discover devices', command=self.quick_discover).pack(side='left', padx=3)
        ttk.Button(networkbar, text='Fetch presets.json', command=self.quick_fetch).pack(side='left', padx=3)
        ttk.Button(networkbar, text='Open device WebUI', command=self.quick_webui).pack(side='left', padx=3)
        self.quick_target_label = tk.StringVar(value='No target device selected')
        ttk.Label(networkbar, textvariable=self.quick_target_label).pack(side='left', padx=14)
        self.tabs = ttk.Notebook(root)
        self.tabs.pack(fill='both', expand=True)
        self.make_library_tab()
        self.make_playlist_tab()
        self.make_export_tab()
        self.make_devices_tab()
        self.make_effects_tab()
        self.make_palette_tab()
        self.make_viewer_tab()
        self.make_webui_tab()
        self.status_var = tk.StringVar(value='Ready. Select a library entry to edit.')
        ttk.Label(root, textvariable=self.status_var, anchor='w').pack(fill='x', pady=(7, 0))

    def make_library_tab(self):
        tab = ttk.Frame(self.tabs, padding=6)
        self.tabs.add(tab, text='Preset library / JSON')
        paned = ttk.Panedwindow(tab, orient='horizontal')
        paned.pack(fill='both', expand=True)
        left = ttk.Frame(paned)
        right = ttk.Frame(paned)
        paned.add(left, weight=1)
        paned.add(right, weight=3)
        toolbar = ttk.Frame(left)
        toolbar.pack(fill='x', pady=3)
        ttk.Button(toolbar, text='New preset', command=lambda: self.create_item('preset')).pack(side='left', padx=2)
        ttk.Button(toolbar, text='Duplicate', command=self.duplicate_item).pack(side='left', padx=2)
        ttk.Button(toolbar, text='Delete', command=self.delete_item).pack(side='left', padx=2)
        filterbar = ttk.Frame(left)
        filterbar.pack(fill='x', pady=(3, 2))
        ttk.Checkbutton(filterbar, text='Show only effects on target device', variable=self.only_supported, command=self.refresh_compatibility_views).pack(side='left')
        self.search_var = tk.StringVar()
        search = ttk.Entry(left, textvariable=self.search_var)
        search.pack(fill='x', pady=5)
        search.insert(0, '')
        self.search_var.trace_add('write', lambda *_: self.refresh_library_list())
        self.library_tree = ttk.Treeview(left, columns=('kind', 'name'), show='headings', selectmode='browse')
        self.library_tree.heading('kind', text='Type')
        self.library_tree.heading('name', text='Name')
        self.library_tree.column('kind', width=80, stretch=False)
        self.library_tree.column('name', width=260)
        self.library_tree.pack(fill='both', expand=True)
        self.library_tree.bind('<<TreeviewSelect>>', self.selected_library)
        quick = ttk.Frame(right)
        quick.pack(fill='x', pady=(0, 5))
        ttk.Label(quick, text='Effect').pack(side='left', padx=(0, 3))
        self.fx_var = tk.StringVar()
        self.fx_choice = ttk.Combobox(quick, textvariable=self.fx_var, width=25)
        self.fx_choice.pack(side='left', padx=(0, 8))
        ttk.Label(quick, text='Palette').pack(side='left', padx=(0, 3))
        self.palette_var = tk.StringVar()
        self.palette_choice = ttk.Combobox(quick, textvariable=self.palette_var, width=23)
        self.palette_choice.pack(side='left', padx=(0, 8))
        ttk.Label(quick, text='Group').pack(side='left', padx=(0, 3))
        self.group_var = tk.StringVar(value='GP')
        self.group_choice = ttk.Combobox(quick, textvariable=self.group_var, values=['GP', 'GPP', 'GPB'], width=5, state='readonly')
        self.group_choice.pack(side='left')
        self.group_value = tk.StringVar(value='1')
        ttk.Entry(quick, textvariable=self.group_value, width=6).pack(side='left', padx=3)
        ttk.Button(quick, text='Apply', command=self.apply_quick).pack(side='left', padx=3)
        ttk.Button(quick, text='Test live on ESP32', command=self.test_current_live).pack(side='left', padx=7)
        ttk.Label(right, text='Structured parameters (double-click any scalar to edit)').pack(anchor='w', pady=(0, 4))
        self.parameter_tree = ttk.Treeview(right, columns=('value',), show='tree headings', selectmode='browse')
        self.parameter_tree.heading('#0', text='JSON property')
        self.parameter_tree.heading('value', text='Value')
        self.parameter_tree.column('#0', width=240)
        self.parameter_tree.column('value', width=300)
        self.parameter_tree.pack(fill='both', expand=True)
        self.parameter_tree.bind('<Double-1>', self.change_param)
        bar = ttk.Frame(right)
        bar.pack(fill='x', pady=(4, 3))
        ttk.Label(bar, text='JSON entry body (paste this directly after "1": in presets.json)').pack(side='left')
        ttk.Button(bar, text='Copy JSON', command=self.copy_json).pack(side='right', padx=3)
        ttk.Button(bar, text='Apply JSON / Save', command=self.apply_json).pack(side='right', padx=3)
        self.raw_text = tk.Text(right, height=14, font=('Consolas', 10), wrap='none', undo=True)
        self.raw_text.pack(fill='both', expand=True)
        ttk.Label(right, text='Edits in the structured tree refresh this JSON. After pasting, click Apply JSON / Save. Playlist JSON can be edited here too.').pack(anchor='w')

    def make_playlist_tab(self):
        tab = ttk.Frame(self.tabs, padding=6)
        self.tabs.add(tab, text='Playlist designer')
        outer = ttk.Panedwindow(tab, orient='horizontal')
        outer.pack(fill='both', expand=True)
        left, right = ttk.Frame(outer), ttk.Frame(outer)
        outer.add(left, weight=1)
        outer.add(right, weight=3)
        ttk.Button(left, text='New playlist', command=lambda: self.create_item('playlist')).pack(fill='x', pady=3)
        self.playlist_list = tk.Listbox(left, exportselection=False)
        self.playlist_list.pack(fill='both', expand=True)
        self.playlist_list.bind('<<ListboxSelect>>', self.selected_playlist)
        top = ttk.Frame(right)
        top.pack(fill='x', pady=5)
        ttk.Label(top, text='Preset name:').pack(side='left')
        self.preset_choice = ttk.Combobox(top, state='readonly', width=47)
        self.preset_choice.pack(side='left', padx=5)
        ttk.Button(top, text='Add to playlist', command=self.add_to_playlist).pack(side='left', padx=3)
        ttk.Button(top, text='Save playlist', command=self.save_playlist).pack(side='right')
        self.playlist_steps = ttk.Treeview(right, columns=('name', 'duration'), show='headings', height=15)
        self.playlist_steps.heading('name', text='Preset (psn, authoritative)')
        self.playlist_steps.heading('duration', text='Duration (100ms ticks)')
        self.playlist_steps.column('name', width=350)
        self.playlist_steps.column('duration', width=170)
        self.playlist_steps.pack(fill='both', expand=True)
        self.playlist_steps.bind('<Double-1>', self.edit_playlist_duration)
        tools = ttk.Frame(right)
        tools.pack(fill='x', pady=6)
        for label, handler in [('Move up', lambda: self.move_step(-1)), ('Move down', lambda: self.move_step(1)), ('Remove step', self.remove_step)]:
            ttk.Button(tools, text=label, command=handler).pack(side='left', padx=3)
        ttk.Label(right, text='The displayed list edits psn and dur. Numeric ps is regenerated during export. For advanced fields, edit the playlist JSON in the library tab.').pack(anchor='w')
        self.playlist_key = None

    def make_export_tab(self):
        tab = ttk.Frame(self.tabs, padding=12)
        self.tabs.add(tab, text='Build / export')
        self.export_tab = tab
        ttk.Label(tab, text='Select library entries for the device file', font=('Segoe UI', 13, 'bold')).pack(anchor='w')
        ttk.Label(tab, text='Presets and playlists are stored separately in the library. Export combines them, resolves psn names into ps IDs, and validates dependencies.').pack(anchor='w', pady=(2, 10))
        self.export_tree = ttk.Treeview(tab, columns=('selected', 'kind', 'name'), show='headings', selectmode='extended')
        for col, txt, width in [('selected', 'Export', 80), ('kind', 'Type', 110), ('name', 'Library name', 600)]:
            self.export_tree.heading(col, text=txt)
            self.export_tree.column(col, width=width, stretch=(col == 'name'))
        self.export_tree.pack(fill='both', expand=True)
        self.export_tree.bind('<Double-1>', self.toggle_export)
        bar = ttk.Frame(tab)
        bar.pack(fill='x', pady=7)
        ttk.Button(bar, text='Select all', command=lambda: self.set_export_all(True)).pack(side='left', padx=3)
        ttk.Button(bar, text='Clear all', command=lambda: self.set_export_all(False)).pack(side='left', padx=3)
        ttk.Button(bar, text='Select playlist dependencies', command=self.select_dependencies).pack(side='left', padx=3)
        ttk.Button(bar, text='Validate & generate export', command=self.export_dialog).pack(side='right', padx=3)
        self.export_info = tk.Text(tab, height=6, font=('Consolas', 10), state='disabled')
        self.export_info.pack(fill='x')

    def make_devices_tab(self):
        tab = ttk.Frame(self.tabs, padding=9)
        self.tabs.add(tab, text='Devices / sync (IP + mDNS)')
        self.devices_tab = tab
        bar = ttk.Frame(tab)
        bar.pack(fill='x', pady=4)
        ttk.Button(bar, text='Discover PulSar devices', command=self.scan_devices).pack(side='left', padx=3)
        ttk.Button(bar, text='Install mDNS dependency', command=self.install_mdns).pack(side='left', padx=3)
        ttk.Label(bar, text='mDNS: _pulsar._tcp.local.').pack(side='left', padx=10)
        ttk.Label(bar, text='Manual IP:').pack(side='left', padx=(20, 3))
        self.ip_var = tk.StringVar()
        ttk.Entry(bar, textvariable=self.ip_var, width=21).pack(side='left')
        ttk.Button(bar, text='Add IP', command=self.add_manual_device).pack(side='left', padx=3)
        self.device_tree = ttk.Treeview(tab, columns=('name', 'ip', 'port', 'module', 'mac'), show='headings', selectmode='browse')
        for col, width in [('name', 245), ('ip', 145), ('port', 60), ('module', 185), ('mac', 165)]:
            self.device_tree.heading(col, text=col.title())
            self.device_tree.column(col, width=width, stretch=(col == 'name'))
        self.device_tree.pack(fill='both', expand=True)
        self.device_tree.bind('<<TreeviewSelect>>', self.selected_device)
        effectbar = ttk.Frame(tab)
        effectbar.pack(fill='x', pady=4)
        ttk.Label(effectbar, text='Target device:').pack(side='left', padx=3)
        self.target_choice = ttk.Combobox(effectbar, state='readonly', textvariable=self.target_device, width=46)
        self.target_choice.pack(side='left', padx=4)
        self.target_choice.bind('<<ComboboxSelected>>', lambda e: self.target_changed())
        ttk.Button(effectbar, text='Use highlighted device as target', command=self.choose_target_from_selection).pack(side='left', padx=3)
        ttk.Button(effectbar, text='Read effect list (/json/effects)', command=self.refresh_target_effects).pack(side='left', padx=3)
        ttk.Button(effectbar, text='Clear target', command=self.clear_target).pack(side='left', padx=3)
        controls = ttk.Frame(tab)
        controls.pack(fill='x', pady=8)
        ttk.Button(controls, text='Download / load into editor', command=self.download_device).pack(side='left', padx=3)
        ttk.Button(controls, text='Download to file only', command=self.download_file).pack(side='left', padx=3)
        ttk.Button(controls, text='Upload selected export', command=self.upload_device).pack(side='left', padx=3)
        ttk.Button(controls, text='Test selected preset', command=self.test_current_live).pack(side='left', padx=3)
        ttk.Button(controls, text='Open device WebUI', command=self.open_device).pack(side='left', padx=3)
        ttk.Checkbutton(controls, text='Hide incompatible presets', variable=self.only_supported, command=self.refresh_compatibility_views).pack(side='left', padx=12)
        self.device_output = tk.Text(tab, height=9, font=('Consolas', 10), wrap='word')
        self.device_output.pack(fill='x')
        self.device_output.insert('end', 'Device discovery uses the same _pulsar._tcp.local. service as tools/python_scripts/mdns_scan/scan_pulsar_mdns.py.\nDownload uses GET /presets.json; upload uses POST /upload (multipart data field).\nDevice access requires the PC to share the ESP32 LAN. HTTP upload requires confirmation and makes a local backup first.\n')

    def make_effects_tab(self):
        tab = ttk.Frame(self.tabs, padding=9)
        self.tabs.add(tab, text='Effect catalog / compatibility')
        top = ttk.Frame(tab)
        top.pack(fill='x', pady=4)
        ttk.Label(top, text='Firmware Effects.cpp:').pack(side='left')
        self.effects_path = tk.StringVar()
        ttk.Entry(top, textvariable=self.effects_path).pack(side='left', padx=4, fill='x', expand=True)
        ttk.Button(top, text='Browse', command=self.browse_effects_cpp).pack(side='left', padx=3)
        ttk.Button(top, text='Parse source', command=self.load_source_effects).pack(side='left', padx=3)
        ttk.Label(tab, textvariable=self.effects_source_label).pack(anchor='w', pady=(2, 7))
        controls = ttk.Frame(tab)
        controls.pack(fill='x', pady=5)
        ttk.Checkbutton(controls, text='Hide presets not supported by selected target', variable=self.only_supported, command=self.refresh_compatibility_views).pack(side='left')
        ttk.Button(controls, text='Refresh device effects', command=self.refresh_target_effects).pack(side='left', padx=12)
        self.compat_label = tk.StringVar(value='Choose a target device to check preset compatibility.')
        ttk.Label(tab, textvariable=self.compat_label).pack(anchor='w', pady=5)
        self.effect_list = ttk.Treeview(tab, columns=('id', 'name', 'source'), show='headings')
        for name, width in (('id', 70), ('name', 400), ('source', 160)):
            self.effect_list.heading(name, text=name.upper() if name == 'id' else name.title())
            self.effect_list.column(name, width=width, stretch=(name == 'name'))
        self.effect_list.pack(fill='both', expand=True)
        ttk.Label(tab, text='Source catalog is an index of potential compiled effects; selected device /json/effects is authoritative. Unknown or unsupported effects are hidden when the filter is active. No library items are deleted.').pack(anchor='w', pady=5)
        self.refresh_effect_catalog_view()

    def refresh_effect_catalog_view(self):
        if not hasattr(self, 'effect_list'):
            return
        self.effect_list.delete(*self.effect_list.get_children())
        target = self.target_device.get()
        available = self.device_effect_cache.get(target)
        if available is not None:
            for id_, name in sorted(available.items()):
                self.effect_list.insert('', 'end', values=(id_, name, 'Device enabled'))
        else:
            for name in self.source_effects:
                self.effect_list.insert('', 'end', values=('', name, 'Source (possible)'))
        if available is not None:
            presets = [rec for rec in self.catalog.entries.values() if rec['category'] == self.category.get() and rec['kind'] == 'preset']
            n_ok = sum(preset_compatible(rec['data'], available) for rec in presets)
            self.compat_label.set(f'{target}: {len(available)} enabled effects; {n_ok}/{len(presets)} library presets compatible in {self.category.get()}')
        else:
            self.compat_label.set(f'{len(self.source_effects)} source effect names; choose a target and fetch /json/effects for accurate filtering.')
        self.update_effect_choices()
        self.update_palette_choices()

    def load_local_effects_if_available(self):
        path = find_effects_cpp()
        if path:
            self.effects_path.set(str(path))
            self.load_source_effects()
        else:
            self.effects_source_label.set('Browse to local PulSar/code/6_Lights/03_Animator/Effects.cpp to build the complete source catalog.')

    def browse_effects_cpp(self):
        path = filedialog.askopenfilename(title='Choose PulSar Effects.cpp', filetypes=[('C++ files','*.cpp'),('All files','*.*')])
        if path:
            self.effects_path.set(path)
            self.load_source_effects()

    def load_source_effects(self):
        try:
            path = pathlib.Path(self.effects_path.get())
            names = read_source_effects(path)
            self.source_effects = names
            self.effects_source_label.set(f'{len(names)} effect names found in {path.name} and related source. Source catalog is not a build-specific enabled list.')
            self.refresh_effect_catalog_view()
            if not names:
                self.status('No named effect configs extracted; device catalog still works. Check firmware naming format.')
        except Exception as exc:
            messagebox.showerror('Effect source error', str(exc))

    def target_changed(self):
        target = self.target_device.get()
        if target and target not in self.device_effect_cache:
            self.refresh_target_effects()
        else:
            self.refresh_compatibility_views()

    def choose_target_from_selection(self):
        selected = self.device_tree.selection()
        if not selected:
            messagebox.showinfo('Target device', 'Highlight a discovered or manually added device first.')
            return
        self.target_device.set(selected[0])
        self.quick_target_label.set('Target: ' + selected[0])
        self.target_changed()

    def clear_target(self):
        self.target_device.set('')
        self.quick_target_label.set('No target device selected')
        self.only_supported.set(False)
        self.refresh_compatibility_views()

    def refresh_target_effects(self):
        key = self.target_device.get()
        if not key or key not in self.devices:
            messagebox.showinfo('Device effects', 'Select a target device in the Devices tab first.')
            return
        device = dict(self.devices[key])
        self.device_log(f'Reading /json/effects from {key}...')
        def done(result, error):
            if error:
                self.device_log(f'Effect query failed for {key}: {error}')
                messagebox.showerror('Device effect list unavailable', str(error) + '\n\nFilter not changed. Previously cached effects (if any) remain.')
                return
            self.device_effect_cache[key] = result
            self.device_log(f'Loaded {len(result)} enabled effects from {key}.')
            self.refresh_compatibility_views()
        self.worker(lambda: fetch_device_effects(device['ip'], device['port']), done)

    def compatible_preset(self, key):
        rec = self.catalog.entries[key]
        if rec['kind'] != 'preset':
            return True
        effects = self.device_effect_cache.get(self.target_device.get())
        return effects is None or preset_compatible(rec['data'], effects)

    def is_visible_compatible(self, key):
        if not self.only_supported.get():
            return True
        available = self.device_effect_cache.get(self.target_device.get())
        if available is None:
            return True  # No authoritative list: never silently hide everything.
        rec = self.catalog.entries[key]
        if rec['kind'] == 'playlist':
            # Keep playlist editor available; incompatible steps are visible there.
            return True
        return preset_compatible(rec['data'], available)

    def refresh_compatibility_views(self):
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()
        self.refresh_effect_catalog_view()
        self.update_palette_choices()
        self.refresh_viewer()

    def update_effect_choices(self):
        target = self.target_device.get()
        values = self.device_effect_cache.get(target)
        if values is not None:
            fx_names = sorted(set(values.values()), key=str.casefold)
        else:
            fx_names = self.source_effects
        if hasattr(self, 'fx_choice'):
            self.fx_choice['values'] = fx_names

    def refresh_library(self):
        self.catalog.refresh()
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()
        self.refresh_effect_catalog_view()
        self.update_palette_choices()
        self.refresh_viewer()
        self.status(f'Loaded {len(self.catalog.entries)} library items. Active category: {self.category.get()}')

    def refresh_library_list(self):
        if not hasattr(self, 'library_tree'):
            return
        self.library_tree.delete(*self.library_tree.get_children())
        term = self.search_var.get().casefold()
        for key, rec in sorted(self.catalog.entries.items(), key=lambda pair: (pair[1]['kind'], pair[1]['data'].get('n','').casefold())):
            if rec['category'] != self.category.get():
                continue
            if not self.is_visible_compatible(key):
                continue
            if term and term not in rec['data'].get('n', '').casefold() and term not in key.casefold():
                continue
            self.library_tree.insert('', 'end', iid=key, values=(rec['kind'], rec['data'].get('n')))

    def selected_library(self, event=None):
        selected = self.library_tree.selection()
        if not selected:
            return
        self.current_key = selected[0]
        self.display_current()

    def display_current(self):
        self.parameter_tree.delete(*self.parameter_tree.get_children())
        if not self.current_key or self.current_key not in self.catalog.entries:
            return
        data = self.catalog.entries[self.current_key]['data']
        effects = data.get('Effects', {}) if self.catalog.entries[self.current_key]['kind'] == 'preset' else {}
        self.fx_choice['values'] = sorted({rec['data'].get('Effects', {}).get('FX', '') for rec in self.catalog.entries.values() if rec['kind'] == 'preset' and rec['data'].get('Effects', {}).get('FX')})
        self.palette_choice['values'] = sorted({rec['data'].get('Pal', '') for rec in self.catalog.entries.values() if rec['kind'] == 'preset' and rec['data'].get('Pal')})
        self.fx_var.set(effects.get('FX', effects.get('Function', '')))
        self.palette_var.set(data.get('Pal', ''))
        grouping = next((key for key in ('GPB', 'GPP', 'GP', 'GroupingPerc', 'Grouping') if key in effects), 'GP')
        self.group_var.set({'GroupingPerc':'GPP', 'Grouping':'GP'}.get(grouping, grouping))
        self.group_value.set(str(effects.get(grouping, 1)))
        self.json_paths = {}
        def descend(parent, value, path):
            if isinstance(value, dict):
                for k, v in value.items():
                    iid = self.parameter_tree.insert(parent, 'end', text=k, values=('{' + str(len(v)) + ' fields}' if isinstance(v, dict) else '[' + str(len(v)) + ' items]' if isinstance(v, list) else json.dumps(v, ensure_ascii=False),), open=True)
                    self.json_paths[iid] = path + [k]
                    descend(iid, v, path + [k])
            elif isinstance(value, list):
                for i, v in enumerate(value):
                    iid = self.parameter_tree.insert(parent, 'end', text=f'[{i}]', values=(json.dumps(v, ensure_ascii=False) if not isinstance(v, (dict, list)) else '(expand)',), open=False)
                    self.json_paths[iid] = path + [i]
                    descend(iid, v, path + [i])
        descend('', data, [])
        self.raw_text.delete('1.0', 'end')
        self.raw_text.insert('1.0', json.dumps(data, indent=2, ensure_ascii=False))

    def apply_quick(self):
        if not self.current_key or self.catalog.entries[self.current_key]['kind'] != 'preset':
            messagebox.showinfo('Preset selector', 'Select a preset (not a playlist) first.')
            return
        data = self.catalog.entries[self.current_key]['data']
        fx = self.fx_var.get().strip()
        pal = self.palette_var.get().strip()
        try:
            group_value = int(self.group_value.get())
        except ValueError:
            messagebox.showerror('Invalid grouping', 'Grouping must be an integer.')
            return
        if group_value < 0:
            messagebox.showerror('Invalid grouping', 'Grouping cannot be negative.')
            return
        effects = data.setdefault('Effects', {})
        if fx:
            effects['FX'] = fx
            effects.pop('Function', None)
        if pal:
            data['Pal'] = pal
        for key in ('GP', 'GPP', 'GPB', 'Grouping', 'GroupingPerc'):
            effects.pop(key, None)
        effects[self.group_var.get()] = group_value
        self.catalog.save(self.current_key)
        self.display_current()
        self.status('Quick editor changes saved')

    def change_param(self, event):
        if not self.current_key:
            return
        iid = self.parameter_tree.identify_row(event.y)
        if not iid or iid not in self.json_paths:
            return
        path = self.json_paths[iid]
        data = self.catalog.entries[self.current_key]['data']
        obj = data
        for step in path[:-1]:
            obj = obj[step]
        value = obj[path[-1]]
        if isinstance(value, (dict, list)):
            messagebox.showinfo('Composite property', 'Edit complex objects and arrays in the JSON pane, then click Apply JSON / Save.')
            return
        result = simpledialog.askstring('Edit parameter', f'{" / ".join(map(str, path))}\nEnter JSON value (number, true, false, null or quoted string). Unquoted text is saved as a string:', initialvalue=json.dumps(value, ensure_ascii=False), parent=self)
        if result is None:
            return
        try:
            new_value = json.loads(result)
        except json.JSONDecodeError:
            new_value = result
        if path == ['n'] and (not isinstance(new_value, str) or not new_value.strip()):
            messagebox.showerror('Invalid name', 'Preset names must be non-empty strings.')
            return
        obj[path[-1]] = new_value
        self.catalog.save(self.current_key)
        self.display_current()
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()
        self.status(f'Saved {self.current_key}')

    def apply_json(self):
        if not self.current_key:
            return
        try:
            value = json.loads(self.raw_text.get('1.0', 'end'))
            if not isinstance(value, dict) or not isinstance(value.get('n'), str) or not value['n'].strip():
                raise ValueError('The entry must be a JSON object with a nonempty "n" name')
            if entry_type(value) != self.catalog.entries[self.current_key]['kind']:
                raise ValueError('Cannot change a preset into a playlist (or vice versa) by editing JSON')
            self.catalog.entries[self.current_key]['data'] = value
            self.catalog.save(self.current_key)
            self.display_current()
            self.refresh_library_list()
            self.refresh_playlists()
            self.refresh_export()
            self.status('JSON parsed and saved successfully')
        except Exception as exc:
            messagebox.showerror('Invalid JSON', str(exc))

    def copy_json(self):
        text = self.raw_text.get('1.0', 'end').strip()
        try:
            json.loads(text)
        except json.JSONDecodeError as exc:
            messagebox.showerror('Invalid JSON', str(exc))
            return
        self.clipboard_clear()
        self.clipboard_append(text)
        self.status('Copied standalone entry JSON body (without numeric ID)')

    def create_item(self, kind):
        name = simpledialog.askstring('New ' + kind, f'{kind.title()} name:', parent=self)
        if not name:
            return
        key = self.catalog.create(kind, self.category.get(), name)
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()
        self.current_key = key
        if kind == 'preset':
            self.tabs.select(0)
            self.library_tree.selection_set(key)
            self.library_tree.see(key)
        self.display_current()
        self.status(f'Created {name}')

    def duplicate_item(self):
        if not self.current_key:
            return
        rec = self.catalog.entries[self.current_key]
        name = simpledialog.askstring('Duplicate', 'New name:', initialvalue=rec['data']['n'] + ' Copy', parent=self)
        if not name:
            return
        key = self.catalog.create(rec['kind'], rec['category'], name, rec['data'])
        self.current_key = key
        self.refresh_library_list()
        self.library_tree.selection_set(key)
        self.refresh_playlists()
        self.refresh_export()
        self.display_current()

    def delete_item(self):
        if not self.current_key:
            return
        if not messagebox.askyesno('Delete library item', f'Delete {self.catalog.entries[self.current_key]["data"]["n"]!r} from library?'):
            return
        self.catalog.delete(self.current_key)
        self.current_key = None
        self.raw_text.delete('1.0', 'end')
        self.parameter_tree.delete(*self.parameter_tree.get_children())
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()

    def refresh_playlists(self):
        if not hasattr(self, 'playlist_list'):
            return
        self.playlist_list.delete(0, 'end')
        self.playlist_keys = []
        for key, name in sorted(self.catalog.names('playlist', self.category.get()), key=lambda v: v[1].casefold()):
            self.playlist_keys.append(key)
            self.playlist_list.insert('end', name)
        preset_names = [name for key, name in sorted(self.catalog.names('preset', self.category.get()), key=lambda v: v[1].casefold()) if self.is_visible_compatible(key)]
        self.preset_choice['values'] = preset_names
        if preset_names and not self.preset_choice.get():
            self.preset_choice.set(preset_names[0])
        if self.playlist_key in self.playlist_keys:
            self.playlist_list.selection_set(self.playlist_keys.index(self.playlist_key))
            self.display_playlist()

    def selected_playlist(self, event=None):
        if not self.playlist_list.curselection():
            return
        self.playlist_key = self.playlist_keys[self.playlist_list.curselection()[0]]
        self.display_playlist()

    def display_playlist(self):
        self.playlist_steps.delete(*self.playlist_steps.get_children())
        if not self.playlist_key or self.playlist_key not in self.catalog.entries:
            return
        pl = self.catalog.entries[self.playlist_key]['data']['playlist']
        names = pl.get('psn', [])
        durs = pl.get('dur', [])
        for i, name in enumerate(names):
            duration = durs[i] if isinstance(durs, list) and i < len(durs) else '?'
            self.playlist_steps.insert('', 'end', iid=str(i), values=(name, duration))

    def add_to_playlist(self):
        if not self.playlist_key:
            messagebox.showinfo('Select playlist', 'Select or create a playlist first.')
            return
        name = self.preset_choice.get()
        if not name:
            return
        pl = self.catalog.entries[self.playlist_key]['data']['playlist']
        pl.setdefault('psn', []).append(name)
        pl.pop('ps', None)
        durs = pl.get('dur')
        if not isinstance(durs, list):
            durs = [int(durs or 100)] * (len(pl['psn']) - 1)
            pl['dur'] = durs
        durs.append(100)
        self.save_playlist()
        self.display_playlist()

    def move_step(self, delta):
        if not self.playlist_key or not self.playlist_steps.selection():
            return
        idx = int(self.playlist_steps.selection()[0])
        pl = self.catalog.entries[self.playlist_key]['data']['playlist']
        target = idx + delta
        if target < 0 or target >= len(pl.get('psn', [])):
            return
        # Keep all aligned arrays in sync (dur, transition, time_window start/end).
        for arr in [pl.get('psn'), pl.get('dur'), pl.get('transition')]:
            if isinstance(arr, list) and len(arr) == len(pl['psn']):
                arr[idx], arr[target] = arr[target], arr[idx]
        tw = pl.get('time_window', {})
        if isinstance(tw, dict):
            for arr in [tw.get('start'), tw.get('end')]:
                if isinstance(arr, list) and len(arr) == len(pl['psn']):
                    arr[idx], arr[target] = arr[target], arr[idx]
        pl.pop('ps', None)  # stale numeric IDs must be regenerated on export
        self.save_playlist()
        self.display_playlist()
        self.playlist_steps.selection_set(str(target))

    def remove_step(self):
        if not self.playlist_key or not self.playlist_steps.selection():
            return
        idx = int(self.playlist_steps.selection()[0])
        pl = self.catalog.entries[self.playlist_key]['data']['playlist']
        previous_len = len(pl['psn'])
        for arr in [pl.get('psn'), pl.get('dur'), pl.get('transition')]:
            if isinstance(arr, list) and len(arr) == previous_len:
                arr.pop(idx)
        tw = pl.get('time_window', {})
        if isinstance(tw, dict):
            for arr in [tw.get('start'), tw.get('end')]:
                if isinstance(arr, list) and len(arr) == previous_len:
                    arr.pop(idx)
        pl.pop('ps', None)
        self.save_playlist()
        self.display_playlist()

    def edit_playlist_duration(self, event):
        if not self.playlist_key:
            return
        row = self.playlist_steps.identify_row(event.y)
        if not row:
            return
        idx = int(row)
        pl = self.catalog.entries[self.playlist_key]['data']['playlist']
        durs = pl.get('dur', [])
        if not isinstance(durs, list):
            durs = [int(durs or 100)] * len(pl['psn'])
            pl['dur'] = durs
        while len(durs) < len(pl['psn']):
            durs.append(100)
        value = simpledialog.askinteger('Duration', 'Duration in 100ms ticks:', initialvalue=int(durs[idx]), minvalue=2, maxvalue=10000000, parent=self)
        if value is not None:
            durs[idx] = value
            self.save_playlist()
            self.display_playlist()

    def save_playlist(self):
        if self.playlist_key:
            self.catalog.save(self.playlist_key)
            self.status('Playlist saved; numeric IDs will be resolved during export')

    def refresh_export(self):
        if not hasattr(self, 'export_tree'):
            return
        self.export_tree.delete(*self.export_tree.get_children())
        for key, rec in sorted(self.catalog.entries.items(), key=lambda pair: (pair[1]['kind'], pair[1]['data'].get('n','').casefold())):
            if rec['category'] == self.category.get() and self.is_visible_compatible(key):
                selected = self.selected_export.get(key, True)
                self.export_tree.insert('', 'end', iid=key, values=('[x]' if selected else '[ ]', rec['kind'], rec['data'].get('n')))

    def toggle_export(self, event):
        iid = self.export_tree.identify_row(event.y)
        if iid:
            self.selected_export[iid] = not self.selected_export.get(iid, True)
            self.export_tree.set(iid, 'selected', '[x]' if self.selected_export[iid] else '[ ]')

    def set_export_all(self, selected):
        for key in self.export_tree.get_children():
            self.selected_export[key] = selected
        self.refresh_export()

    def select_dependencies(self):
        for key in self.export_tree.get_children():
            rec = self.catalog.entries[key]
            if rec['kind'] != 'playlist' or not self.selected_export.get(key, True):
                continue
            for name in rec['data']['playlist'].get('psn', []):
                for candidate, cre in self.catalog.entries.items():
                    if cre['kind'] == 'preset' and cre['category'] == self.category.get() and cre['data']['n'].casefold() == str(name).casefold():
                        self.selected_export[candidate] = True
        self.refresh_export()

    def compile(self):
        keys = [k for k in self.export_tree.get_children() if self.selected_export.get(k, True)]
        presets = [k for k in keys if self.catalog.entries[k]['kind'] == 'preset']
        playlists = [k for k in keys if self.catalog.entries[k]['kind'] == 'playlist']
        return compile_export(self.catalog, self.category.get(), presets, playlists)

    def export_dialog(self):
        try:
            doc, ids = self.compile()
        except Exception as exc:
            messagebox.showerror('Export validation failed', str(exc))
            return None
        dest = EXPORTS / dt.datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        if dest.exists():
            dest = EXPORTS / (dest.name + '_' + item_id()[:4])
        dest.mkdir(parents=True)
        write_json(dest / 'presets.json', doc)
        write_json(dest / 'manifest.json', {'created_utc': dt.datetime.now(dt.timezone.utc).isoformat(), 'category': self.category.get(), 'names_to_ids': {self.catalog.entries[k]['data']['n']: val for k, val in ids.items()}, 'entry_count': len(ids)})
        self.export_info.config(state='normal')
        self.export_info.delete('1.0', 'end')
        self.export_info.insert('1.0', f'Exported {len(ids)} entries\n{dest / "presets.json"}\n{dest / "manifest.json"}\n')
        self.export_info.config(state='disabled')
        self.last_export = dest / 'presets.json'
        self.status(f'Export saved: {self.last_export}')
        messagebox.showinfo('Export complete', f'{len(ids)} entries generated and validated.\n\n{dest / "presets.json"}')
        return dest / 'presets.json'

    def import_local(self):
        path = filedialog.askopenfilename(title='Import existing PulSar presets.json', filetypes=[('JSON', '*.json'), ('All', '*.*')])
        if not path:
            return
        replace = messagebox.askyesno('Import matching names', 'Replace existing entries with matching names in this category?\n\nYes: update existing. No: create additional library entries.')
        try:
            added, skipped = self.catalog.import_file(path, self.category.get(), replace=replace)
        except Exception as exc:
            messagebox.showerror('Import failed', str(exc))
            return
        self.refresh_library()
        self.status(f'Imported {added} entries; skipped {skipped} entries')
        messagebox.showinfo('Import complete', f'Imported {added} entries. Skipped {skipped} unnamed/empty entries.\nCategory: {self.category.get()}')

    def worker(self, callback, done):
        def task():
            try:
                self.worker_events.put((done, callback(), None))
            except Exception as exc:
                self.worker_events.put((done, None, exc))
        threading.Thread(target=task, daemon=True).start()

    def poll_workers(self):
        try:
            while True:
                done, result, error = self.worker_events.get_nowait()
                done(result, error)
        except queue.Empty:
            pass
        self.after(125, self.poll_workers)

    def device_log(self, message):
        self.device_output.insert('end', message + '\n')
        self.device_output.see('end')
        self.status(message[:160])

    def add_device(self, record):
        key = f'{record["ip"]}:{record["port"]}'
        self.devices[key] = record
        if hasattr(self, 'target_choice'):
            self.target_choice['values'] = list(self.devices)
        if self.device_tree.exists(key):
            self.device_tree.delete(key)
        self.device_tree.insert('', 'end', iid=key, values=(record.get('name', '?'), record['ip'], record['port'], record.get('module',''),record.get('mac','')))

    def scan_devices(self):
        self.device_log('Scanning PulSar mDNS services for 8 seconds...')
        def discover():
            from zeroconf import ServiceBrowser, ServiceListener, Zeroconf
            import time
            class Listener(ServiceListener):
                def __init__(self, zc):
                    self.zc, self.seen = zc, {}
                def add_service(self, zc, service_type, name):
                    self.update_service(zc, service_type, name)
                def update_service(self, zc, service_type, name):
                    info = zc.get_service_info(service_type, name, timeout=2500)
                    if not info:
                        return
                    ipv4 = [s for s in info.parsed_addresses() if ':' not in s]
                    if not ipv4:
                        return
                    props = {k.decode('utf-8','replace').lower():v.decode('utf-8','replace') for k,v in info.properties.items()}
                    self.seen[f'{ipv4[0]}:{info.port}'] = {'name':props.get('name',name.removesuffix(SERVICE).rstrip('.')), 'ip':ipv4[0], 'port':info.port, 'module':props.get('module',''), 'mac':props.get('mac','')}
                def remove_service(self, zc, service_type, name):
                    pass
            zc = Zeroconf()
            listener = Listener(zc)
            browser = ServiceBrowser(zc, SERVICE, listener)
            try:
                time.sleep(8)
                return list(listener.seen.values())
            finally:
                browser.cancel()
                zc.close()
        def done(records, error):
            if error:
                self.device_log(f'Discovery failed: {error}. Install: py -m pip install zeroconf')
                return
            for item in records:
                self.add_device(item)
            self.device_log(f'Discovery complete: {len(records)} PulSar device(s).')
        self.worker(discover, done)

    def add_manual_device(self):
        raw = self.ip_var.get().strip()
        if not raw:
            return
        parsed = urlparse(raw if '://' in raw else 'http://' + raw)
        ip, port = parsed.hostname, parsed.port or 80
        if not ip or not re.fullmatch(r'[a-zA-Z0-9_.-]+', ip):
            messagebox.showerror('Invalid address', 'Enter an IP address or local hostname, optionally followed by :port')
            return
        self.add_device({'name':ip, 'ip':ip, 'port':port, 'module':'Manual entry', 'mac':''})
        self.device_tree.selection_set(f'{ip}:{port}')
        self.target_device.set(f'{ip}:{port}')
        self.quick_target_label.set('Target: ' + f'{ip}:{port}')
        self.device_log(f'Added manual device {ip}:{port}')

    def selected_device(self, event=None):
        selected = self.device_tree.selection()
        if selected:
            self.ip_var.set(selected[0])
            self.quick_ip.set(selected[0])

    def chosen_device(self):
        selected = self.device_tree.selection()
        if not selected:
            messagebox.showinfo('Select device', 'Select a discovered device (or add an IP address) first.')
            return None
        return self.devices[selected[0]]

    def download_device(self):
        device = self.chosen_device()
        if not device:
            return
        self.device_log(f'Downloading /presets.json from {device["ip"]}...')
        def done(data, error):
            if error:
                self.device_log(f'Download failed: {error}')
                return
            backup = EXPORTS / dt.datetime.now().strftime('device_%Y-%m-%d_%H-%M-%S')
            write_json(backup / 'presets.json', data)
            self.device_log(f'Download saved: {backup / "presets.json"}')
            if messagebox.askyesno('Import downloaded presets', f'Import {len(data)} downloaded entries into the {self.category.get()} library?\nDevice: {device["ip"]}\n\nChoose No to keep the downloaded backup without importing.'):
                path = backup / 'presets.json'
                count, skipped = self.catalog.import_file(path, self.category.get(), replace=False)
                self.refresh_library()
                self.tabs.select(0)
                self.device_log(f'Imported {count} entries, skipped {skipped}. Existing names were not overwritten.')
        self.worker(lambda: http_get_presets(device['ip'], device['port']), done)

    def download_file(self):
        device = self.chosen_device()
        if not device:
            return
        target = filedialog.asksaveasfilename(title='Save device presets', defaultextension='.json', initialfile=f'presets_{file_key(device["name"])}.json', filetypes=[('JSON', '*.json')])
        if not target:
            return
        def done(data, error):
            if error:
                self.device_log(f'Download failed: {error}')
                return
            write_json(target, data)
            self.device_log(f'Saved {len(data)} entries to {target}')
        self.worker(lambda: http_get_presets(device['ip'], device['port']), done)

    def upload_device(self):
        device = self.chosen_device()
        if not device:
            return
        path = filedialog.askopenfilename(title='Choose generated presets.json to upload', initialdir=str(EXPORTS), filetypes=[('JSON', '*.json')])
        if not path:
            return
        try:
            document = read_json(path)
            if not isinstance(document, dict):
                raise ValueError('Expected a JSON object with numeric preset IDs')
        except Exception as exc:
            messagebox.showerror('Invalid upload file', str(exc))
            return
        if not messagebox.askyesno('Replace device presets.json', f'This REPLACES the device /presets.json.\n\nDevice: {device["name"]} ({device["ip"]}:{device["port"]})\nFile: {path}\nEntries: {len(document)}\n\nA backup download will be attempted first. Continue?'):
            return
        self.device_log(f'Backing up device before upload: {device["ip"]}')
        def run():
            original_bytes = http_get_presets_bytes(device['ip'], device['port'])  # fail closed if unreadable
            existing = json.loads(original_bytes.decode('utf-8-sig'))
            if not isinstance(existing, dict):
                raise ValueError('Remote presets.json is not a JSON object; no upload attempted')
            backup = EXPORTS / 'device_backups' / file_key(device['ip']) / dt.datetime.now().strftime('%Y-%m-%d_%H-%M-%S_%f')
            backup.mkdir(parents=True, exist_ok=False)
            (backup / 'presets.json').write_bytes(original_bytes)  # exact original bytes
            write_json(backup / 'backup_manifest.json', {'device':device, 'sha256':hashlib.sha256(original_bytes).hexdigest(), 'source':'GET /presets.json', 'upload_source':str(path)})
            if (backup / 'presets.json').read_bytes() != original_bytes:
                raise RuntimeError('Local backup verification failed. Upload cancelled.')
            status, message = http_upload_presets(device['ip'], device['port'], document)
            if status != 200:
                raise RuntimeError(f'Upload HTTP status {status}: {message}')
            fetched = http_get_presets(device['ip'], device['port'])
            if fetched != document:
                raise RuntimeError('Upload returned success but downloaded file differs from uploaded JSON. Check device and backup: ' + str(backup))
            return backup, status, len(document)
        def done(result, error):
            if error:
                self.device_log(f'Upload not verified: {error}')
                messagebox.showerror('Upload error', str(error))
                return
            backup, status, n = result
            self.device_log(f'Upload confirmed HTTP {status}, {n} entries; backup in {backup}')
            messagebox.showinfo('Upload successful', f'Device presets.json uploaded and read-back verified.\nBackup: {backup / "presets.json"}')
        self.worker(run, done)

    def open_device(self):
        device = self.chosen_device()
        if device:
            import webbrowser
            webbrowser.open(f'http://{device["ip"]}:{device["port"]}/')

    def dark_theme(self):
        self.configure(bg='#111419')
        style = ttk.Style(self)
        style.theme_use('clam')
        BG, PANEL, FIELD, EDGE, FG, MUTED, BLUE = '#111419', '#1b2029', '#242b36', '#384252', '#e7ebf2', '#aeb8c7', '#397bdb'
        style.configure('.', background=BG, foreground=FG, font=('Segoe UI', 10))
        style.configure('TFrame', background=BG)
        style.configure('TLabel', background=BG, foreground=FG)
        style.configure('TLabelframe', background=BG, foreground=FG, bordercolor=EDGE)
        style.configure('TLabelframe.Label', background=BG, foreground=FG)
        style.configure('TButton', background=FIELD, foreground=FG, bordercolor=EDGE, padding=(10, 6), relief='flat')
        style.map('TButton', background=[('active', '#35445a'), ('pressed', BLUE)], foreground=[('disabled','#6d7481')])
        style.configure('TEntry', fieldbackground=FIELD, foreground=FG, insertcolor=FG, bordercolor=EDGE)
        style.configure('TCombobox', fieldbackground=FIELD, background=FIELD, foreground=FG, arrowcolor=FG, bordercolor=EDGE)
        style.map('TCombobox', fieldbackground=[('readonly', FIELD)], foreground=[('readonly', FG)], selectbackground=[('readonly', FIELD)], selectforeground=[('readonly', FG)])
        style.configure('TCheckbutton', background=BG, foreground=FG)
        style.configure('TRadiobutton', background=BG, foreground=FG)
        style.configure('Profile.TRadiobutton', background=PANEL, foreground=FG, padding=(12, 8), relief='flat')
        style.map('Profile.TRadiobutton', background=[('active', '#2b3545'), ('selected', '#243f69')], foreground=[('selected', '#fff')])
        style.configure('TNotebook', background=BG, bordercolor=EDGE)
        style.configure('TNotebook.Tab', background=PANEL, foreground=MUTED, padding=(14, 9))
        style.map('TNotebook.Tab', background=[('selected', '#243f69')], foreground=[('selected', '#fff')])
        style.configure('Treeview', background=PANEL, fieldbackground=PANEL, foreground=FG, rowheight=29, bordercolor=EDGE)
        style.configure('Treeview.Heading', background=FIELD, foreground=FG, padding=(6, 8))
        style.map('Treeview', background=[('selected', BLUE)], foreground=[('selected', '#fff')])
        style.configure('Vertical.TScrollbar', background=FIELD, troughcolor=BG)
        style.configure('Horizontal.TScrollbar', background=FIELD, troughcolor=BG)
        self.option_add('*TCombobox*Listbox.background', FIELD)
        self.option_add('*TCombobox*Listbox.foreground', FG)
        self.option_add('*TCombobox*Listbox.selectBackground', BLUE)
        self.option_add('*Listbox.background', PANEL)
        self.option_add('*Listbox.foreground', FG)
        self.option_add('*Listbox.selectBackground', BLUE)
        self.option_add('*Text.background', PANEL)
        self.option_add('*Text.foreground', FG)
        self.option_add('*Text.insertBackground', '#ffffff')
        self.option_add('*Text.selectBackground', BLUE)
        self.option_add('*Canvas.background', BG)

    def quick_connect(self):
        self.ip_var.set(self.quick_ip.get())
        self.add_manual_device()
        self.tabs.select(self.devices_tab)

    def quick_discover(self):
        self.tabs.select(self.devices_tab)
        self.scan_devices()

    def quick_fetch(self):
        if not self.device_tree.selection() and self.quick_ip.get().strip():
            self.ip_var.set(self.quick_ip.get())
            self.add_manual_device()
        self.tabs.select(self.devices_tab)
        self.download_device()

    def quick_webui(self):
        if not self.device_tree.selection() and self.quick_ip.get().strip():
            self.ip_var.set(self.quick_ip.get())
            self.add_manual_device()
        self.open_device()

    def install_mdns(self):
        if not messagebox.askyesno('Install zeroconf', f'Install zeroconf into this exact Python interpreter?\n\n{sys.executable} -m pip install zeroconf'):
            return
        def run():
            return subprocess.run([sys.executable, '-m', 'pip', 'install', 'zeroconf'], capture_output=True, text=True, timeout=120)
        def done(result, error):
            if error:
                self.device_log(f'Install failed: {error}')
            else:
                self.device_log('zeroconf install result: ' + (result.stdout[-500:] if result.returncode == 0 else result.stderr[-900:]))
                if result.returncode == 0:
                    messagebox.showinfo('mDNS ready', 'zeroconf installed. Click Discover PulSar devices.')
        self.device_log('Installing zeroconf into active interpreter...')
        self.worker(run, done)

    def load_local_palettes_if_available(self):
        sources = find_palette_sources()
        self.palette_source_paths = sources
        if sources:
            self.source_palettes = read_source_palettes(sources)
        self.update_palette_choices()
        self.refresh_palette_tab()

    def update_palette_choices(self):
        choices = set(self.source_palettes)
        choices.update(palette_names_from_library(self.catalog))
        available = self.device_palette_cache.get(self.target_device.get())
        if available:
            choices.update(available.values())
        if hasattr(self, 'palette_choice'):
            self.palette_choice['values'] = sorted(choices, key=str.casefold)
        if hasattr(self, 'palette_source_status'):
            self.palette_source_status.set(f'{len(self.source_palettes)} source names, {len(choices)} including library/device names')

    def make_palette_tab(self):
        tab = ttk.Frame(self.tabs, padding=12)
        self.tabs.add(tab, text='Palettes / colours')
        heading = ttk.Frame(tab)
        heading.pack(fill='x', pady=5)
        self.palette_source_status = tk.StringVar(value='Searching for local PulSar palette headers...')
        ttk.Label(heading, textvariable=self.palette_source_status).pack(side='left')
        ttk.Button(heading, text='Scan PulSar headers', command=self.load_local_palettes_if_available).pack(side='right', padx=5)
        ttk.Button(heading, text='Fetch device palettes', command=self.refresh_device_palettes).pack(side='right', padx=5)
        ttk.Label(tab, text='Palette names are read from local C++ headers and from /json/palettes on the selected ESP32; installed device lists are authoritative.').pack(anchor='w', pady=5)
        self.palette_table = ttk.Treeview(tab, columns=('name', 'origin'), show='headings')
        self.palette_table.heading('name', text='Palette name')
        self.palette_table.heading('origin', text='Source')
        self.palette_table.column('name', width=390)
        self.palette_table.column('origin', width=180)
        self.palette_table.pack(fill='both', expand=True)
        self.refresh_palette_tab()

    def refresh_palette_tab(self):
        if not hasattr(self, 'palette_table'):
            return
        self.palette_table.delete(*self.palette_table.get_children())
        installed = self.device_palette_cache.get(self.target_device.get(), {})
        source = set(self.source_palettes)
        for name in sorted(source | set(installed.values()) | set(palette_names_from_library(self.catalog)), key=str.casefold):
            origin = 'ESP32' if name in installed.values() else 'PulSar C++' if name in source else 'Preset library'
            self.palette_table.insert('', 'end', values=(name, origin))

    def refresh_device_palettes(self):
        key = self.target_device.get()
        if not key or key not in self.devices:
            messagebox.showinfo('Target', 'Connect an ESP32 in Devices first.')
            return
        device = self.devices[key]
        def done(result, error):
            if error:
                messagebox.showerror('Palette query', str(error))
            else:
                self.device_palette_cache[key] = result
                self.device_log(f'Loaded {len(result)} palette names from {key}')
                self.update_palette_choices()
                self.refresh_palette_tab()
        self.worker(lambda: fetch_device_palettes(device['ip'], device['port']), done)

    def make_viewer_tab(self):
        tab = ttk.Frame(self.tabs, padding=9)
        self.tabs.add(tab, text='Playlist visualizer / matrix')
        controls = ttk.Frame(tab)
        controls.pack(fill='x', pady=(0, 9))
        ttk.Label(controls, text='Playlist usage matrix', font=('Segoe UI', 13, 'bold')).pack(side='left', padx=4)
        self.viewer_search = tk.StringVar()
        self.viewer_search.trace_add('write', lambda *_: self.refresh_viewer())
        ttk.Label(controls, text='Filter:').pack(side='left', padx=(28, 4))
        ttk.Entry(controls, textvariable=self.viewer_search, width=23).pack(side='left', padx=3)
        ttk.Button(controls, text='Refresh', command=self.refresh_viewer).pack(side='right', padx=4)
        self.viewer_summary = tk.StringVar(value='')
        ttk.Label(tab, textvariable=self.viewer_summary).pack(anchor='w', pady=(0, 8))
        split = ttk.Panedwindow(tab, orient='vertical')
        split.pack(fill='both', expand=True)
        top, bottom = ttk.Frame(split), ttk.Frame(split)
        split.add(top, weight=2)
        split.add(bottom, weight=3)
        playlistpan = ttk.Panedwindow(top, orient='horizontal')
        playlistpan.pack(fill='both', expand=True)
        plleft, plright = ttk.Frame(playlistpan), ttk.Frame(playlistpan)
        playlistpan.add(plleft, weight=3)
        playlistpan.add(plright, weight=2)
        self.viewer_playlists = ttk.Treeview(plleft, columns=('playlist', 'steps', 'preview'), show='headings', height=7)
        for col, width in [('playlist', 235), ('steps', 62), ('preview', 500)]:
            self.viewer_playlists.heading(col, text=col.title())
            self.viewer_playlists.column(col, width=width, stretch=col=='preview')
        self.viewer_playlists.pack(fill='both', expand=True)
        self.viewer_playlists.bind('<<TreeviewSelect>>', self.viewer_playlist_select)
        ttk.Label(plright, text='Selected playlist — sequence', font=('Segoe UI', 11, 'bold')).pack(anchor='w', padx=6, pady=4)
        self.viewer_entries = ttk.Treeview(plright, columns=('preset','duration'), show='headings', height=6)
        self.viewer_entries.heading('preset', text='Preset')
        self.viewer_entries.heading('duration', text='Duration (100ms)')
        self.viewer_entries.column('preset', width=290)
        self.viewer_entries.column('duration', width=125)
        self.viewer_entries.pack(fill='both', expand=True)
        ttk.Button(plright, text='Edit selected playlist', command=self.viewer_edit_playlist).pack(anchor='e', padx=6, pady=4)
        canvas_wrap = ttk.Frame(bottom)
        canvas_wrap.pack(fill='both', expand=True)
        self.matrix_canvas = tk.Canvas(canvas_wrap, bg='#151b24', highlightthickness=0)
        h = ttk.Scrollbar(canvas_wrap, orient='horizontal', command=self.matrix_canvas.xview)
        v = ttk.Scrollbar(canvas_wrap, orient='vertical', command=self.matrix_canvas.yview)
        self.matrix_canvas.configure(xscrollcommand=h.set, yscrollcommand=v.set)
        h.pack(side='bottom', fill='x')
        v.pack(side='right', fill='y')
        self.matrix_canvas.pack(fill='both', expand=True)
        self.matrix_canvas.bind('<Double-Button-1>', self.viewer_double_click)
        self.matrix_item_keys = {}
        ttk.Label(tab, text='Rows: presets   Columns: playlists   Colored cells: used preset. Double-click a preset row to open its JSON.').pack(anchor='w', pady=6)
        self.refresh_viewer()

    def refresh_viewer(self):
        if not hasattr(self, 'viewer_playlists'):
            return
        category = self.category.get()
        presets = [(k, v) for k, v in self.catalog.entries.items() if v['category']==category and v['kind']=='preset']
        playlists = [(k, v) for k, v in self.catalog.entries.items() if v['category']==category and v['kind']=='playlist']
        playlists.sort(key=lambda t: t[1]['data'].get('n', '').casefold())
        names = {v['data'].get('n','').casefold():k for k,v in presets}
        if self.only_supported.get() and self.target_device.get() in self.device_effect_cache:
            presets = [(k,v) for k,v in presets if self.is_visible_compatible(k)]
        term = self.viewer_search.get().strip().casefold()
        if term:
            presets = [(k,v) for k,v in presets if term in v['data'].get('n','').casefold()]
        presets.sort(key=lambda t:t[1]['data'].get('n','').casefold())
        self.viewer_playlists.delete(*self.viewer_playlists.get_children())
        for j,(k,v) in enumerate(playlists):
            pl = v['data'].get('playlist',{})
            psn = pl.get('psn',[])
            self.viewer_playlists.insert('', 'end', iid=k, values=(v['data'].get('n'), len(psn), '  →  '.join(psn[:8])+(' ...' if len(psn)>8 else '')))
        self.viewer_summary.set(f'{len(presets)} presets · {len(playlists)} playlists · {sum(len(v["data"].get("playlist",{}).get("psn",[])) for _,v in playlists)} playlist placements')
        c = self.matrix_canvas
        c.delete('all')
        self.matrix_item_keys={}
        left, top, row_h, col_w, title_h = 300, 80, 27, 66, 80
        width = left + len(playlists)*col_w + 30
        height = top + len(presets)*row_h + 25
        c.configure(scrollregion=(0,0,width,height))
        for j,(k,entry) in enumerate(playlists):
            col='#17314f' if j%2 else '#1d2737'
            x=left+j*col_w
            c.create_rectangle(x, 0, x+col_w, height, fill=col, outline='')
            c.create_text(x+col_w/2, 14, text=str(j+1), fill='#91a7c1', font=('Segoe UI',9))
            name=entry['data'].get('n',k)
            c.create_text(x+col_w/2, 44, text=name[:10], fill='#e6edf8', font=('Segoe UI',9), width=col_w-4)
        for i,(key, rec) in enumerate(presets):
            y=top+i*row_h
            name=rec['data'].get('n', key)
            c.create_rectangle(0,y,left,y+row_h,fill='#1d2632' if i%2 else '#151c26',outline='#303b4b')
            c.create_text(12,y+row_h/2,text=name[:44],fill='#e2e8f1',anchor='w',font=('Segoe UI',10))
            self.matrix_item_keys[i] = key
            for j,(_, plrec) in enumerate(playlists):
                in_playlist=name.casefold() in {str(v).casefold() for v in plrec['data'].get('playlist',{}).get('psn',[])}
                x=left+j*col_w
                if in_playlist:
                    palette=['#317ec0','#44a89d','#ab76cf','#d2a04c','#c06884','#6d9ac1'][j%6]
                    c.create_rectangle(x+7,y+4,x+col_w-7,y+row_h-4,fill=palette,outline='')
                c.create_line(x,y+row_h,x+col_w,y+row_h,fill='#2e394a')
        c.create_line(left,0,left,height,fill='#61758c')

    def viewer_playlist_select(self, event=None):
        selected=self.viewer_playlists.selection()
        if not selected:
            return
        key=selected[0]
        item=self.catalog.entries.get(key)
        if not item:
            return
        self.viewed_playlist_key=key
        self.viewer_entries.delete(*self.viewer_entries.get_children())
        pl=item['data'].get('playlist',{})
        for i,name in enumerate(pl.get('psn',[])):
            durations=pl.get('dur',[])
            duration=durations[i] if isinstance(durations,list) and i<len(durations) else 100
            self.viewer_entries.insert('', 'end', values=(name,duration))

    def viewer_edit_playlist(self):
        key=getattr(self,'viewed_playlist_key',None)
        if not key:
            return
        self.playlist_key=key
        self.tabs.select(1)
        self.refresh_playlists()
        if key in getattr(self,'playlist_keys',[]):
            i=self.playlist_keys.index(key)
            self.playlist_list.selection_clear(0,'end')
            self.playlist_list.selection_set(i)
            self.selected_playlist()

    def viewer_double_click(self, event):
        y=self.matrix_canvas.canvasy(event.y)
        idx=int((y-80)//27)
        key=self.matrix_item_keys.get(idx)
        if not key:
            return
        self.current_key=key
        self.tabs.select(0)
        self.refresh_library_list()
        self.display_current()

    def make_webui_tab(self):
        tab = ttk.Frame(self.tabs, padding=12)
        self.tabs.add(tab, text='Live test / device WebUI')
        ttk.Label(tab, text='Embedded browser: optional pywebview window; full PulSar WebUI uses JavaScript and is not native Tkinter.').pack(anchor='w', pady=7)
        bar=ttk.Frame(tab)
        bar.pack(fill='x', pady=5)
        ttk.Button(bar, text='Test current preset LIVE', command=self.test_current_live).pack(side='left', padx=4)
        ttk.Button(bar, text='Launch WebUI window (pywebview)', command=self.launch_webview).pack(side='left', padx=4)
        ttk.Button(bar, text='Open device in default browser', command=self.open_device).pack(side='left', padx=4)
        ttk.Button(bar, text='Open device playlist viewer', command=lambda:self.open_webui_path('/lights/playlist_viewer')).pack(side='left', padx=4)
        ttk.Button(bar, text='Open device lights', command=lambda:self.open_webui_path('/lights')).pack(side='left', padx=4)
        ttk.Label(tab, text='The native Playlist visualizer / matrix tab works offline. For full effects, palette pickers, virtual views and live preview, use the firmware WebUI.').pack(anchor='w', pady=12)

    def open_webui_path(self,path):
        device=self.chosen_device()
        if device:
            webbrowser.open(f'http://{device["ip"]}:{device["port"]}{path}')

    def launch_webview(self):
        device=self.chosen_device()
        if not device:
            return
        try:
            import webview
        except ImportError:
            messagebox.showinfo('Optional browser', f'Install pywebview into {sys.executable}\n\n{sys.executable} -m pip install pywebview\n\nOr use Open device in default browser for full WebUI.')
            return
        subprocess.Popen([sys.executable, str(BASE / 'webui_window.py'), f'http://{device["ip"]}:{device["port"]}/'], cwd=str(BASE))

    def test_current_live(self):
        if self.current_key is None or self.current_key not in self.catalog.entries:
            messagebox.showinfo('Live test', 'Select a preset in the library first.')
            return
        rec=self.catalog.entries[self.current_key]
        if rec['kind'] != 'preset':
            messagebox.showinfo('Live test', 'Select an effect preset, rather than a playlist, for direct live preview.')
            return
        dev=self.chosen_device()
        if dev is None:
            return
        try:
            data=json.loads(self.raw_text.get('1.0','end'))
            if not isinstance(data,dict):
                raise ValueError('Selected preset must be a JSON object')
            payload={k:v for k,v in data.items() if k not in ('n','ql','ps','playlist')}
            payload['v']=True
        except Exception as exc:
            messagebox.showerror('Live test',str(exc))
            return
        def work():
            request=Request(f'http://{dev["ip"]}:{dev["port"]}/json/si',data=json.dumps(payload,separators=(',',':')).encode('utf-8'),headers={'Content-Type':'application/json; charset=UTF-8'},method='POST')
            with urlopen(request,timeout=10) as resp:
                return resp.status,resp.read(1000).decode('utf-8','replace')
        def done(result,error):
            if error:
                self.device_log(f'Live effect test failed: {error}')
                messagebox.showerror('Live test',str(error))
            else:
                self.device_log(f'Live preset sent to {dev["ip"]}: HTTP {result[0]} — {data.get("n", "unnamed")}')
        self.device_log(f'Sending live effect preview to {dev["ip"]} (does not write presets.json)')
        self.worker(work,done)


if __name__ == '__main__':
    App().mainloop()
