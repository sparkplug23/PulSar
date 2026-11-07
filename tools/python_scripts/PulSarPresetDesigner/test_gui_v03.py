from main import App
app=App()
app.update()
assert len(app.tabs.tabs()) >= 8
for tab in app.tabs.tabs():
    app.tabs.select(tab)
    app.update()
for category in ('2d','audio_reactive','1d'):
    app.category.set(category)
    app.refresh_library()
    app.update()
assert len(app.palette_choice['values']) > 0
app.destroy()
print('v0.3 GUI initialization, every tab, three profiles, palette dropdown: OK')
