"""Discover PulSar palette names from installed firmware sources and device WebUI.

C++ parser reads literal labels in palette-related headers and source, and 
uses existing library names. A device /json/palettes response is authoritative.
"""
from __future__ import annotations
import re
import json
from pathlib import Path
from urllib.request import Request, urlopen


def find_repo():
    here = Path(__file__).resolve()
    candidates = [here.parent, *here.parents, Path.home() / 'Documents' / 'GitHub' / 'PulSar', Path.home() / 'Documents' / 'GitHub_Personal' / 'PulSar']
    for root in candidates:
        for path in (root, root / 'PulSar'):
            if (path / 'code' / '6_Lights' / '02_Palette').exists():
                return path
    return None


def find_palette_sources():
    root = find_repo()
    if root is None:
        return []
    paths = [root / 'code' / '6_Lights' / '02_Palette' / 'mPalette.h', root / 'code' / '6_Lights' / '02_Palette' / 'mPalette.cpp', root / 'code' / '6_Lights' / '03_Animator' / 'Palettes.cpp']
    language = root / 'code' / '2_CoreSystem' / '11_Languages'
    if language.exists():
        paths.extend(sorted(language.glob('mLanguage*.h')))
    paths.extend(sorted((root / 'code' / '6_Lights' / '02_Palette').glob('*.h')))
    return sorted({p for p in paths if p.is_file()})


def read_source_palettes(paths):
    candidates = set()
    for path in paths:
        data = Path(path).read_text(encoding='utf-8', errors='replace')
        for m in re.finditer(r'(?m)^\s*#\s*define\s+(D_PALETTE[A-Za-z_0-9]*)\s+"([^"\\\n]{1,90})"', data):
            candidates.add(m.group(2).strip())
        for m in re.finditer(r'(?m)^\s*(?:DEFINE_PGM_CTR|DEF_PGM_CTR)\s*\(\s*(?:PM_PALETTE|D_PALETTE)[\w]*\s*\)\s*"([^"\\\n]{1,90})"', data):
            candidates.add(m.group(1).strip())
        for m in re.finditer(r'(?m)^\s*(?:const\s+)?(?:char\s*\*\s*|char\s+)(?:const\s+)?(?:PM_PALETTE|D_PALETTE)[\w]*[^=\n]*=\s*"([^"\\\n]{1,90})"', data):
            candidates.add(m.group(1).strip())
        # Some firmware resources use PROGMEM and a _NAME_CTR literal macro.
        for m in re.finditer(r'(?m)^\s*#\s*define\s+[A-Z0-9_]*(?:PALETTE|PAL)[A-Z0-9_]*(?:NAME|CTR)[A-Z0-9_]*\s+"([^"\\\n]{1,90})"', data):
            candidates.add(m.group(1).strip())
    result=set()
    for entry in candidates:
        # Firmware exposes first token as normal name, slash-separated variants as aliases.
        for name in entry.split('/'):
            name=name.strip()
            if name and not any(c in name for c in '{};%') and len(name)<80:
                result.add(name)
    return sorted(result,key=str.casefold)


def palette_names_from_library(catalog):
    names=set()
    for rec in catalog.entries.values():
        data=rec.get('data',{})
        for field in ('Pal','Pal2','ColourPalette','Palette2'):
            name=data.get(field)
            if isinstance(name,str) and name.strip():
                names.add(name.strip())
        for field in ('ColourPalette','Palette2'):
            name=(data.get('Effects') or {}).get(field)
            if isinstance(name,str) and name.strip():
                names.add(name.strip())
    return sorted(names,key=str.casefold)


def device_palettes(doc):
    if isinstance(doc,dict):
        items=doc.items()
    elif isinstance(doc,list):
        items=enumerate(doc)
    else:
        raise ValueError('Expected object or array from /json/palettes')
    result={}
    for key,value in items:
        if isinstance(value,dict):
            value=value.get('name',value.get('n',''))
        if isinstance(value,str) and value.strip():
            result[str(key)]=value.strip()
    if not result:
        raise ValueError('No palette names returned by the device')
    return result


def fetch_device_palettes(ip,port=80,timeout=8):
    url=f'http://{ip}:{port}/json/palettes'
    with urlopen(Request(url,headers={'Cache-Control':'no-cache'}),timeout=timeout) as response:
        raw=response.read(2_000_001)
    if len(raw)>2_000_000:
        raise ValueError('Palette list exceeds 2 MB')
    return device_palettes(json.loads(raw.decode('utf-8-sig')))
