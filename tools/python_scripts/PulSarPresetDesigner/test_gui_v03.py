from main import App
app=App()
app.update()
assert len(app.tabs.tabs()) >= 8
for tab in app.tabs.tabs():
    app.tabs.select(tab)
    app.update()
assert len(app.library_tree.get_children()) == len(app.catalog.entries), 'Unified library must show all entries'
assert 'category' in app.library_tree['columns']
assert len(app.playlist_keys) == len([r for r in app.catalog.entries.values() if r['kind'] == 'playlist'])
assert len(app.export_tree.get_children()) == len(app.catalog.entries)
assert len(app.palette_choice['values']) > 0
app.destroy()
print('v0.4 GUI initialization, unified library across storage files: OK')
