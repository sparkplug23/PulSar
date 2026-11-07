import json
import tempfile
from pathlib import Path
from main import Catalog, compile_export, http_get_presets, new_entry


def test_export():
    c = Catalog()
    good_preset = [key for key, rec in c.entries.items() if rec['kind'] == 'preset' and rec['category'] == '1d' and rec['data']['n'] == '3Jan26a Static Snowy02'][0]
    playlist = [key for key, rec in c.entries.items() if rec['kind'] == 'playlist' and rec['category'] == '1d' and rec['data']['n'] == 'N1 Init Test'][0]
    # Explicitly test psn compilation with a playlist that references a selected preset.
    custom = c.create('playlist', '1d', 'Temporary Unit Test Playlist', {'n': 'Temporary Unit Test Playlist', 'playlist': {'psn': ['3Jan26a Static Snowy02'], 'dur': [100], 'repeat': 0}, 'on': True})
    try:
        compiled, index = compile_export(c, '1d', [good_preset], [custom])
        assert compiled[str(index[custom])]['playlist']['ps'] == [index[good_preset]]
        assert compiled[str(index[custom])]['playlist']['psn'] == ['3Jan26a Static Snowy02']
        c.entries[custom]['data']['playlist']['psn'] = ['does not exist']
        try:
            compile_export(c, '1d', [good_preset], [custom])
            raise AssertionError('missing name should fail')
        except ValueError as exc:
            assert 'unresolved' in str(exc)
    finally:
        c.delete(custom)
    print('Export generation, name resolution and missing-name protection: OK')


if __name__ == '__main__':
    test_export()
