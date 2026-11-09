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
LIBRARY = BASE / 'lib'
LEGACY_LIBRARY = BASE / 'library'
EXPORTS = BASE / 'exports'
WORKSPACE_FILE = BASE / 'workspace' / 'working_set.json'
CATEGORIES = ('1d', '2d')
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
    """Exactly three source-of-truth JSON arrays. No device preset IDs in the library."""
    def __init__(self):
        self.entries = {}
        LIBRARY.mkdir(parents=True, exist_ok=True)
        self.migrate_legacy_if_needed()
        self.refresh()
        self.archive_obsolete_library_files()

    @staticmethod
    def storage_file(kind, category=None):
        if kind == 'playlist':
            return LIBRARY / 'playlists.json'
        if kind == 'preset' and category in ('1d', '2d'):
            return LIBRARY / f'presets_{category}.json'
        raise ValueError('Presets must belong to 1d or 2d; all playlists belong to playlists.json')

    @staticmethod
    def key_for(kind, category, index):
        return f'{kind}/{category}/{index}'

    def migrate_legacy_if_needed(self):
        """Import older arrays or old individual JSON resources only when the new file is absent."""
        sources = (
            ('preset', '1d', ('presets_1d.json',)),
            ('preset', '2d', ('presets_2d.json', 'presets_audio_reactive.json')),
            ('playlist', 'shared', ('playlists_1d.json', 'playlists_2d.json', 'playlists_audio_reactive.json')),
        )
        for kind, category, filenames in sources:
            target = self.storage_file(kind, category)
            if target.exists():
                continue
            records = []
            source_files_found = False
            for filename in filenames:
                previous = LIBRARY / filename
                if previous.is_file():
                    source_files_found = True
                    docs = read_json(previous)
                    if not isinstance(docs, list):
                        raise ValueError(f'Expected JSON array in {previous}')
                    for doc in docs:
                        if not isinstance(doc, dict) or not isinstance(doc.get('n'), str) or entry_type(doc) != kind:
                            raise ValueError(f'Invalid record in {previous}')
                        if kind == 'playlist':
                            doc = copy.deepcopy(doc)
                            doc['playlist'].pop('ps', None)
                        records.append(doc)
            if not source_files_found:
                legacy_categories = ('1d', '2d', 'audio_reactive') if kind == 'playlist' else (('1d',) if category == '1d' else ('2d', 'audio_reactive'))
                for old_category in legacy_categories:
                    folder = LEGACY_LIBRARY / ('presets' if kind == 'preset' else 'playlists') / old_category
                    if not folder.is_dir():
                        continue
                    for source in sorted(folder.glob('*.json')):
                        doc = read_json(source)
                        if isinstance(doc, dict) and isinstance(doc.get('n'), str) and entry_type(doc) == kind:
                            if kind == 'playlist':
                                doc = copy.deepcopy(doc)
                                doc['playlist'].pop('ps', None)
                            records.append(doc)
            write_json(target, records)

    def archive_obsolete_library_files(self):
        """Keep only the three current JSON arrays inside lib/, saving obsolete arrays elsewhere."""
        retired = ('presets_audio_reactive.json', 'playlists_1d.json', 'playlists_2d.json', 'playlists_audio_reactive.json', '_imported_ids.json')
        paths = [LIBRARY / name for name in retired if (LIBRARY / name).exists()]
        if not paths:
            return
        archive_root = LIBRARY.parent / 'migration_backups'
        archive = archive_root / dt.datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        archive.mkdir(parents=True, exist_ok=True)
        for path in paths:
            destination = archive / path.name
            if destination.exists():
                destination = archive / f'{path.stem}_{item_id()[:6]}{path.suffix}'
            os.replace(path, destination)

    def refresh(self):
        self.entries = {}
        for kind, category in (('preset', '1d'), ('preset', '2d'), ('playlist', 'shared')):
            path = self.storage_file(kind, category)
            if not path.exists():
                write_json(path, [])
            documents = read_json(path)
            if not isinstance(documents, list):
                raise ValueError(f'{path} must contain a JSON array')
            for index, doc in enumerate(documents):
                if not isinstance(doc, dict) or not isinstance(doc.get('n'), str) or entry_type(doc) != kind:
                    raise ValueError(f'Invalid {kind} at {path}, entry {index}')
                key = self.key_for(kind, category, index)
                self.entries[key] = {'kind': kind, 'category': category, 'data': doc, 'path': path, 'index': index}

    def save_group(self, kind, category):
        records = [rec for rec in self.entries.values() if rec['kind'] == kind and rec['category'] == category]
        records.sort(key=lambda rec: rec['index'])
        if kind == 'playlist':
            for rec in records:
                rec['data'].get('playlist', {}).pop('ps', None)
        if [rec['index'] for rec in records] != list(range(len(records))):
            raise ValueError('Library array indices inconsistent; reload before saving')
        write_json(self.storage_file(kind, category), [rec['data'] for rec in records])

    def save(self, key):
        rec = self.entries[key]
        self.save_group(rec['kind'], rec['category'])

    def create(self, kind, category, name, data=None):
        if kind == 'playlist':
            category = 'shared'
        elif category not in ('1d', '2d'):
            raise ValueError('Choose 1d or 2d for a preset')
        if not name.strip():
            raise ValueError('Name must not be empty')
        doc = copy.deepcopy(data if data is not None else new_entry(name, kind))
        doc['n'] = name
        if kind == 'playlist' and isinstance(doc.get('playlist'), dict):
            doc['playlist'].pop('ps', None)  # Device-only generated IDs, never library content
        index = sum(rec['kind'] == kind and rec['category'] == category for rec in self.entries.values())
        key = self.key_for(kind, category, index)
        self.entries[key] = {'kind': kind, 'category': category, 'data': doc, 'path': self.storage_file(kind, category), 'index': index}
        self.save(key)
        return key

    def delete(self, key):
        rec = self.entries[key]
        kind, category = rec['kind'], rec['category']
        remaining = [r for k, r in self.entries.items() if k != key and r['kind'] == kind and r['category'] == category]
        remaining.sort(key=lambda r: r['index'])
        if kind == 'playlist':
            for record in remaining:
                record['data'].get('playlist', {}).pop('ps', None)
        write_json(self.storage_file(kind, category), [r['data'] for r in remaining])
        self.refresh()

    def names(self, kind, category=None):
        return [(key, rec['data'].get('n', key)) for key, rec in self.entries.items() if rec['kind'] == kind and (category is None or rec['category'] == category)]

    def import_file(self, path, category, replace=False):
        obj = read_json(path)
        if not isinstance(obj, dict):
            raise ValueError('Expected device presets.json with numeric preset ID keys')
        if category not in ('1d', '2d'):
            raise ValueError('Choose 1d or 2d for imported presets')
        imported, skipped = 0, 0
        id_to_name = {str(k): str(v.get('n')) for k, v in obj.items() if isinstance(v, dict) and v.get('n') and 'playlist' not in v}
        for old_id, source in sorted(obj.items(), key=lambda pair: int(pair[0]) if str(pair[0]).isdigit() else -1):
            if not isinstance(source, dict) or not source.get('n'):
                skipped += 1
                continue
            data = copy.deepcopy(source)
            kind = entry_type(data)
            if kind == 'playlist':
                pl = data['playlist']
                if not pl.get('psn') and isinstance(pl.get('ps'), list):
                    pl['psn'] = [id_to_name.get(str(v), f'UNRESOLVED_PRESET_{v}') for v in pl['ps']]
                pl.pop('ps', None)
            name = str(data['n'])
            dest_category = 'shared' if kind == 'playlist' else category
            matches = [(key, rec) for key, rec in self.entries.items() if rec['category'] == dest_category and rec['kind'] == kind and rec['data'].get('n') == name]
            if matches and replace:
                key, rec = matches[0]
                rec['data'] = data
                self.save(key)
            else:
                self.create(kind, dest_category, name, data)
            imported += 1
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
    # Numeric preset IDs exist ONLY in generated device presets.json, never the library.
    keys_to_ids = {key: index for index, key in enumerate(selected, start=1)}
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


