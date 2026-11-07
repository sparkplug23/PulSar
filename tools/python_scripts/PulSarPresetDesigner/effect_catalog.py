"""PulSar effect catalog: scan local firmware sources or query a running device.

The device's /json/effects is the authoritative set of enabled effects.
The source parser is best-effort and intentionally does not pretend that
preprocessor-conditional registrations are active for any given build.
"""
from __future__ import annotations
import json
import re
from pathlib import Path
from urllib.request import Request, urlopen


def normalize_effect(value):
    return ' '.join(str(value).strip().casefold().split())


def device_effects(document):
    """Decode /json/effects: PulSar returns {numeric_id: effect_name}."""
    if isinstance(document, dict):
        iterator = document.items()
    elif isinstance(document, list):
        iterator = enumerate(document)
    else:
        raise ValueError('Expected object or list from /json/effects')
    result = {}
    for id_, value in iterator:
        if isinstance(value, dict):
            value = value.get('name', value.get('n', ''))
        if not isinstance(value, str):
            continue
        name = value.strip()
        if not name or 'RSVD' in name.upper():
            continue
        try:
            num_id = int(id_)
        except (TypeError, ValueError):
            continue
        result[num_id] = name
    if not result:
        raise ValueError('No named effects returned by device')
    return result


def fetch_device_effects(host, port=80, timeout=8):
    url = f'http://{host}:{int(port)}/json/effects'
    with urlopen(Request(url, headers={'Cache-Control':'no-cache'}), timeout=timeout) as response:
        raw = response.read(2_000_001)
    if len(raw) > 2_000_000:
        raise ValueError('Effect catalog exceeds 2 MB')
    return device_effects(json.loads(raw.decode('utf-8-sig')))


def read_source_effects(path):
    """Extract all discoverable names from Effects.cpp and sibling source files.

    Parse quoted registration/config strings (not function identifiers). Supports
    both literal names and C/C++ macro definitions. Results are *not* a device
    capability list: #if-disabled effects can be present in the source.
    """
    path = Path(path)
    if not path.is_file():
        raise FileNotFoundError(path)
    text = path.read_text(encoding='utf-8', errors='replace')
    # Definitions may reside in sibling headers / effects files.
    sources = [path]
    if path.name == 'Effects.cpp':
        sources += sorted(p for p in path.parent.glob('Effects*.cpp') if p != path)[:30]
        sources += sorted(path.parent.glob('Effects*.h'))[:30]
    all_text = '\n'.join(p.read_text(encoding='utf-8', errors='replace') for p in sources)
    # Select likely effect names from config definitions ending with a semicolon
    # ('Effect name@Speed,...;...') and addEffect registrations.
    result = set()
    def add(candidate):
        candidate = candidate.strip().split('@', 1)[0].split(';', 1)[0].strip()
        if not candidate or len(candidate) > 100 or 'RSVD' in candidate.upper():
            return
        if not re.search(r'[A-Za-z]', candidate):
            return
        if re.search(r'[{}<>\\]|\b(?:PROGMEM|PSTR|return|const|uint|char|define)\b', candidate):
            return
        result.add(candidate)

    # Search effect configuration tables in adjacent text, including raw-string literals.
    for m in re.finditer(r'(?s)(?:PM_EFFECT_CONFIG|_data_FX|EFFECT_CONFIG)[A-Za-z_0-9]*.{0,350}?R"([A-Za-z_0-9=]*)\((.*?)\)\1"', all_text):
        add(m.group(2))
    for m in re.finditer(r'(?m)^\s*(?:const\s+)?(?:char|static\s+const\s+char)\s+(?:\w+::)?(?:PM_EFFECT_CONFIG|_data_FX|EFFECT_CONFIG)[\w]*\s*\[\s*\][^=]*=\s*"([^"\\]+)', all_text):
        add(m.group(1))
    # Firmware may reference config macros; find literal names in their defines.
    for m in re.finditer(r'(?m)^\s*#\s*define\s+(?:PM_EFFECT_CONFIG|EFFECT_CONFIG)[\w]*\s+"([^"\\]+)', all_text):
        add(m.group(1))
    # Some registrations contain literal effect names directly.
    for m in re.finditer(r'addEffect\s*\([^;]{0,700}', all_text):
        for name in re.findall(r'"([^"\n]{2,80})"', m.group()):
            if any(c in name for c in (';', '@')):
                add(name)
    # Common named configuration stored in PROGMEM, e.g. "Name@...;...".
    for m in re.finditer(r'"([A-Za-z][^"\\\n]{2,95}@[A-Za-z][^"\\\n]{0,95};[^"\\\n]*)"', all_text):
        add(m.group(1))
    return sorted(result, key=str.casefold)


def find_effects_cpp(root=None):
    roots = []
    if root:
        roots.append(Path(root))
    here = Path(__file__).resolve()
    roots.extend([here.parent, *list(here.parents)[:7], Path.home() / 'Documents' / 'GitHub' / 'PulSar'])
    candidates = []
    for base in roots:
        candidates.extend([base / 'code' / '6_Lights' / '03_Animator' / 'Effects.cpp',base / 'PulSar' / 'code' / '6_Lights' / '03_Animator' / 'Effects.cpp'])
    return next((p for p in candidates if p.is_file()), None)


def preset_compatible(document, available):
    """Return True only if an effect is explicitly supported, or no effect is set.

    Empty and palette-only presets are compatible because they do not select an
    effect; 'unknown' is treated as incompatible, not as supported.
    """
    fx = document.get('Effects') or {}
    if not isinstance(fx, dict):
        return False
    name = fx.get('FX', fx.get('Function'))
    if name in (None, ''):
        return True
    if isinstance(name, bool):
        return False
    if isinstance(name, int) or (isinstance(name, str) and name.strip().isdigit()):
        return int(name) in available
    actual = {normalize_effect(n) for n in available.values()}
    return normalize_effect(name) in actual
