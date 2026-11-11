"""Push a local PulSar presets.json to one or more devices via the /edit filesystem API.

No external packages required.
Run: py push_pulsar_presets.py
"""
from pathlib import Path
from datetime import datetime
from urllib.parse import urlsplit, urlencode
from urllib.request import Request, build_opener, ProxyHandler
from urllib.error import HTTPError, URLError
import hashlib
import json
import re
import socket
import sys
import uuid

# ======================== CONFIGURATION ========================
src_preset_file = r"notes\PresetJsonFiles\Christmas2026_Shared_9Nov26\presets.json"
# dest_devices = [
#     "col26--light01--whitehall-front",  # replace with actual mDNS hostname
#     "col26--light02--meadows-front",  # replace with actual mDNS hostname
#     "col26--light03--meadows-side",  # replace with actual mDNS hostname
    # ]
dest_devices = [
    # "192.168.0.170", # whitehall
    # "192.168.0.172", # meadows front
    # "192.168.0.173", # meadows side
    # "192.168.0.174", # 2d matrix
    # "192.168.0.160", # 2d matrix fridge
    "192.168.0.162", # pink serial tester
    ]
filesystem_path = "/presets.json"

backup_directory = Path(__file__).resolve().parent / "preset_upload_backups"
http_timeout_seconds = 15
# Set to False to ask before overwriting each device.
auto_confirm = False
# ===============================================================


def device_url(device):
    device = device.strip()
    if not device:
        raise ValueError("Empty device name")
    if "://" not in device:
        if not device.endswith(".local") and not _is_ip(device):
            device += ".local"
        device = "http://" + device
    u = urlsplit(device)
    if u.scheme not in ("http", "https") or not u.hostname:
        raise ValueError(f"Invalid device address: {device}")
    return f"{u.scheme}://{u.netloc}"


def _is_ip(host):
    try:
        socket.inet_aton(host)
        return True
    except OSError:
        return False


def sha256(data):
    return hashlib.sha256(data).hexdigest()


# Bypass Windows/system HTTP proxies for local device connections.
HTTP = build_opener(ProxyHandler({}))

def http_get(url):
    request = Request(url, headers={"Cache-Control": "no-cache"})
    with HTTP.open(request, timeout=http_timeout_seconds) as response:
        return response.read()

def http_post_file(url, filename, payload):
    # Matches FileEditor::handleUpload: multipart field "data", filename "/presets.json".
    boundary = "----PulSar" + uuid.uuid4().hex
    header = (
        f"--{boundary}\r\n"
        f'Content-Disposition: form-data; name="data"; filename="{filename}"\r\n'
        "Content-Type: application/json\r\n\r\n"
    ).encode("ascii")
    body = header + payload + f"\r\n--{boundary}--\r\n".encode("ascii")
    request = Request(url, data=body, method="POST", headers={
        "Content-Type": f"multipart/form-data; boundary={boundary}",
        "Content-Length": str(len(body)),
    })
    with HTTP.open(request, timeout=http_timeout_seconds) as response:
        return response.read().decode("utf-8", errors="replace")

def locate_source(filename):
    source = Path(filename)
    if source.is_absolute() or source.is_file():
        return source
    # Allow running script from a tools/ subfolder or directly from repo root.
    for parent in (Path(__file__).resolve().parent, *Path(__file__).resolve().parents):
        candidate = parent / source
        if candidate.is_file():
            return candidate
    return source

