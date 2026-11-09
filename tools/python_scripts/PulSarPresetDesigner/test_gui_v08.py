"""Run with a graphical desktop or xvfb-run -a python test_gui_v08.py."""
from main import App
app = App()
try:
    app.update()
    tabs=[app.tabs.tab(t,'text') for t in app.tabs.tabs()]
    assert tabs[:3]==['Preset library','Device Build','Playlist editor'],tabs
    preset_count=sum(1 for r in app.catalog.entries.values() if r['kind']=='preset')
    assert len(app.library_tree.get_children())==preset_count
    assert app.library_tree['columns']==('name',)
    assert len(app.export_tree.get_children())==len(app.workspace.entries)
    assert len(app.playlist_keys)==sum(1 for r in app.catalog.entries.values() if r['kind']=='playlist')
    first = next(k for k,r in app.catalog.entries.items() if r['kind']=='preset')
    original = dict(app.catalog.entries[first]['data'])
    app.workspace_copy_from_key(first)
    assert len(app.workspace.entries)>=1
    assert app.workspace.entries[app.draft_key]['data']==original
    assert app.catalog.entries[first]['data']==original
    app.tabs.select(app.workspace_tab)
    app.update()
    print('PASS: redesigned GUI, three panes, playlist separation and working template copy')
finally:
    app.destroy()
