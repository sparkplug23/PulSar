"""Run with: python test_device_build.py (GUI check needs a desktop / display)."""
import copy
import json
import tempfile
from pathlib import Path
from main import BuildWorkspace, compile_export, Catalog

with tempfile.TemporaryDirectory() as temp:
    ws = BuildWorkspace(Path(temp) / 'working_set.json')
    source_preset = {'n':'Example Effect', 'Pal':'Snowy 02', 'Effects':{'FX':'Static Palette', 'GPB':20}}
    first = ws.add(source_preset, '1d', origin='Example Effect')
    ws.entries[first]['data']['Effects']['GPB'] = 15
    ws.save()
    assert source_preset['Effects']['GPB'] == 20, 'Working edits leaked into source template'
    second = ws.add({'n':'Test Playlist','playlist':{'psn':['Example Effect'], 'dur':[100], 'ps':[201]}},'shared')
    assert 'ps' not in ws.entries[second]['data']['playlist'], 'Device IDs must not persist in working playlist'
    output, ids = compile_export(ws, 'all', [first], [second])
    assert output[str(ids[second])]['playlist']['ps'] == [ids[first]], 'Names must resolve to exported numeric IDs'
    assert 'ps' not in ws.entries[second]['data']['playlist']
    ws_reloaded = BuildWorkspace(Path(temp) / 'working_set.json')
    assert len(ws_reloaded.entries) == 2, 'Workspace should survive restart'
    assert source_preset['Effects']['GPB'] == 20
    ws_reloaded.delete(first)
    assert len(ws_reloaded.entries) == 1
print('PASS: source isolation, working edits, psn resolution, exported IDs, persistence and deletion')
