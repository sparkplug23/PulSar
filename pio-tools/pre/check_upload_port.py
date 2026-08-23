Import("env")

import ctypes


MB_YESNO = 0x00000004
MB_ICONWARNING = 0x00000030
MB_TOPMOST = 0x00040000

IDYES = 6


def classify_upload_port(upload_port):
    if not upload_port:
        return "missing"

    port_lower = upload_port.lower()
    port_upper = upload_port.upper()

    if port_lower.endswith(".local"):
        return "mdns"

    if port_upper.startswith("COM"):
        return "com"

    if upload_port[0].isdigit():
        return "ip"

    return "other"


def confirm_non_mdns_upload(env_name, upload_port, port_type):
    if port_type == "ip":
        reason = "raw IP address"
    elif port_type == "com":
        reason = "COM / serial port"
    elif port_type == "missing":
        reason = "missing upload port"
    else:
        reason = "non-mDNS hostname or other upload target"

    message = (
        f"Environment:\n"
        f"{env_name}\n\n"
        f"Upload port:\n"
        f"{upload_port if upload_port else '<empty>'}\n\n"
        f"This appears to be a {reason}.\n\n"
        f"For group flashing, PulSar devices should normally use an mDNS "
        f".local upload port so that DHCP/IP changes cannot cause firmware "
        f"to be uploaded to the wrong device.\n\n"
        f"Proceed with this upload?"
    )

    result = ctypes.windll.user32.MessageBoxW(
        0,
        message,
        "PulSar Upload Safety Check",
        MB_YESNO | MB_ICONWARNING | MB_TOPMOST
    )

    return result == IDYES


def check_upload_port(source, target, env):
    env_name = str(env["PIOENV"])
    upload_port = str(env.subst("$UPLOAD_PORT")).strip()
    port_type = classify_upload_port(upload_port)

    print("")
    print(f"[PulSar Upload Safety] Environment: {env_name}")
    print(f"[PulSar Upload Safety] Upload port: {upload_port if upload_port else '<empty>'}")

    if port_type == "mdns":
        print("[PulSar Upload Safety] mDNS upload target confirmed.")
        return

    if not confirm_non_mdns_upload(env_name, upload_port, port_type):
        print(f"[PulSar Upload Safety] Upload cancelled for {env_name}.")
        env.Exit(1)

    print(f"[PulSar Upload Safety] Non-mDNS upload manually approved for {env_name}.")


env.AddPreAction("upload", check_upload_port)