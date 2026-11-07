"""PulSar Preset & Playlist Designer — standard-library Tkinter desktop GUI."""
from __future__ import annotations
import copy
import datetime as dt
import json
import re
import shutil
import tkinter as tk
from pathlib import Path
from tkinter import ttk, filedialog, messagebox, simpledialog

ROOT = Path(__file__).resolve().parent
LIBRARY = ROOT / 'library'
EXPORTS = ROOT / 'exports'
CATEGORIES = ('1d', '2d', 'audio_reactive')
KINDS = ('presets', 'playlists')
FIELDS = ('n', 'Pal', 'Pal2', 'FX', 'SX', 'IX', 'GP', 'GPP', 'GPB', 'DC', 'RA', 'BrightnessRGB')
NUMERIC = {'SX', 'IX', 'GP', 'GPP', 'GPB', 'DC', 'RA', 'BrightnessRGB'}
EFFECT_FIELDS = {'FX', 'SX', 'IX', 'GP', 'GPP', 'GPB', 'DC', 'RA'}
DEFAULT_PRESET = {'n': 'New Preset', 'Pal': 'Snowy 02', 'Effects': {'FX': 'Static Palette', 'IX': 127, 'SX': 127, 'GP': 1, 'DC': 0, 'RA': 1000}, 'BrightnessRGB': 100}
DEFAULT_PLAYLIST = {'n': 'New Playlist', 'playlist': {'psn': [], 'dur': [], 'repeat': 0, 'end': 0, 'r': 1}, 'on': True}


def dump(obj):
    return json.dumps(obj, indent=2, ensure_ascii=False)


def slug(s):
    s = re.sub(r'[^a-z0-9]+', '_', s.lower()).strip('_')
    return s[:60] or 'item'


def unique_key(existing, name):
    base = slug(name)
    result = base
    i = 2
    while result in existing:
        result = f'{base}_{i}'
        i += 1
    return result


def read_json(path):
    with path.open('r', encoding='utf-8-sig') as f:
        return json.load(f)


def write_json(path, obj):
    path.parent.mkdir(parents=True, exist_ok=True)
    temp = path.with_suffix(path.suffix + '.tmp')
    temp.write_text(dump(obj) + '\n', encoding='utf-8')
    temp.replace(path)


class Library:
    def __init__(self):
        self.data = {(kind, category): {} for kind in KINDS for category in CATEGORIES}
        for (kind, category) in self.data:
            path = LIBRARY / kind / f'{category}.json'
            if path.exists():
                loaded = read_json(path)
                for item in loaded.get('items', []):
                    self.data[kind, category][item['key']] = item

    def save(self, kind, category):
        items = list(self.data[kind, category].values())
        write_json(LIBRARY / kind / f'{category}.json', {'version': 1, 'kind': kind, 'category': category, 'items': items})

    def add(self, kind, category, data, source_id=None, source=None):
        items = self.data[kind, category]
        key = unique_key(items, data.get('n', 'unnamed'))
        items[key] = {'key': key, 'source_id': source_id, 'data': copy.deepcopy(data)}
        if source:
            items[key]['source'] = source
        self.save(kind, category)
        return key


def import_presets_file(lib, path, category):
    root = read_json(path)
    if not isinstance(root, dict):
        raise ValueError('Expected a JSON object keyed by preset IDs.')
    source = str(path)
    id_to_name = {str(id_): data.get('n', f'Preset {id_}') for id_, data in root.items() if isinstance(data, dict) and 'playlist' not in data and data.get('n')}
    counts = {'presets': 0, 'playlists': 0}
    notices = []
    for id_, data in root.items():
        if not isinstance(data, dict) or not data:
            continue
        kind = 'playlists' if 'playlist' in data else 'presets'
        entry = copy.deepcopy(data)
        entry.setdefault('n', f'{kind[:-1].title()} {id_}')
        if kind == 'playlists':
            pl = entry['playlist']
            if not isinstance(pl, dict):
                notices.append(f'{id_}: skipped invalid playlist object')
                continue
            psn = pl.get('psn')
            if not isinstance(psn, list) or not psn:
                if isinstance(pl.get('ps'), list):
                    names = [id_to_name.get(str(i)) for i in pl['ps']]
                    if all(names):
                        pl['psn'] = names
                    else:
                        notices.append(f'{id_}: playlist has unresolved numeric references; retained ps[]')
        lib.add(kind, category, entry, int(id_) if str(id_).isdigit() else None, source)
        counts[kind] += 1
    return counts, notices


