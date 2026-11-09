"""Export resolves names and never inserts numeric IDs into the library."""
import tempfile
from pathlib import Path
import main

with tempfile.TemporaryDirectory() as folder:
    main.LIBRARY = Path(folder) / 'lib'
    main.LEGACY_LIBRARY = Path(folder) / 'old_library'
    c = main.Catalog()
    p = c.create('preset', '1d', 'Static Snow', main.new_entry('Static Snow', 'preset'))
    q = c.create('playlist', 'shared', 'Evening Show', {'n': 'Evening Show', 'playlist': {'psn': ['Static Snow'], 'dur': [100], 'repeat': 0}, 'on': True})
    out, assigned = main.compile_export(c, 'all', [p], [q])
    assert out[str(assigned[q])]['playlist']['ps'] == [assigned[p]]
    assert 'ps' not in c.entries[q]['data']['playlist']
    c.entries[q]['data']['playlist']['psn'] = ['No such preset']
    try:
        main.compile_export(c, 'all', [p], [q])
        raise AssertionError('Unresolved name should fail')
    except ValueError as exc:
        assert 'unresolved' in str(exc)
    print('Export ID generation, psn resolution and missing-name validation: OK')
