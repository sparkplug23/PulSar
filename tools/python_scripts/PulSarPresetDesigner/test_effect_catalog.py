from effect_catalog import device_effects, preset_compatible, read_source_effects
from pathlib import Path
from tempfile import TemporaryDirectory


def test():
    enabled = device_effects({'0':'Static Palette','3':'Firefly','5':'RSVD'})
    assert enabled == {0:'Static Palette',3:'Firefly'}
    assert preset_compatible({'Effects':{'FX':'Firefly'}},enabled)
    assert preset_compatible({'Effects':{'FX':' firefly  '}},enabled)
    assert preset_compatible({'Effects':{'FX':3}},enabled)
    assert not preset_compatible({'Effects':{'FX':'Meteor'}},enabled)
    assert not preset_compatible({'Effects':{'FX':5}},enabled)
    assert preset_compatible({'Pal':'Snowy 02'},enabled)
    with TemporaryDirectory() as tmp:
        path=Path(tmp)/'Effects.cpp'
        path.write_text('const char PM_EFFECT_CONFIG__FIREFLY[] PROGMEM = "Firefly@Speed,Intensity;!;1";\nconst char PM_EFFECT_CONFIG__STATIC[] PROGMEM = R"=====('+ 'Static Palette@Speed;!;1'+')=====";')
        assert read_source_effects(path) == ['Firefly','Static Palette']
    print('Effect source parsing and device compatibility: OK')

if __name__ == '__main__':
    test()