def export_category(lib, category, output_base=EXPORTS):
    presets = list(lib.data['presets', category].values())
    playlists = list(lib.data['playlists', category].values())
    if not presets and not playlists:
        raise ValueError(f'No library entries in {category!r}.')
    all_items = presets + playlists
    assigned = {}
    used = set()
    issues = []
    # Preserve imported IDs when possible. Allocate other IDs from free slots.
    for item in all_items:
        id_ = item.get('source_id')
        if isinstance(id_, int) and id_ > 0 and id_ not in used:
            assigned[item['key'], 'playlists' if item in playlists else 'presets'] = id_
            used.add(id_)
    free = 1
    for kind, entries in (('presets', presets), ('playlists', playlists)):
        for item in entries:
            key = item['key'], kind
            if key not in assigned:
                while free in used:
                    free += 1
                assigned[key] = free
                used.add(free)
    names = {}
    for item in presets:
        name = item['data'].get('n', '')
        names.setdefault(name.casefold(), []).append(assigned[item['key'], 'presets'])
    result = {}
    for kind, entries in (('presets', presets), ('playlists', playlists)):
        for item in entries:
            data = copy.deepcopy(item['data'])
            id_ = assigned[item['key'], kind]
            if id_ > 250:
                issues.append(f'ID {id_} ({data.get("n", "")}) exceeds the legacy on-device 1–250 scan range; confirm firmware limits.')
            if kind == 'playlists':
                pl = data.get('playlist', {})
                psn = pl.get('psn')
                if isinstance(psn, list) and psn:
                    numeric = []
                    for name in psn:
                        ids = names.get(str(name).casefold(), [])
                        if len(ids) != 1:
                            issues.append(f'Playlist {data.get("n", id_)}: name {name!r} has {len(ids)} matching presets.')
                        else:
                            numeric.append(ids[0])
                    if len(numeric) == len(psn):
                        pl['ps'] = numeric
                elif isinstance(pl.get('ps'), list):
                    issues.append(f'Playlist {data.get("n", id_)}: only ps[] available; names must be resolved before safe export.')
                else:
                    issues.append(f'Playlist {data.get("n", id_)}: missing psn[] array.')
                if isinstance(pl.get('dur'), list) and isinstance(psn, list) and len(pl['dur']) != len(psn):
                    issues.append(f'Playlist {data.get("n", id_)}: dur[] and psn[] lengths differ.')
            result[str(id_)] = data
    # ID range warning is non-fatal; missing/ambiguous references are fatal.
    fatal = [v for v in issues if not v.startswith('ID ')]
    if fatal:
        raise ValueError('\n'.join(fatal[:25]))
    if '0' not in result:
        result['0'] = {}
    result = dict(sorted(result.items(), key=lambda pair: int(pair[0])))
    now = dt.datetime.now().strftime('%Y-%m-%d_%H%M%S_%f')
    out = Path(output_base) / now / category
    out.mkdir(parents=True, exist_ok=False)
    write_json(out / 'presets.json', result)
    write_json(out / 'manifest.json', {'generated_at': dt.datetime.now().isoformat(timespec='seconds'), 'category': category, 'preset_count': len(presets), 'playlist_count': len(playlists), 'warnings': issues, 'source_libraries': [str(LIBRARY / k / f'{category}.json') for k in KINDS]})
    return out / 'presets.json', issues


