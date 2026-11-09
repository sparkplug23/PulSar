# PulSar Preset & Playlist Designer — v0.4

A Windows-friendly Tkinter editor for PulSar effects, palettes, device playlists and JSON. Source libraries are stored as individual JSON resource files under `library/presets/{1d,2d,audio_reactive}` and `library/playlists/{1d,2d,audio_reactive}`. Older 1d JSON resource collections and their original ID index are retained for compatibility.

## Start (Windows)

Double-click `Launch_PulSar_Designer.cmd`, or from PowerShell run:

```powershell
& "$HOME\.local\bin\python3.15.exe" .\main.py
```

The standard Tkinter desktop GUI requires no GUI dependency installation. The interface starts in dark mode.

## What is included

- **Preset library / JSON:** browse, create, duplicate and delete, edit scalar/nested JSON parameters, choose Effect and Palette from catalogs, copy/paste the **entire JSON body** corresponding to one numeric entry in `presets.json`. Use **Apply JSON / Save** to persist changes.
- **Unified library:** all 1D, 2D and audio-reactive presets/playlists are shown together. Category is a storage-file property, not a global GUI mode. The library and export tables display the source category. When creating or importing, choose which category folder should hold the files.
- **Playlist designer:** build `psn[]`-based sequences; edit order and duration. `ps[]` is generated at export, never used as the editable source of truth.
- **Playlist visualizer / matrix:** offline playlist usage matrix (presets as rows, playlists as columns) and a playlist detail view with ordered preset names and durations. Double-click a preset row to open the JSON editor.
- **Palettes / colours:** scan `mPalette_Progmem.h`, `mPalette.h`, language headers and related C++ sources in the local PulSar repository for literal palette labels; fall back to names used by existing preset resources. If a target is selected, fetch its authoritative `GET /json/palettes` list. Slash-separated firmware aliases are included.
- **Effects:** automatically read effect names from the PulSar checkout (`code/6_Lights/03_Animator/Effects.cpp` and related files) when the GUI starts; fetch `GET /json/effects` from an ESP32. The latter is the authority on what the firmware has enabled. Optional filtering hides incompatible presets without deleting them.
- **Devices / sync:** use mDNS `_pulsar._tcp.local.` or type `192.168.x.y` / hostname into the persistent IP field at the top. Select a target, fetch its presets file, import it into your chosen storage category, upload a generated export, or open its WebUI.
- **Live preset test:** select a preset and click **Test live on ESP32**; POST the currently edited JSON parameters to `/json/si`. This is a runtime effect test and does not save to `/presets.json`.
- **Live WebUI:** open the full browser directly. The **Live test / device WebUI** tab can optionally launch an independent `pywebview` desktop window, which supports the firmware's JavaScript UI. Tkinter itself cannot embed the full interactive browser without such an extra engine.

## mDNS discovery on Windows

mDNS requires the `zeroconf` module in the **same interpreter that launches the GUI**. In v0.2 it was missing and caused the error in your screenshot.

Use `Install_mDNS.cmd`, or the **Install mDNS dependency** button within the Devices tab, or run:

```powershell
& "$HOME\.local\bin\python3.15.exe" -m pip install --break-system-packages zeroconf
```

Manual IP entry, HTTP downloads and uploads work even if `zeroconf` is not installed. Discovery finds devices on the same reachable mDNS network segment; routers/VLANs may block multicast discovery.

## Import, generate and deploy

1. Browse and edit presets/playlists from **all library files together**. The storage category appears in a separate table column.
2. Click **Open presets.json** and select your existing November source, e.g. `notes/PresetJsonFiles/November2026/xmas26_outside_trees/presets.json`. When importing, choose 1d, 2d or audio_reactive as the file storage destination. Import can append or replace matching names in that destination.
3. Edit individual presets in either view. The JSON body editor matches the inner value of `"1": {...}`.
4. Build playlists by name, then use **Build / export** to select entries and generate `exports/YYYY-MM-DD_HH-MM-SS/presets.json` and `manifest.json`.
5. Use the Devices tab to choose a discovered/manual target and upload the generated file.

**Safety:** Every upload first GETs the current `/presets.json`, saves the **original bytes** to `exports/device_backups/<host>/<timestamp>/presets.json`, creates a checksum manifest, verifies local backup integrity, and only then submits POST `/upload`. If the existing file cannot be fetched or the backup fails, **upload is cancelled**. After uploading, the device file is downloaded and compared to the proposed JSON; failure is reported explicitly. Do not interrupt device power during upload.

**Limitations:** Not tested against your actual physical ESP32 devices from this environment. The selected device's exposed WebUI endpoints may differ on older firmware builds. Source scanning is best-effort; effect/palette lists obtained from the device are authoritative. Native playlist visualization depicts the sequence and preset usage, **not a pixel-accurate simulation** of LED effects. The full PulSar effect preview must run on a device and is accessed via the live WebUI. The default libraries are the earlier 2025 sample; import the current November 2026 file explicitly to migrate it.

## Tests

```powershell
py test_core.py
py test_effect_catalog.py
py test_v03.py
```

`test_gui_v03.py` requires a graphical desktop session (or a virtual display on Linux).

## Automatic firmware effect catalog (v0.5)

When launched inside the PulSar repository, the designer automatically locates `code/6_Lights/03_Animator/Effects.cpp` relative to its own script path and parses effect labels when it opens. There is no source file selector or manual parsing step. The **Refresh source catalog** button rescans the current checkout after firmware edits. The device capability filter continues to use the connected board’s live `/json/effects` instead of assuming every effect in the repository is compiled.