class BuildWorkspace:
    """Independent editable working set. Never writes to the master lib/ arrays."""
    def __init__(self, path=WORKSPACE_FILE):
        self.path = pathlib.Path(path)
        self.entries = {}
        self.load()

    def load(self):
        self.entries = {}
        if not self.path.is_file():
            return
        raw = read_json(self.path)
        if not isinstance(raw, list):
            raise ValueError('Build workspace must be a JSON array')
        for item in raw:
            if not isinstance(item, dict) or not isinstance(item.get('data'), dict):
                raise ValueError('Invalid workspace entry')
            data = item['data']
            kind = entry_type(data)
            category = item.get('category', 'shared' if kind == 'playlist' else '1d')
            key = item.get('key') or item_id()
            if key in self.entries:
                raise ValueError('Duplicate workspace entry key')
            if kind == 'playlist':
                data.get('playlist', {}).pop('ps', None)
            self.entries[key] = {'kind': kind, 'category': category, 'data': data, 'origin': item.get('origin', '')}

    def save(self):
        write_json(self.path, [{'key':k, 'category':r['category'], 'origin':r.get('origin', ''), 'data':r['data']} for k,r in self.entries.items()])

    def add(self, data, category, origin=''):
        document = copy.deepcopy(data)
        kind = entry_type(document)
        if kind == 'playlist':
            document['playlist'].pop('ps', None)
            category = 'shared'
        elif category not in ('1d', '2d'):
            raise ValueError('Choose 1d or 2d for an effect preset')
        key = item_id()
        while key in self.entries:
            key = item_id()
        self.entries[key] = {'kind':kind, 'category':category, 'data':document, 'origin':origin}
        self.save()
        return key

    def delete(self, key):
        del self.entries[key]
        self.save()


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
        self.workspace = BuildWorkspace()
        self.draft_key = None
        self.category = tk.StringVar(value='1d')  # Storage destination only; never filters the GUI
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
        self.refresh_workspace()
        self.after(125, self.poll_workers)
        self.after(350, self.load_local_palettes_if_available)
        self.after(450, self.load_local_effects_if_available)

    def status(self, message):
        self.status_var.set(message)

    def make_ui(self):
        root = ttk.Frame(self, padding=10)
        root.pack(fill='both', expand=True)
        title = ttk.Frame(root)
        title.pack(fill='x', pady=(0, 8))
        ttk.Label(title, text='PulSar Preset & Playlist Designer', font=('Segoe UI', 17, 'bold')).pack(side='left')
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
        self.make_workspace_tab()
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
        tab = ttk.Frame(self.tabs, padding=7)
        self.tabs.add(tab, text='Preset library')
        panes = ttk.Panedwindow(tab, orient='horizontal')
        panes.pack(fill='both', expand=True)
        left, middle, right = ttk.Frame(panes), ttk.Frame(panes), ttk.Frame(panes)
        panes.add(left, weight=2)
        panes.add(middle, weight=3)
        panes.add(right, weight=3)
        toolbar = ttk.Frame(left)
        toolbar.pack(fill='x', pady=3)
        ttk.Button(toolbar, text='New', command=lambda: self.create_item('preset')).pack(side='left', padx=2)
        ttk.Button(toolbar, text='Duplicate', command=self.duplicate_item).pack(side='left', padx=2)
        ttk.Button(toolbar, text='Delete', command=self.delete_item).pack(side='left', padx=2)
        ttk.Button(left, text='Copy to Device Build  →', command=self.add_selected_library_to_build).pack(fill='x', pady=5)
        self.search_var = tk.StringVar()
        self.search_var.trace_add('write', lambda *_: self.refresh_library_list())
        ttk.Entry(left, textvariable=self.search_var).pack(fill='x', pady=4)
        ttk.Checkbutton(left, text='Only effects on target ESP32', variable=self.only_supported, command=self.refresh_compatibility_views).pack(anchor='w', pady=3)
        self.library_tree = ttk.Treeview(left, columns=('name',), show='headings', selectmode='browse')
        self.library_tree.heading('name', text='Preset templates')
        self.library_tree.column('name', width=270)
        self.library_tree.pack(fill='both', expand=True)
        self.library_tree.bind('<<TreeviewSelect>>', self.selected_library)
        ttk.Label(middle, text='Edit library preset', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=(3, 5))
        quick = ttk.Frame(middle)
        quick.pack(fill='x', pady=(0, 6))
        ttk.Label(quick, text='Effect').pack(side='left')
        self.fx_var = tk.StringVar()
        self.fx_choice = ttk.Combobox(quick, textvariable=self.fx_var, width=21)
        self.fx_choice.pack(side='left', padx=3, fill='x', expand=True)
        ttk.Label(quick, text='Palette').pack(side='left')
        self.palette_var = tk.StringVar()
        self.palette_choice = ttk.Combobox(quick, textvariable=self.palette_var, width=19)
        self.palette_choice.pack(side='left', padx=3, fill='x', expand=True)
        grouping = ttk.Frame(middle)
        grouping.pack(fill='x', pady=(0, 5))
        ttk.Label(grouping, text='Grouping').pack(side='left')
        self.group_var = tk.StringVar(value='GP')
        self.group_choice = ttk.Combobox(grouping, textvariable=self.group_var, values=['GP','GPP','GPB'], width=5, state='readonly')
        self.group_choice.pack(side='left', padx=4)
        self.group_value = tk.StringVar(value='1')
        ttk.Entry(grouping, textvariable=self.group_value, width=7).pack(side='left')
        ttk.Button(grouping, text='Apply to library', command=self.apply_quick).pack(side='right', padx=3)
        self.parameter_tree = ttk.Treeview(middle, columns=('value',), show='tree headings', selectmode='browse')
        self.parameter_tree.heading('#0', text='Property (double-click to edit)')
        self.parameter_tree.heading('value', text='Value')
        self.parameter_tree.column('#0', width=185)
        self.parameter_tree.column('value', width=180)
        self.parameter_tree.pack(fill='both', expand=True)
        self.parameter_tree.bind('<Double-1>', self.change_param)
        ttk.Label(right, text='Library JSON — exact preset body', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=(3, 5))
        bar = ttk.Frame(right)
        bar.pack(fill='x', pady=(0, 5))
        ttk.Button(bar, text='Copy JSON', command=self.copy_json).pack(side='right', padx=3)
        ttk.Button(bar, text='Save to library', command=self.apply_json).pack(side='right', padx=3)
        self.raw_text = tk.Text(right, font=('Consolas', 10), wrap='none', undo=True)
        self.raw_text.pack(fill='both', expand=True)
        ttk.Label(right, text='Changes here update the master library only when saved.').pack(anchor='w', pady=5)

    def make_workspace_tab(self):
        tab = ttk.Frame(self.tabs, padding=7)
        self.tabs.add(tab, text='Device Build')
        self.workspace_tab = tab
        panes = ttk.Panedwindow(tab, orient='horizontal')
        panes.pack(fill='both', expand=True)
        left, middle, right = ttk.Frame(panes), ttk.Frame(panes), ttk.Frame(panes)
        panes.add(left, weight=2)
        panes.add(middle, weight=3)
        panes.add(right, weight=3)
        ttk.Label(left, text='Working presets / playlists', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        sourcebar = ttk.Frame(left)
        sourcebar.pack(fill='x', pady=3)
        self.template_choice = ttk.Combobox(sourcebar, state='readonly')
        self.template_choice.pack(side='left', fill='x', expand=True, padx=(0, 3))
        ttk.Button(sourcebar, text='Add copy', command=self.workspace_add_template).pack(side='left')
        buttons = ttk.Frame(left)
        buttons.pack(fill='x', pady=(2, 6))
        ttk.Button(buttons, text='Duplicate', command=self.workspace_duplicate).pack(side='left', padx=2)
        ttk.Button(buttons, text='Remove', command=self.workspace_remove).pack(side='left', padx=2)
        self.workspace_tree = ttk.Treeview(left, columns=('name',), show='headings', selectmode='browse')
        self.workspace_tree.heading('name', text='Device build items')
        self.workspace_tree.column('name', width=290)
        self.workspace_tree.pack(fill='both', expand=True)
        self.workspace_tree.bind('<<TreeviewSelect>>', self.workspace_selected)
        ttk.Label(middle, text='Edit working copy', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        tools = ttk.Frame(middle)
        tools.pack(fill='x', pady=(0, 5))
        ttk.Button(tools, text='Save copy to library', command=self.workspace_save_to_library).pack(side='left', padx=2)
        ttk.Button(tools, text='Test live', command=self.workspace_test_live).pack(side='left', padx=2)
        self.workspace_params = ttk.Treeview(middle, columns=('value',), show='tree headings')
        self.workspace_params.heading('#0', text='Property (double-click to edit)')
        self.workspace_params.heading('value', text='Value')
        self.workspace_params.column('#0', width=190)
        self.workspace_params.column('value', width=185)
        self.workspace_params.pack(fill='both', expand=True)
        self.workspace_params.bind('<Double-1>', self.workspace_edit_parameter)
        ttk.Label(right, text='Working JSON — device build source', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        toolsright = ttk.Frame(right)
        toolsright.pack(fill='x', pady=(0, 5))
        ttk.Button(toolsright, text='Copy JSON', command=self.workspace_copy_json).pack(side='right', padx=3)
        ttk.Button(toolsright, text='Apply JSON to copy', command=self.workspace_apply_json).pack(side='right', padx=3)
        self.workspace_raw = tk.Text(right, font=('Consolas', 10), wrap='none', undo=True)
        self.workspace_raw.pack(fill='both', expand=True)
        ttk.Label(right, text='Edits stay in Device Build; templates remain unchanged until explicitly saved to library.').pack(anchor='w', pady=5)

    def refresh_workspace(self):
        if not hasattr(self, 'workspace_tree'):
            return
        selection = self.draft_key
        self.workspace_tree.delete(*self.workspace_tree.get_children())
        for key, rec in self.workspace.entries.items():
            self.workspace_tree.insert('', 'end', iid=key, values=(('♫ ' if rec['kind']=='playlist' else '◉ ') + rec['data'].get('n','Unnamed'),))
        self.template_sources = {}
        choices = []
        for key, rec in sorted(self.catalog.entries.items(), key=lambda pair: pair[1]['data'].get('n','').casefold()):
            label = f'{"Playlist" if rec["kind"]=="playlist" else rec["category"].upper()} · {rec["data"].get("n", "")}'
            if label in self.template_sources:
                label += f' [{key}]'
            self.template_sources[label] = key
            choices.append(label)
        self.template_choice['values'] = choices
        if choices and self.template_choice.get() not in self.template_sources:
            self.template_choice.set(choices[0])
        if selection in self.workspace.entries:
            self.workspace_tree.selection_set(selection)
            self.workspace_tree.see(selection)
            self.workspace_display()
        else:
            self.draft_key = None
            self.workspace_params.delete(*self.workspace_params.get_children())
            self.workspace_raw.delete('1.0', 'end')
        self.refresh_export()

    def add_selected_library_to_build(self):
        if not self.current_key or self.current_key not in self.catalog.entries:
            messagebox.showinfo('Select template', 'Select a library preset first.')
            return
        self.workspace_copy_from_key(self.current_key)

    def workspace_add_template(self):
        key = self.template_sources.get(self.template_choice.get())
        if key:
            self.workspace_copy_from_key(key)

    def workspace_copy_from_key(self, key):
        rec = self.catalog.entries[key]
        self.draft_key = self.workspace.add(rec['data'], rec['category'], origin=rec['data']['n'])
        self.refresh_workspace()
        self.tabs.select(self.workspace_tab)
        self.status(f'Working copy created from {rec["data"]["n"]}; library remains unchanged')

    def workspace_selected(self, event=None):
        selected = self.workspace_tree.selection()
        if selected:
            self.draft_key = selected[0]
            self.workspace_display()

    def workspace_display(self):
        self.workspace_params.delete(*self.workspace_params.get_children())
        if self.draft_key not in self.workspace.entries:
            return
        data = self.workspace.entries[self.draft_key]['data']
        self.workspace_paths = {}
        def descend(parent, obj, path):
            if isinstance(obj, dict):
                items = obj.items()
            elif isinstance(obj, list):
                items = enumerate(obj)
            else:
                return
            for k, value in items:
                label = str(k) if isinstance(obj, dict) else f'[{k}]'
                display = f'{{{len(value)} fields}}' if isinstance(value, dict) else f'[{len(value)} items]' if isinstance(value, list) else json.dumps(value, ensure_ascii=False)
                iid = self.workspace_params.insert(parent, 'end', text=label, values=(display,), open=not isinstance(value, list))
                self.workspace_paths[iid] = path + [k]
                descend(iid, value, path + [k])
        descend('', data, [])
        self.workspace_raw.delete('1.0','end')
        self.workspace_raw.insert('1.0', json.dumps(data, indent=2, ensure_ascii=False))

    def workspace_edit_parameter(self, event):
        if self.draft_key not in self.workspace.entries:
            return
        iid = self.workspace_params.identify_row(event.y)
        if iid not in getattr(self, 'workspace_paths', {}):
            return
        path = self.workspace_paths[iid]
        obj = self.workspace.entries[self.draft_key]['data']
        for step in path[:-1]:
            obj = obj[step]
        previous = obj[path[-1]]
        if isinstance(previous, (list, dict)):
            messagebox.showinfo('JSON editor', 'Edit arrays/objects in the JSON panel.')
            return
        raw = simpledialog.askstring('Edit working value', '/'.join(map(str,path)), initialvalue=json.dumps(previous,ensure_ascii=False), parent=self)
        if raw is None:
            return
        try:
            value = json.loads(raw)
        except json.JSONDecodeError:
            value = raw
        if path == ['n'] and (not isinstance(value, str) or not value.strip()):
            messagebox.showerror('Invalid name', 'Name must be a nonempty string.')
            return
        obj[path[-1]] = value
        self.workspace.entries[self.draft_key]['data'].get('playlist', {}).pop('ps', None)
        self.workspace.save()
        self.refresh_workspace()
        self.status('Working copy updated (library unchanged)')

    def workspace_apply_json(self):
        if self.draft_key not in self.workspace.entries:
            return
        try:
            value = json.loads(self.workspace_raw.get('1.0','end'))
            if not isinstance(value, dict) or not isinstance(value.get('n'), str) or not value['n'].strip():
                raise ValueError('JSON must have a nonempty n name')
            rec = self.workspace.entries[self.draft_key]
            if entry_type(value) != rec['kind']:
                raise ValueError('Cannot change a preset into a playlist or vice versa')
            if rec['kind']=='playlist':
                value['playlist'].pop('ps',None)
            rec['data'] = value
            self.workspace.save()
            self.refresh_workspace()
            self.status('Working JSON saved (library unchanged)')
        except Exception as exc:
            messagebox.showerror('Invalid working JSON', str(exc))

    def workspace_copy_json(self):
        try:
            data = json.loads(self.workspace_raw.get('1.0','end'))
            if not isinstance(data, dict):
                raise ValueError('Expected JSON object')
        except Exception as exc:
            messagebox.showerror('Invalid JSON', str(exc))
            return
        self.clipboard_clear()
        self.clipboard_append(json.dumps(data, indent=2, ensure_ascii=False))
        self.status('Working JSON copied')

    def workspace_duplicate(self):
        if self.draft_key not in self.workspace.entries:
            return
        rec = self.workspace.entries[self.draft_key]
        data = copy.deepcopy(rec['data'])
        name = simpledialog.askstring('Duplicate build item', 'New name:', initialvalue=data['n']+' Copy', parent=self)
        if not name:
            return
        data['n'] = name
        self.draft_key = self.workspace.add(data, rec['category'], origin=rec.get('origin',''))
        self.refresh_workspace()

    def workspace_remove(self):
        if self.draft_key not in self.workspace.entries:
            return
        rec = self.workspace.entries[self.draft_key]
        if messagebox.askyesno('Remove working item', f'Remove {rec["data"]["n"]!r} from the device build? This does not delete its library template.'):
            self.workspace.delete(self.draft_key)
            self.draft_key = None
            self.refresh_workspace()

    def workspace_save_to_library(self):
        if self.draft_key not in self.workspace.entries:
            return
        rec = self.workspace.entries[self.draft_key]
        data = rec['data']
        name = data['n']
        matches = [(k,r) for k,r in self.catalog.entries.items() if r['kind']==rec['kind'] and r['data']['n'].casefold()==name.casefold()]
        if matches:
            if len(matches) > 1:
                messagebox.showerror('Ambiguous library name', 'Multiple library templates have this name. Rename the working item first.')
                return
            if not messagebox.askyesno('Replace library template?', f'Overwrite existing library entry {name!r} with this working copy?'):
                return
            key, original = matches[0]
            original['data'] = copy.deepcopy(data)
            self.catalog.save(key)
        else:
            if not messagebox.askyesno('Add to library?', f'Add {name!r} to the master library?'):
                return
            key = self.catalog.create(rec['kind'],rec['category'], name, data)
        self.refresh_library()
        self.refresh_workspace()
        self.status(f'Library template saved: {name}')

    def workspace_test_live(self):
        if self.draft_key not in self.workspace.entries:
            return
        if self.workspace.entries[self.draft_key]['kind'] != 'preset':
            messagebox.showinfo('Live test', 'Select a working preset rather than a playlist.')
            return
        try:
            doc = json.loads(self.workspace_raw.get('1.0','end'))
        except Exception as exc:
            messagebox.showerror('Invalid JSON',str(exc))
            return
        self.send_live_preset(doc)

    def make_playlist_tab(self):
        tab = ttk.Frame(self.tabs, padding=7)
        self.tabs.add(tab, text='Playlist editor')
        self.playlist_tab = tab
        panes = ttk.Panedwindow(tab, orient='horizontal')
        panes.pack(fill='both', expand=True)
        left, middle, right = ttk.Frame(panes), ttk.Frame(panes), ttk.Frame(panes)
        panes.add(left, weight=2)
        panes.add(middle, weight=3)
        panes.add(right, weight=3)
        ttk.Label(left, text='Library playlists', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        ttk.Button(left, text='New playlist', command=lambda: self.create_item('playlist')).pack(fill='x', pady=3)
        ttk.Button(left, text='Copy playlist to Device Build  →', command=self.playlist_copy_to_build).pack(fill='x', pady=3)
        self.playlist_list = tk.Listbox(left, exportselection=False)
        self.playlist_list.pack(fill='both', expand=True)
        self.playlist_list.bind('<<ListboxSelect>>', self.selected_playlist)
        ttk.Label(middle, text='Playlist sequence / durations', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        addbar = ttk.Frame(middle)
        addbar.pack(fill='x', pady=5)
        self.preset_choice = ttk.Combobox(addbar, state='readonly', width=34)
        self.preset_choice.pack(side='left', fill='x', expand=True, padx=(0, 4))
        ttk.Button(addbar, text='Add', command=self.add_to_playlist).pack(side='left')
        self.playlist_steps = ttk.Treeview(middle, columns=('name', 'duration'), show='headings', height=15)
        self.playlist_steps.heading('name', text='Preset name (psn)')
        self.playlist_steps.heading('duration', text='Duration (100ms)')
        self.playlist_steps.column('name', width=250)
        self.playlist_steps.column('duration', width=110)
        self.playlist_steps.pack(fill='both', expand=True)
        self.playlist_steps.bind('<Double-1>', self.edit_playlist_duration)
        tools = ttk.Frame(middle)
        tools.pack(fill='x', pady=6)
        for label, handler in [('Up', lambda: self.move_step(-1)), ('Down', lambda: self.move_step(1)), ('Remove', self.remove_step)]:
            ttk.Button(tools, text=label, command=handler).pack(side='left', padx=3)
        ttk.Label(right, text='Library playlist JSON', font=('Segoe UI', 11, 'bold')).pack(anchor='w', pady=5)
        controls = ttk.Frame(right)
        controls.pack(fill='x', pady=(0, 5))
        ttk.Button(controls, text='Copy JSON', command=self.copy_playlist_json).pack(side='right', padx=3)
        ttk.Button(controls, text='Save playlist JSON', command=self.apply_playlist_json).pack(side='right', padx=3)
        self.playlist_raw = tk.Text(right, font=('Consolas', 10), wrap='none', undo=True)
        self.playlist_raw.pack(fill='both', expand=True)
        ttk.Label(right, text='psn is the source of truth. Generated numeric ps never belongs in the library.').pack(anchor='w', pady=5)
        self.playlist_key = None

    def playlist_copy_to_build(self):
        if self.playlist_key not in self.catalog.entries:
            messagebox.showinfo('Select playlist', 'Select a playlist in the left column first.')
            return
        self.workspace_copy_from_key(self.playlist_key)

    def apply_playlist_json(self):
        if self.playlist_key not in self.catalog.entries:
            return
        try:
            doc = json.loads(self.playlist_raw.get('1.0','end'))
            if not isinstance(doc, dict) or not isinstance(doc.get('n'),str) or not doc['n'].strip():
                raise ValueError('Playlist must have an n name')
            if not isinstance(doc.get('playlist'),dict):
                raise ValueError('Expected playlist object')
            doc['playlist'].pop('ps',None)
            self.catalog.entries[self.playlist_key]['data'] = doc
            self.catalog.save(self.playlist_key)
            self.refresh_playlists()
            self.refresh_workspace()
            self.refresh_viewer()
            self.status('Library playlist JSON saved')
        except Exception as exc:
            messagebox.showerror('Invalid playlist JSON',str(exc))

    def copy_playlist_json(self):
        try:
            doc = json.loads(self.playlist_raw.get('1.0','end'))
            if not isinstance(doc,dict):
                raise ValueError('Expected object')
        except Exception as exc:
            messagebox.showerror('Invalid playlist JSON',str(exc))
            return
        self.clipboard_clear()
        self.clipboard_append(json.dumps(doc, indent=2, ensure_ascii=False))

    def make_export_tab(self):
        tab = ttk.Frame(self.tabs, padding=12)
        self.tabs.add(tab, text='Build / export')
        self.export_tab = tab
        ttk.Label(tab, text='Select working copies for the device file', font=('Segoe UI', 13, 'bold')).pack(anchor='w')
        ttk.Label(tab, text='Only items in Device Build are exported. Numeric IDs and playlist ps arrays are generated here; the library is unchanged.').pack(anchor='w', pady=(2, 10))
        self.export_tree = ttk.Treeview(tab, columns=('selected', 'kind', 'category', 'name'), show='headings', selectmode='extended')
        for col, txt, width in [('selected', 'Export', 80), ('kind', 'Type', 95), ('category', 'Type', 100), ('name', 'Working name', 600)]:
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
        ttk.Label(top, text='Firmware effect catalog — read automatically from local PulSar C++ sources').pack(side='left')
        ttk.Button(top, text='Refresh source catalog', command=self.load_local_effects_if_available).pack(side='right', padx=3)
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
            presets = [rec for rec in self.catalog.entries.values() if rec['kind'] == 'preset']
            n_ok = sum(preset_compatible(rec['data'], available) for rec in presets)
            self.compat_label.set(f'{target}: {len(available)} enabled effects; {n_ok}/{len(presets)} library presets compatible')
        else:
            self.compat_label.set(f'{len(self.source_effects)} source effect names; choose a target and fetch /json/effects for accurate filtering.')
        self.update_effect_choices()
        self.update_palette_choices()

    def load_local_effects_if_available(self):
        path = find_effects_cpp()
        if path is None:
            self.source_effects = []
            self.effects_source_label.set('PulSar checkout not found. Run the designer inside the PulSar repository to load its effect catalog.')
            self.refresh_effect_catalog_view()
            return
        try:
            names = read_source_effects(path)
        except (OSError, UnicodeError, ValueError) as exc:
            self.source_effects = []
            self.effects_source_label.set(f'Could not read firmware effects: {exc}')
            self.refresh_effect_catalog_view()
            return
        self.source_effects = names
        self.effects_source_label.set(f'{len(names)} firmware effect names from {path.parent} (automatic; source is not device-specific)')
        self.refresh_effect_catalog_view()
        if not names:
            self.status('No effect names extracted from the firmware source. Check registration/config format.')

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
        self.refresh_workspace()
        self.status(f'Loaded {len(self.catalog.entries)} presets and playlists')

    def refresh_library_list(self):
        if not hasattr(self, 'library_tree'):
            return
        self.library_tree.delete(*self.library_tree.get_children())
        term = self.search_var.get().casefold()
        for key, rec in sorted(self.catalog.entries.items(), key=lambda pair: pair[1]['data'].get('n','').casefold()):
            if rec['kind'] != 'preset':
                continue
            if not self.is_visible_compatible(key):
                continue
            if term and term not in rec['data'].get('n', '').casefold() and term not in key.casefold():
                continue
            self.library_tree.insert('', 'end', iid=key, values=(rec['data'].get('n'),))

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
        self.update_effect_choices()
        self.update_palette_choices()
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

    def choose_storage_category(self):
        value = simpledialog.askstring('Library storage', 'Preset storage file (both visible in GUI):\n1d / 2d', initialvalue=self.category.get(), parent=self)
        if value is None:
            return None
        value = value.strip().lower()
        if value not in ('1d', '2d'):
            messagebox.showerror('Invalid library file', 'Choose 1d or 2d')
            return None
        self.category.set(value)
        return value

    def create_item(self, kind):
        category = 'shared' if kind == 'playlist' else self.choose_storage_category()
        if category is None:
            return
        name = simpledialog.askstring('New ' + kind, f'{kind.title()} name:', parent=self)
        if not name:
            return
        key = self.catalog.create(kind, category, name)
        self.refresh_library_list()
        self.refresh_playlists()
        self.refresh_export()
        if kind == 'preset':
            self.current_key = key
            self.tabs.select(0)
            self.library_tree.selection_set(key)
            self.library_tree.see(key)
            self.display_current()
        else:
            self.playlist_key = key
            self.tabs.select(self.playlist_tab)
            self.refresh_playlists()
            if key in self.playlist_keys:
                index = self.playlist_keys.index(key)
                self.playlist_list.selection_clear(0, 'end')
                self.playlist_list.selection_set(index)
                self.display_playlist()
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
        for key, name in sorted(self.catalog.names('playlist', None), key=lambda v: v[1].casefold()):
            self.playlist_keys.append(key)
            self.playlist_list.insert('end', name)
        preset_names = [name for key, name in sorted(self.catalog.names('preset', None), key=lambda v: v[1].casefold()) if self.is_visible_compatible(key)]
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
        self.playlist_raw.delete('1.0', 'end')
        self.playlist_raw.insert('1.0', json.dumps(self.catalog.entries[self.playlist_key]['data'], indent=2, ensure_ascii=False))

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
            self.display_playlist()
            self.status('Playlist template saved; numeric IDs are generated during export')

    def refresh_export(self):
        if not hasattr(self, 'export_tree'):
            return
        self.export_tree.delete(*self.export_tree.get_children())
        for key, rec in self.workspace.entries.items():
            selected = self.selected_export.get(key, True)
            self.export_tree.insert('', 'end', iid=key, values=('[x]' if selected else '[ ]', rec['kind'], rec['category'], rec['data'].get('n')))

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
        # Explicitly copy missing playlist prerequisites from the library, leaving originals untouched.
        present = {r['data']['n'].casefold() for r in self.workspace.entries.values() if r['kind']=='preset'}
        to_add = []
        for key in self.export_tree.get_children():
            rec = self.workspace.entries[key]
            if rec['kind']!='playlist' or not self.selected_export.get(key, True):
                continue
            for name in rec['data']['playlist'].get('psn', []):
                if str(name).casefold() in present:
                    continue
                matches = [r for r in self.catalog.entries.values() if r['kind']=='preset' and r['data']['n'].casefold()==str(name).casefold()]
                if len(matches)!=1:
                    messagebox.showerror('Missing or ambiguous template', f'Cannot resolve library preset {name!r}; found {len(matches)} matches.')
                    return
                to_add.append(matches[0])
                present.add(str(name).casefold())
        for rec in to_add:
            self.workspace.add(rec['data'], rec['category'], origin=rec['data']['n'])
        self.refresh_workspace()
        self.status(f'Added {len(to_add)} missing preset dependencies to Device Build')

    def compile(self):
        keys = [k for k in self.export_tree.get_children() if self.selected_export.get(k, True)]
        presets = [k for k in keys if self.workspace.entries[k]['kind']=='preset']
        playlists = [k for k in keys if self.workspace.entries[k]['kind']=='playlist']
        return compile_export(self.workspace, 'all', presets, playlists)

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
        write_json(dest / 'manifest.json', {'created_utc': dt.datetime.now(dt.timezone.utc).isoformat(), 'category': 'all', 'names_to_ids': {self.workspace.entries[k]['data']['n']: val for k, val in ids.items()}, 'entry_count': len(ids)})
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
        category = self.choose_storage_category()
        if category is None:
            return
        replace = messagebox.askyesno('Import matching names', f'Presets → lib/presets_{category}.json\nPlaylists → lib/playlists.json\n\nYes: replace same-name entries. No: append.')
        try:
            added, skipped = self.catalog.import_file(path, category, replace=replace)
        except Exception as exc:
            messagebox.showerror('Import failed', str(exc))
            return
        self.refresh_library()
        self.status(f'Imported {added} entries; skipped {skipped} entries')
        messagebox.showinfo('Import complete', f'Imported {added} entries. Skipped {skipped} unnamed/empty entries.\nStored in: {category}')

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
            if messagebox.askyesno('Import downloaded presets', f'Import {len(data)} downloaded entries into the preset library?\nDevice: {device["ip"]}\n\nChoose No to keep the downloaded backup without importing.'):
                category = self.choose_storage_category()
                if category is None:
                    return
                path = backup / 'presets.json'
                count, skipped = self.catalog.import_file(path, category, replace=False)
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
        presets = [(k, v) for k, v in self.catalog.entries.items() if v['kind']=='preset']
        playlists = [(k, v) for k, v in self.catalog.entries.items() if v['kind']=='playlist']
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
        self.tabs.select(self.playlist_tab)
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
            messagebox.showinfo('Live test', 'Select a library preset first.')
            return
        try:
            data = json.loads(self.raw_text.get('1.0','end'))
        except Exception as exc:
            messagebox.showerror('Invalid JSON',str(exc))
            return
        self.send_live_preset(data)

    def send_live_preset(self, data):
        if not isinstance(data,dict) or 'playlist' in data:
            messagebox.showinfo('Live test','Select an effect preset, not a playlist.')
            return
        dev = self.chosen_device()
        if dev is None:
            return
        payload={k:v for k,v in data.items() if k not in ('n','ql','ps','playlist')}
        payload['v']=True
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
        self.device_log(f'Sending live effect preview to {dev["ip"]} (no filesystem write)')
        self.worker(work,done)


if __name__ == '__main__':
    App().mainloop()
