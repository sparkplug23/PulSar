# PulSar Preset & Playlist Designer

Desktop editor for authoring reusable PulSar presets and playlists. All presets and playlists are shown together in one GUI. The library has **exactly three authoritative JSON files**:

```text
lib/
    presets_1d.json
    presets_2d.json
    playlists.json
```

Each file is a JSON **array** containing the complete preset/playlist bodies. No device preset IDs are stored in these files. Playlists reference presets via `playlist.psn` (names); `playlist.ps` is generated only during device export.

Presets are stored in the 1D or 2D file based on the preset's intended effect type. Playlists can use both 1D and 2D presets and therefore all share `playlists.json`.

## Running

From the PulSar checkout:

```powershell
& "$env:APPDATA\uv\python\cpython-3.15.1-windows-x86_64-none\python.exe" ".\tools\python_scripts\PulSarPresetDesigner\main.py"
```

For mDNS discovery, install `zeroconf` into the interpreter running the GUI. Manual IP operation does not require mDNS.

## Workflow (v0.8)

1. **Preset library**: Browse preset templates from both `lib/presets_1d.json` and `lib/presets_2d.json` in one list. Three columns: preset list, structured editor, raw JSON. Library changes are written to the master file only when you deliberately edit/save a template.
2. **Device Build**: Select any library preset or playlist and click **Add copy**; or select a preset in the library and click **Copy to Device Build**. Edit copies using the structured editor or raw JSON. Working changes are persisted in `workspace/working_set.json`, not in `lib/`.
3. If a working preset is worth reusing, use **Save copy to library**. It asks before adding or replacing the template.
4. **Playlist editor**: Edit playlist templates by name and duration, stored only in `lib/playlists.json`. To include a playlist in the device build, copy it into Device Build. You can edit working playlist JSON there without changing the source playlist.
5. **Build / export**: Only working copies are eligible for export. **Select playlist dependencies** copies referenced presets into the working set from the library. Export validates names and generates numeric IDs and `playlist.ps` into a timestamped `exports/.../presets.json`.
6. **Devices**: Can fetch/import presets, send a working preset live, and upload a generated file. Upload first downloads and verifies an exact-byte backup of the existing device file, and verifies after transfer.

The master library contains **exactly three authoritative arrays**. `workspace/working_set.json` is an independent editable draft/session, **not** another library. Re-importing a device collection does not change the master library unless explicitly requested through the import workflow.

## Library and exports

- Create, duplicate, edit, and delete 1D/2D presets in their appropriate source array.
- All playlist definitions are kept in `lib/playlists.json`, irrespective of the presets they reference.
- Edit an individual entry through the GUI parameter table or its standalone raw JSON body.
- Import an ESP32 `presets.json`: preset IDs are discarded; playlist `ps` references are converted to names when `psn` is missing.
- Build/export: select the desired working entries, validate unique names and references, generate numeric IDs and `playlist.ps`, and write a timestamped `exports/.../presets.json`.
- Select a PulSar device to fetch its existing presets or deploy a new export. Uploads must first back up the device's existing file.

## Migration from v0.6

When `lib/playlists.json` is absent, the GUI merges older `lib/playlists_1d.json`, `lib/playlists_2d.json`, and `lib/playlists_audio_reactive.json` arrays into the single playlist file and removes the generated numeric `ps` arrays from the new source library. Obsolete aggregate files are moved to `migration_backups/YYYY-MM-DD_HH-MM-SS/` so `lib/` contains only the three current JSON files; their contents remain available for rollback. Existing `lib/presets_1d.json` and `lib/presets_2d.json` are retained. If the new preset files don't exist, the GUI imports corresponding older arrays or legacy individual preset files.

The old `lib/_imported_ids.json` file is **not used**. Numeric IDs belong only to the generated device `presets.json` and its export manifest. The new package contains only three library JSON files.

## Source catalogs

The GUI discovers local PulSar C++ sources relative to the project to extract effect and palette names. A selected ESP32's `/json/effects` and `/json/palettes` provide its actual compiled capability list.
