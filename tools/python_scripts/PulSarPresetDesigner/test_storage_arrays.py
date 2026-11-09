"""Exactly three library files; device preset IDs are generated only at export."""
import json
import tempfile
from pathlib import Path
import main

with tempfile.TemporaryDirectory() as temp:
    base = Path(temp)
    main.LIBRARY = base / 'lib'
    main.LEGACY_LIBRARY = base / 'library'
    main.LIBRARY.mkdir()
    for name, contents in {
        'presets_1d.json': [{'n': 'Alpha', 'Effects': {'FX': 'Static Palette'}}, {'n': 'Beta', 'Effects': {'FX': 'Firefly'}}],
        'presets_2d.json': [{'n': 'Icicle', 'Effects': {'FX': 'SnowGlow'}}],
        'playlists_1d.json': [{'n': 'Tree Show', 'playlist': {'psn': ['Alpha', 'Beta'], 'ps': [88, 99], 'dur': [100, 200]}}],
        'playlists_2d.json': [{'n': 'Icicle Show', 'playlist': {'psn': ['Icicle'], 'ps': [122], 'dur': [100]}}],
        'playlists_audio_reactive.json': []
    }.items():
        (main.LIBRARY / name).write_text(json.dumps(contents), encoding='utf-8')
    c = main.Catalog()
    assert len(c.entries) == 5
    assert len(json.loads((main.LIBRARY / 'playlists.json').read_text())) == 2
    assert all('ps' not in item['playlist'] for item in json.loads((main.LIBRARY / 'playlists.json').read_text()))
    assert all('source_id' not in record for record in c.entries.values())
    assert not (main.LIBRARY / '_imported_ids.json').exists()
    assert {f.name for f in main.LIBRARY.glob('*.json')} >= {'presets_1d.json', 'presets_2d.json', 'playlists.json'}
    key = c.create('playlist', '1d', 'All Lights', {'n': 'All Lights', 'playlist': {'psn': ['Alpha', 'Icicle'], 'ps': [300, 1000], 'dur': [50, 50]}})
    assert c.entries[key]['category'] == 'shared'
    assert 'ps' not in c.entries[key]['data']['playlist']
    ids = {item['data']['n']: name for name, item in c.entries.items()}
    result, assigned = main.compile_export(c, 'all', [ids['Alpha'], ids['Icicle']], [key])
    assert result[str(assigned[key])]['playlist']['ps'] == [assigned[ids['Alpha']], assigned[ids['Icicle']]]
    assert 'ps' not in c.entries[key]['data']['playlist']
    c.entries[ids['Alpha']]['data']['Effects']['IX'] = 30
    c.save(ids['Alpha'])
    assert json.loads((main.LIBRARY / 'presets_1d.json').read_text())[0]['Effects']['IX'] == 30
    c.delete(ids['Beta'])
    assert len(json.loads((main.LIBRARY / 'presets_1d.json').read_text())) == 1
    c.refresh()
    assert len(c.entries) == 5
    print('Three authoritative library arrays, migration, edit/delete, cross-type playlists, ID-free storage and export: OK')