class Designer(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('PulSar — Preset & Playlist Designer')
        self.geometry('1460x850')
        self.minsize(1000, 650)
        self.lib = Library()
        self.category = tk.StringVar(value='1d')
        self.kind = tk.StringVar(value='presets')
        self.search = tk.StringVar(value='')
        self.current_key = None
        self.loading = False
        self.dirty = False
        self.field_dirty = False
        self.raw_dirty = False
        self.status = tk.StringVar(value='Ready. Import a presets.json or select an item.')
        self.fields = {}
        self.configure_gui()
        self.populate()
        self.protocol('WM_DELETE_WINDOW', self.on_close)

    def configure_gui(self):
        self.option_add('*Font', 'Segoe UI 10')
        toolbar = ttk.Frame(self, padding=8)
        toolbar.pack(fill='x')
        ttk.Label(toolbar, text='Library type:').pack(side='left')
        self.category_box = ttk.Combobox(toolbar, textvariable=self.category, values=CATEGORIES, state='readonly', width=18)
        self.category_box.pack(side='left', padx=5)
        self.category_box.bind('<<ComboboxSelected>>', self.change_collection)
        self.kind_box = ttk.Combobox(toolbar, textvariable=self.kind, values=KINDS, state='readonly', width=12)
        self.kind_box.pack(side='left', padx=5)
        self.kind_box.bind('<<ComboboxSelected>>', self.change_collection)
        ttk.Button(toolbar, text='Import presets.json', command=self.import_file).pack(side='left', padx=8)
        ttk.Button(toolbar, text='Export category', command=self.export).pack(side='left', padx=4)
        ttk.Button(toolbar, text='Open export folder', command=lambda: self.open_path(EXPORTS)).pack(side='left', padx=4)
        self.main_pane = ttk.Panedwindow(self, orient='horizontal')
        self.main_pane.pack(fill='both', expand=True, padx=8, pady=4)
        left = ttk.Frame(self.main_pane, padding=4)
        center = ttk.Frame(self.main_pane, padding=4)
        self.main_pane.add(left, weight=1)
        self.main_pane.add(center, weight=3)
        ttk.Label(left, text='Library', font=('Segoe UI', 13, 'bold')).pack(anchor='w')
        ttk.Entry(left, textvariable=self.search).pack(fill='x', pady=(5, 8))
        self.search.trace_add('write', lambda *_: self.populate())
        list_frame = ttk.Frame(left)
        list_frame.pack(fill='both', expand=True)
        self.listbox = tk.Listbox(list_frame, exportselection=False, font=('Consolas', 10), activestyle='dotbox')
        self.listbox.pack(side='left', fill='both', expand=True)
        sb = ttk.Scrollbar(list_frame, command=self.listbox.yview)
        sb.pack(side='right', fill='y')
        self.listbox.configure(yscrollcommand=sb.set)
        self.listbox.bind('<<ListboxSelect>>', self.select)
        actions = ttk.Frame(left)
        actions.pack(fill='x', pady=8)
        for label, cb in [('New', self.new_item), ('Duplicate', self.duplicate), ('Delete', self.delete_item)]:
            ttk.Button(actions, text=label, command=cb).pack(side='left', padx=2)
        self.count_label = ttk.Label(left, text='')
        self.count_label.pack(anchor='w')
        top = ttk.Frame(center)
        top.pack(fill='x')
        ttk.Label(top, text='Selected item', font=('Segoe UI', 13, 'bold')).pack(side='left')
        ttk.Button(top, text='Save item', command=self.save_item).pack(side='right', padx=4)
        ttk.Button(top, text='Copy JSON', command=self.copy_json).pack(side='right', padx=4)
        ttk.Button(top, text='Copy "ID": {...}', command=self.copy_with_id).pack(side='right', padx=4)
        tabs = ttk.Notebook(center)
        tabs.pack(fill='both', expand=True, pady=8)
        form_tab = ttk.Frame(tabs, padding=10)
        json_tab = ttk.Frame(tabs, padding=10)
        tabs.add(form_tab, text='Structured editor')
        tabs.add(json_tab, text='JSON editor (complete object)')
        self.tabs = tabs
        self.form_tab = form_tab
        form_canvas = tk.Canvas(form_tab, highlightthickness=0)
        form_sb = ttk.Scrollbar(form_tab, orient='vertical', command=form_canvas.yview)
        form_canvas.configure(yscrollcommand=form_sb.set)
        form_sb.pack(side='right', fill='y')
        form_canvas.pack(side='left', fill='both', expand=True)
        self.form_inner = ttk.Frame(form_canvas)
        form_canvas.create_window((0, 0), window=self.form_inner, anchor='nw', tags='inner')
        self.form_inner.bind('<Configure>', lambda e: form_canvas.configure(scrollregion=form_canvas.bbox('all')))
        form_canvas.bind('<Configure>', lambda e: form_canvas.itemconfigure('inner', width=e.width))
        self.draw_form()
        ttk.Label(json_tab, text='Editable JSON object only. Paste the value of one presets.json entry, without its outer numeric ID.').pack(anchor='w', pady=(0, 6))
        json_frame = ttk.Frame(json_tab)
        json_frame.pack(fill='both', expand=True)
        self.raw = tk.Text(json_frame, wrap='none', undo=True, font=('Consolas', 11), tabs=('2c',))
        self.raw.pack(side='left', fill='both', expand=True)
        yscroll = ttk.Scrollbar(json_frame, orient='vertical', command=self.raw.yview)
        yscroll.pack(side='right', fill='y')
        self.raw.configure(yscrollcommand=yscroll.set)
        self.raw.bind('<<Modified>>', self.raw_modified)
        rawbuttons = ttk.Frame(json_tab)
        rawbuttons.pack(fill='x', pady=8)
        ttk.Button(rawbuttons, text='Apply JSON to fields', command=self.apply_json_to_fields).pack(side='left', padx=4)
        ttk.Button(rawbuttons, text='Format JSON', command=self.format_json).pack(side='left', padx=4)
        ttk.Button(rawbuttons, text='Copy JSON', command=self.copy_json).pack(side='left', padx=4)
        ttk.Label(self, textvariable=self.status, anchor='w', relief='sunken', padding=5).pack(fill='x', side='bottom')

    def draw_form(self):
        for child in self.form_inner.winfo_children():
            child.destroy()
        self.fields = {}
        ttk.Label(self.form_inner, text='Quick-edit parameters', font=('Segoe UI', 12, 'bold')).grid(row=0, column=0, columnspan=3, sticky='w', pady=(0, 8))
        for i, key in enumerate(FIELDS):
            ttk.Label(self.form_inner, text=key).grid(row=i + 1, column=0, sticky='w', padx=6, pady=4)
            variable = tk.StringVar()
            variable.trace_add('write', lambda *_: self.mark_field_dirty())
            entry = ttk.Entry(self.form_inner, textvariable=variable, width=48)
            entry.grid(row=i + 1, column=1, sticky='ew', pady=4)
            self.fields[key] = variable
        ttk.Button(self.form_inner, text='Apply fields to JSON', command=self.fields_to_json).grid(row=len(FIELDS) + 1, column=1, sticky='e', pady=12)
        ttk.Label(self.form_inner, text='Blank fields remove that property. Other keys (custom options, segment colours, playlist metadata) are retained.').grid(row=len(FIELDS) + 2, column=0, columnspan=3, sticky='w')
        ttk.Separator(self.form_inner).grid(row=len(FIELDS) + 3, column=0, columnspan=3, sticky='ew', pady=14)
        self.playlist_frame = ttk.LabelFrame(self.form_inner, text='Playlist sequence (names, durations, transitions)', padding=8)
        self.playlist_frame.grid(row=len(FIELDS) + 4, column=0, columnspan=3, sticky='nsew')
        self.playlist_tree = ttk.Treeview(self.playlist_frame, columns=('name', 'dur', 'tr'), show='headings', height=8)
        for col, title, width in [('name', 'Preset name (psn)', 300), ('dur', 'Duration', 75), ('tr', 'Transition ms', 110)]:
            self.playlist_tree.heading(col, text=title)
            self.playlist_tree.column(col, width=width, stretch=col == 'name')
        self.playlist_tree.pack(fill='x', expand=True)
        controls = ttk.Frame(self.playlist_frame)
        controls.pack(fill='x', pady=6)
        for name, callback in [('Add', self.playlist_add), ('Edit', self.playlist_edit), ('Remove', self.playlist_remove), ('Up', lambda: self.playlist_move(-1)), ('Down', lambda: self.playlist_move(1))]:
            ttk.Button(controls, text=name, command=callback).pack(side='left', padx=3)
        ttk.Label(self.playlist_frame, text='Playlist edits update psn[] and dur[]; numeric ps[] is regenerated during export.').pack(anchor='w')
        self.extra_frame = ttk.LabelFrame(self.form_inner, text='All properties (including custom controls and nested JSON)', padding=8)
        self.extra_frame.grid(row=len(FIELDS) + 5, column=0, columnspan=3, sticky='nsew', pady=10)
        self.prop_tree = ttk.Treeview(self.extra_frame, columns=('value',), show='tree headings', height=13)
        self.prop_tree.heading('#0', text='Property path')
        self.prop_tree.heading('value', text='Value')
        self.prop_tree.column('#0', width=280)
        self.prop_tree.column('value', width=390)
        self.prop_tree.pack(fill='both', expand=True)
        self.prop_paths = {}
        self.prop_tree.bind('<Double-1>', lambda event: self.prop_edit())
        bar = ttk.Frame(self.extra_frame)
        bar.pack(fill='x', pady=4)
        for title, callback in [('Edit selected', self.prop_edit), ('Add property', self.prop_add), ('Delete selected', self.prop_delete)]:
            ttk.Button(bar, text=title, command=callback).pack(side='left', padx=3)
        ttk.Label(self.extra_frame, text='Edit values as JSON literals: 12, true, [1,2], or \"string\". Empty field = JSON null.').pack(anchor='w')
        self.form_inner.columnconfigure(1, weight=1)

    def collection_items(self):
        return self.lib.data[self.kind.get(), self.category.get()]

    def populate(self, select_key=None):
        items = self.collection_items()
        needle = self.search.get().casefold().strip()
        self.visible = [key for key, item in items.items() if needle in (item['data'].get('n', '') + ' ' + key + ' ' + str(item.get('source_id', ''))).casefold()]
        self.visible.sort(key=lambda key: (items[key].get('source_id') is None, items[key].get('source_id') or 0, items[key]['data'].get('n', '').casefold()))
        self.listbox.delete(0, 'end')
        for key in self.visible:
            item = items[key]
            self.listbox.insert('end', f'{str(item.get("source_id") or "—"):>4}  {item["data"].get("n", key)}')
        self.count_label.configure(text=f'{len(self.visible)} shown / {len(items)} entries')
        target = select_key if select_key is not None else self.current_key
        if target in self.visible:
            pos = self.visible.index(target)
            self.listbox.selection_set(pos)
            self.listbox.see(pos)

    def change_collection(self, event=None):
        if not self.confirm_discard():
            return
        self.current_key = None
        self.show_data(None)
        self.populate()

    def select(self, event=None):
        selected = self.listbox.curselection()
        if not selected:
            return
        key = self.visible[selected[0]]
        if key == self.current_key:
            return
        if not self.confirm_discard():
            self.populate(self.current_key)
            return
        self.current_key = key
        self.show_data(self.collection_items()[key]['data'])
        self.status.set(f'Editing {self.kind.get()}/{self.category.get()}/{key}')

    def show_data(self, data):
        self.loading = True
        data = data or {}
        self.raw.delete('1.0', 'end')
        self.raw.insert('1.0', dump(data))
        self.raw.edit_modified(False)
        self.update_fields(data)
        self.loading = False
        self.dirty = False
        self.field_dirty = False
        self.raw_dirty = False
        self.title('PulSar — Preset & Playlist Designer')

    def update_fields(self, data):
        effects = data.get('Effects', {})
        for key, variable in self.fields.items():
            value = effects.get(key, '') if key in EFFECT_FIELDS else data.get(key, '')
            variable.set(str(value) if value != '' else '')
        self.render_properties(data)
        for child in self.playlist_tree.get_children():
            self.playlist_tree.delete(child)
        self.playlist_frame.grid_remove() if self.kind.get() != 'playlists' else self.playlist_frame.grid()
        if self.kind.get() == 'playlists':
            pl = data.get('playlist', {})
            names = pl.get('psn', [])
            durations = pl.get('dur', [])
            transitions = pl.get('transition', [])
            for i, name in enumerate(names):
                self.playlist_tree.insert('', 'end', values=(name, durations[i] if i < len(durations) else 100, transitions[i] if isinstance(transitions, list) and i < len(transitions) else ''))

    def render_properties(self, data):
        for item in self.prop_tree.get_children():
            self.prop_tree.delete(item)
        self.prop_paths = {}
        def visit(parent, value, path):
            if not isinstance(value, (dict, list)):
                return
            parts = value.items() if isinstance(value, dict) else enumerate(value)
            for key, child in parts:
                child_path = path + (key,)
                display = str(key)
                val = '{...}' if isinstance(child, dict) else '[...]' if isinstance(child, list) else json.dumps(child, ensure_ascii=False)
                iid = self.prop_tree.insert(parent, 'end', text=display, values=(val,), open=bool(not parent))
                self.prop_paths[iid] = child_path
                visit(iid, child, child_path)
        visit('', data, ())

    def prop_selection(self):
        selected = self.prop_tree.selection()
        return self.prop_paths.get(selected[0]) if selected else None

    def prop_parent(self, data, path):
        parent = data
        for key in path[:-1]:
            parent = parent[key]
        return parent

    def prop_edit(self):
        path = self.prop_selection()
        if not path:
            return
        try:
            data = json.loads(self.raw.get('1.0', 'end'))
            parent = self.prop_parent(data, path)
            old = parent[path[-1]]
            before = dump(old) if isinstance(old, (dict, list)) else json.dumps(old, ensure_ascii=False)
            value = simpledialog.askstring('Edit JSON property', '.'.join(map(str, path)) + ' (JSON value)', initialvalue=before, parent=self)
            if value is None:
                return
            parent[path[-1]] = json.loads(value)
            self.set_editor(data)
        except Exception as e:
            messagebox.showerror('Invalid JSON property', str(e))

    def prop_add(self):
        path = self.prop_selection() or ()
        try:
            data = json.loads(self.raw.get('1.0', 'end'))
            parent = data
            for key in path:
                parent = parent[key]
            if not isinstance(parent, dict):
                raise ValueError('Select a JSON object such as Effects, or deselect to add at the root.')
            key = simpledialog.askstring('Add property', 'JSON key:', parent=self)
            if not key:
                return
            if key in parent:
                raise ValueError(f'Property {key!r} already exists.')
            value = simpledialog.askstring('Add property', 'JSON value (e.g. 127, true, "text", [1,2]):', initialvalue='0', parent=self)
            if value is None:
                return
            parent[key] = json.loads(value)
            self.set_editor(data)
        except Exception as e:
            messagebox.showerror('Cannot add property', str(e))

    def prop_delete(self):
        path = self.prop_selection()
        if not path:
            return
        try:
            data = json.loads(self.raw.get('1.0', 'end'))
            parent = self.prop_parent(data, path)
            if isinstance(parent, list):
                parent.pop(path[-1])
            else:
                del parent[path[-1]]
            self.set_editor(data)
        except Exception as e:
            messagebox.showerror('Cannot delete property', str(e))

    def raw_modified(self, event=None):
        if self.raw.edit_modified():
            if not self.loading:
                self.raw_dirty = True
                self.mark_dirty()
            self.raw.edit_modified(False)

    def mark_field_dirty(self):
        if not self.loading:
            self.field_dirty = True
            self.mark_dirty()

    def mark_dirty(self):
        self.dirty = True
        self.title('PulSar — Preset & Playlist Designer *')

    def read_editor(self):
        data = json.loads(self.raw.get('1.0', 'end').strip())
        if not isinstance(data, dict):
            raise ValueError('Selected item must be a JSON object.')
        if not isinstance(data.get('n'), str) or not data['n'].strip():
            raise ValueError('Item requires a nonempty n (name) property.')
        if self.kind.get() == 'playlists' and not isinstance(data.get('playlist'), dict):
            raise ValueError('Playlist entries require a playlist object.')
        return data

    def set_editor(self, data):
        self.loading = True
        self.raw.delete('1.0', 'end')
        self.raw.insert('1.0', dump(data))
        self.raw.edit_modified(False)
        self.update_fields(data)
        self.loading = False
        self.mark_dirty()

    def apply_json_to_fields(self):
        try:
            data = self.read_editor()
        except Exception as e:
            messagebox.showerror('Invalid JSON', str(e))
            return
        self.loading = True
        self.update_fields(data)
        self.loading = False
        self.field_dirty = False
        self.status.set('JSON parsed and quick-edit fields refreshed.')

    def format_json(self):
        try:
            self.set_editor(self.read_editor())
        except Exception as e:
            messagebox.showerror('Invalid JSON', str(e))

    def fields_to_json(self):
        try:
            data = json.loads(self.raw.get('1.0', 'end').strip())
            if not isinstance(data, dict):
                raise ValueError('JSON root must be an object.')
            for key, variable in self.fields.items():
                raw = variable.get().strip()
                target = data.setdefault('Effects', {}) if key in EFFECT_FIELDS else data
                if not raw:
                    target.pop(key, None)
                else:
                    if key in NUMERIC:
                        target[key] = int(raw)
                    else:
                        target[key] = raw
            self.set_editor(data)
            self.field_dirty = False
            self.status.set('Structured parameters applied to JSON. Save item to persist.')
        except Exception as e:
            messagebox.showerror('Cannot apply fields', str(e))

    def save_item(self):
        if self.current_key is None:
            return False
        try:
            if self.field_dirty:
                if self.raw_dirty:
                    raise ValueError('Both JSON and structured fields changed. Choose Apply fields to JSON or Apply JSON to fields to decide which values win.')
                self.fields_to_json()
            data = self.read_editor()
            self.collection_items()[self.current_key]['data'] = data
            self.lib.save(self.kind.get(), self.category.get())
            self.show_data(data)
            self.populate(self.current_key)
            self.status.set('Saved to library.')
            return True
        except Exception as e:
            messagebox.showerror('Save failed', str(e))
            return False

    def confirm_discard(self):
        if not self.dirty:
            return True
        choice = messagebox.askyesnocancel('Unsaved changes', 'Save changes to the current library item?')
        if choice is None:
            return False
        return self.save_item() if choice else True

    def new_item(self):
        if not self.confirm_discard():
            return
        kind, category = self.kind.get(), self.category.get()
        data = copy.deepcopy(DEFAULT_PLAYLIST if kind == 'playlists' else DEFAULT_PRESET)
        key = self.lib.add(kind, category, data)
        self.current_key = key
        self.search.set('')
        self.populate(key)
        self.show_data(data)

    def duplicate(self):
        if self.current_key is None or not self.confirm_discard():
            return
        data = copy.deepcopy(self.collection_items()[self.current_key]['data'])
        data['n'] += ' Copy'
        key = self.lib.add(self.kind.get(), self.category.get(), data)
        self.current_key = key
        self.search.set('')
        self.populate(key)
        self.show_data(data)

    def delete_item(self):
        if self.current_key is None:
            return
        item = self.collection_items()[self.current_key]
        if not messagebox.askyesno('Delete library item', f'Delete {item["data"].get("n", self.current_key)!r} from the library?'):
            return
        del self.collection_items()[self.current_key]
        self.lib.save(self.kind.get(), self.category.get())
        self.current_key = None
        self.show_data(None)
        self.populate()

    def copy_json(self):
        self.clipboard_clear()
        self.clipboard_append(self.raw.get('1.0', 'end').strip())
        self.status.set('JSON object copied to clipboard.')

    def copy_with_id(self):
        if self.current_key is None:
            return
        item = self.collection_items()[self.current_key]
        id_ = item.get('source_id')
        if id_ is None:
            id_ = simpledialog.askinteger('Preset ID', 'Numeric ID to put before the JSON object:', parent=self, minvalue=1)
            if id_ is None:
                return
        try:
            data = self.read_editor()
            snippet = json.dumps(str(id_)) + ': ' + dump(data)
            self.clipboard_clear()
            self.clipboard_append(snippet)
            self.status.set(f'Copied "{id_}": {{...}} snippet.')
        except Exception as e:
            messagebox.showerror('Invalid JSON', str(e))

    def playlist_data(self):
        data = self.read_editor()
        if not isinstance(data.get('playlist'), dict):
            raise ValueError('Not a playlist object.')
        return data

    def playlist_add(self):
        if self.kind.get() != 'playlists':
            return
        try:
            data = self.playlist_data()
            choices = sorted({x['data'].get('n', '') for x in self.lib.data['presets', self.category.get()].values()})
            if not choices:
                raise ValueError('Create or import preset entries before adding playlist names.')
            win = tk.Toplevel(self)
            win.title('Add playlist entry')
            win.transient(self)
            win.grab_set()
            ttk.Label(win, text='Preset name').grid(row=0, column=0, padx=10, pady=8)
            var = tk.StringVar(value=choices[0])
            cb = ttk.Combobox(win, values=choices, textvariable=var, width=50)
            cb.grid(row=0, column=1, padx=10, pady=8)
            ttk.Label(win, text='Duration').grid(row=1, column=0)
            dur = tk.StringVar(value='100')
            ttk.Entry(win, textvariable=dur).grid(row=1, column=1, sticky='ew', padx=10, pady=8)
            def add():
                try:
                    if var.get() not in choices:
                        raise ValueError('Choose an existing preset name.')
                    duration = int(dur.get())
                    if duration < 1:
                        raise ValueError('Duration must be positive.')
                    pl = data['playlist']
                    pl.setdefault('psn', []).append(var.get())
                    pl.setdefault('dur', []).append(duration)
                    pl.pop('ps', None)
                    self.set_editor(data)
                    win.destroy()
                except Exception as e:
                    messagebox.showerror('Invalid entry', str(e), parent=win)
            ttk.Button(win, text='Add entry', command=add).grid(row=2, column=1, sticky='e', padx=10, pady=10)
        except Exception as e:
            messagebox.showerror('Cannot add entry', str(e))

    def playlist_edit(self):
        sel = self.playlist_tree.selection()
        if not sel:
            return
        i = self.playlist_tree.index(sel[0])
        try:
            data = self.playlist_data()
            pl = data['playlist']
            name = simpledialog.askstring('Preset name', 'Name (must match library):', initialvalue=pl['psn'][i], parent=self)
            if name is None:
                return
            dur = simpledialog.askinteger('Duration', 'Duration in playlist units:', initialvalue=(pl.get('dur', [])[i] if i < len(pl.get('dur', [])) else 100), minvalue=1, parent=self)
            if dur is None:
                return
            pl['psn'][i] = name
            while len(pl.setdefault('dur', [])) < len(pl['psn']):
                pl['dur'].append(100)
            pl['dur'][i] = dur
            pl.pop('ps', None)
            self.set_editor(data)
        except Exception as e:
            messagebox.showerror('Cannot edit entry', str(e))

    def playlist_remove(self):
        sel = self.playlist_tree.selection()
        if not sel:
            return
        i = self.playlist_tree.index(sel[0])
        try:
            data = self.playlist_data()
            pl = data['playlist']
            pl['psn'].pop(i)
            if isinstance(pl.get('dur'), list) and i < len(pl['dur']):
                pl['dur'].pop(i)
            if isinstance(pl.get('transition'), list) and i < len(pl['transition']):
                pl['transition'].pop(i)
            pl.pop('ps', None)
            self.set_editor(data)
        except Exception as e:
            messagebox.showerror('Cannot remove entry', str(e))

    def playlist_move(self, direction):
        sel = self.playlist_tree.selection()
        if not sel:
            return
        i = self.playlist_tree.index(sel[0])
        j = i + direction
        try:
            data = self.playlist_data()
            pl = data['playlist']
            if j < 0 or j >= len(pl['psn']):
                return
            for key in ('psn', 'dur', 'transition'):
                arr = pl.get(key)
                if isinstance(arr, list) and len(arr) == len(pl['psn']):
                    arr[i], arr[j] = arr[j], arr[i]
            pl.pop('ps', None)
            self.set_editor(data)
            iid = self.playlist_tree.get_children()[j]
            self.playlist_tree.selection_set(iid)
        except Exception as e:
            messagebox.showerror('Cannot reorder', str(e))

    def import_file(self):
        if not self.confirm_discard():
            return
        path = filedialog.askopenfilename(title='Import existing PulSar presets.json', filetypes=[('JSON files', '*.json'), ('All files', '*.*')])
        if not path:
            return
        try:
            counts, notices = import_presets_file(self.lib, Path(path), self.category.get())
            self.current_key = None
            self.show_data(None)
            self.populate()
            info = f'Imported {counts["presets"]} presets and {counts["playlists"]} playlists into {self.category.get()}.'
            if notices:
                info += '\n\nNotes:\n' + '\n'.join(notices[:12])
            messagebox.showinfo('Import completed', info)
            self.status.set(info.splitlines()[0])
        except Exception as e:
            messagebox.showerror('Import failed', str(e))

    def export(self):
        if not self.confirm_discard():
            return
        try:
            filename, warnings = export_category(self.lib, self.category.get())
            details = '\n'.join(warnings[:12]) if warnings else 'All named playlist references resolved successfully.'
            messagebox.showinfo('Export completed', f'Generated:\n{filename}\n\n{details}')
            self.status.set(f'Exported {filename}')
        except Exception as e:
            messagebox.showerror('Export blocked: fix validation errors', str(e))

    def open_path(self, path):
        path.mkdir(exist_ok=True, parents=True)
        import os
        import subprocess
        import sys
        if sys.platform == 'win32':
            os.startfile(str(path))
        elif sys.platform == 'darwin':
            subprocess.Popen(['open', str(path)])
        else:
            subprocess.Popen(['xdg-open', str(path)])

    def on_close(self):
        if self.confirm_discard():
            self.destroy()


if __name__ == '__main__':
    Designer().mainloop()
