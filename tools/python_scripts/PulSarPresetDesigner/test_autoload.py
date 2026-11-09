from pathlib import Path
from tempfile import TemporaryDirectory
import main
with TemporaryDirectory() as d:
    f=Path(d)/'Effects.cpp'
    f.write_text('const char PM_EFFECT_CONFIG__FIREFLY[] PROGMEM = "Firefly@Speed;!;1";')
    main.find_effects_cpp=lambda: f
    app=main.App()
    for _ in range(8):
        app.update()
        import time; time.sleep(.12)
    assert 'Firefly' in app.source_effects, app.source_effects
    assert 'Firefly' in app.fx_choice['values']
    assert not hasattr(app,'effects_path')
    app.destroy()
print('Automatic startup firmware catalog and effect combobox: OK')