def main():
    src = locate_source(src_preset_file)
    if not src.is_file():
        sys.exit(f"ERROR: Source file not found: {src.resolve()}")

    try:
        parsed = json.loads(src.read_text(encoding="utf-8-sig"))
        if not isinstance(parsed, dict):
            raise ValueError("Top-level JSON must be an object")
        preset1 = parsed.get("1")
        if not isinstance(preset1, dict) or not isinstance(preset1.get("n"), str):
            raise ValueError('Preset index 1 must contain a string "n"')
    except (UnicodeError, json.JSONDecodeError, ValueError) as exc:
        sys.exit(f"ERROR: Invalid presets JSON: {exc}")

    # Temporary deployment marker: DDMMHHMM, refreshed on each invocation.
    # Only preset 1 is renamed; IDs, effects and playlist references remain untouched.
    old_name = preset1["n"]
    clean_name = re.sub(r"^(?:\d{2}[A-Za-z]{3}\d{2}[a-z]|\d{8})\s+", "", old_name)
    new_name = datetime.now().strftime("%d%m%H%M") + " " + clean_name
    preset1["n"] = new_name

    # Keep the on-disk presets.json synchronized with the uploaded bytes.
    # One compact JSON object per preset/playlist instead of a single huge line.
    entries = [f'  {json.dumps(key)}:{json.dumps(value, ensure_ascii=False, separators=(",", ":"))}' for key, value in parsed.items()]
    payload = ("{\n" + ",\n".join(entries) + "\n}\n").encode("utf-8")
    src.write_bytes(payload)
    print(f"Preset 1 revision: {old_name!r} -> {new_name!r}")

    if not dest_devices:
        sys.exit("ERROR: dest_devices is empty")

    print(f"Source: {src.resolve()}")
    print(f"Entries: {len(parsed)} | Size: {len(payload)} bytes | SHA256: {sha256(payload)}")
    print(f"Destination: {filesystem_path}")

    results = []

    for name in dest_devices:
        print(f"\n--- {name} ---")
        try:
            base = device_url(name)
            endpoint = base + "/edit"

            # Read current file first. Do not overwrite if a backup cannot be made.
            download_url = endpoint + "?" + urlencode({"download": filesystem_path})
            original = http_get(download_url)
            if not original:
                raise RuntimeError("Existing presets file is empty; refusing overwrite")
            try:
                json.loads(original.decode("utf-8-sig"))
            except (UnicodeError, json.JSONDecodeError) as exc:
                raise RuntimeError(f"Existing device file is not valid JSON: {exc}") from exc

            backup_directory.mkdir(parents=True, exist_ok=True)
            stamp = datetime.now().strftime("%Y%m%d_%H%M%S_%f")
            safe_name = "".join(c if c.isalnum() or c in "-_" else "_" for c in name)
            backup_path = backup_directory / f"{safe_name}_{stamp}_presets.json"
            backup_path.write_bytes(original)
            print(f"Backup: {backup_path} ({len(original)} bytes)")

            if original == payload:
                print("Already identical; skipped upload.")
                results.append((name, "IDENTICAL"))
                continue

            if not auto_confirm:
                answer = input(f"Overwrite {base}{filesystem_path}? [y/N]: ").strip().lower()
                if answer != "y":
                    print("Skipped.")
                    results.append((name, "SKIPPED"))
                    continue

            # FileEditor::handleUpload opens the incoming filename in write mode.
            # The multipart form field must be named 'data'.
            response_text = http_post_file(endpoint, filesystem_path, payload)
            if "UPLOADED:" not in response_text or filesystem_path not in response_text:
                raise RuntimeError(f"Unexpected upload response: {response_text[:200]!r}")

            verified_data = http_get(download_url)
            if verified_data != payload:
                raise RuntimeError(
                    f"Readback mismatch: local {sha256(payload)}, device {sha256(verified_data)}"
                )

            print(f"Verified: {len(payload)} bytes, SHA256 {sha256(payload)}")
            results.append((name, "OK"))
        except (HTTPError, URLError, OSError, ValueError, RuntimeError) as exc:
            print(f"FAILED: {exc}")
            results.append((name, "FAILED"))

    print("\nSummary:")
    for name, status in results:
        print(f"  {status:9} {name}")
    if any(status == "FAILED" for _, status in results):
        sys.exit(1)


if __name__ == "__main__":
    main()
