#!/usr/bin/env python3
"""
Scan for PulSar devices advertised over mDNS.

py -m pip install zeroconf

Looks for:
  _pulsar._tcp.local.

Writes:
  output.txt
  output_prev.txt   backup of previous output.txt before overwrite
"""

from __future__ import annotations

import shutil
import socket
import time
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional

from zeroconf import ServiceBrowser, ServiceListener, ServiceStateChange, Zeroconf


SERVICE_TYPE = "_pulsar._tcp.local."
SCAN_SECONDS = 8.0

SCRIPT_DIR = Path(__file__).resolve().parent

OUTPUT_FILE = SCRIPT_DIR / "output.txt"
PREVIOUS_FILE = SCRIPT_DIR / "output_prev.txt"


@dataclass
class PulsarDevice:
    instance: str
    server: str
    port: int
    addresses: List[str]
    properties: Dict[str, str]

    @property
    def mac(self) -> str:
        return self.properties.get("mac", "")

    @property
    def name(self) -> str:
        return self.properties.get("name", "")

    @property
    def module(self) -> str:
        return self.properties.get("module", "")


class PulsarListener(ServiceListener):
    def __init__(self, zeroconf: Zeroconf):
        self.zeroconf = zeroconf
        self.devices: Dict[str, PulsarDevice] = {}

    def update_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        self._read_service(type_, name)

    def remove_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        # Keep removed devices out of the final dump for this scan.
        self.devices.pop(name, None)

    def add_service(self, zc: Zeroconf, type_: str, name: str) -> None:
        self._read_service(type_, name)

    def _read_service(self, type_: str, name: str) -> None:
        info = self.zeroconf.get_service_info(type_, name, timeout=3000)

        if info is None:
            return

        addresses: List[str] = []

        for raw_address in info.addresses:
            try:
                if len(raw_address) == 4:
                    addresses.append(socket.inet_ntop(socket.AF_INET, raw_address))
                elif len(raw_address) == 16:
                    addresses.append(socket.inet_ntop(socket.AF_INET6, raw_address))
            except OSError:
                pass

        properties: Dict[str, str] = {}

        for key, value in info.properties.items():
            key_s = key.decode("utf-8", errors="replace") if isinstance(key, bytes) else str(key)

            if value is None:
                value_s = ""
            elif isinstance(value, bytes):
                value_s = value.decode("utf-8", errors="replace")
            else:
                value_s = str(value)

            properties[key_s] = value_s

        self.devices[name] = PulsarDevice(
            instance=name,
            server=info.server or "",
            port=info.port,
            addresses=sorted(set(addresses)),
            properties=properties,
        )


def backup_previous_output() -> None:
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)

    if OUTPUT_FILE.exists():
        shutil.copy2(OUTPUT_FILE, PREVIOUS_FILE)


def format_device(device: PulsarDevice) -> str:
    lines: List[str] = []

    lines.append(f"Instance : {device.instance}")
    lines.append(f"Server   : {device.server}")
    lines.append(f"Port     : {device.port}")
    lines.append(f"Address  : {', '.join(device.addresses) if device.addresses else '<none>'}")
    lines.append(f"MAC      : {device.mac or '<missing>'}")
    lines.append(f"Name     : {device.name or '<missing>'}")
    lines.append(f"Module   : {device.module or '<missing>'}")

    if device.properties:
        lines.append("TXT      :")
        for key in sorted(device.properties):
            lines.append(f"  {key}={device.properties[key]}")
    else:
        lines.append("TXT      : <none>")

    return "\n".join(lines)


def write_output(devices: List[PulsarDevice]) -> None:
    OUTPUT_FILE.parent.mkdir(parents=True, exist_ok=True)

    now = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    lines: List[str] = []
    lines.append("PulSar mDNS scan")
    lines.append(f"Timestamp : {now}")
    lines.append(f"Service   : {SERVICE_TYPE}")
    lines.append(f"Duration  : {SCAN_SECONDS:.1f} s")
    lines.append(f"Found     : {len(devices)}")
    lines.append("")

    if not devices:
        lines.append("No PulSar devices found.")
    else:
        for index, device in enumerate(devices, start=1):
            lines.append("=" * 72)
            lines.append(f"Device {index}")
            lines.append("=" * 72)
            lines.append(format_device(device))
            lines.append("")

    OUTPUT_FILE.write_text("\n".join(lines), encoding="utf-8")


def main() -> int:
    backup_previous_output()

    zeroconf = Zeroconf()
    listener = PulsarListener(zeroconf)

    print(f"Scanning for {SERVICE_TYPE} for {SCAN_SECONDS:.1f} seconds...")
    browser = ServiceBrowser(zeroconf, SERVICE_TYPE, listener)

    try:
        time.sleep(SCAN_SECONDS)
    finally:
        browser.cancel()
        zeroconf.close()

    devices = sorted(
        listener.devices.values(),
        key=lambda d: (d.name.lower(), d.instance.lower(), ",".join(d.addresses)),
    )

    write_output(devices)

    print(f"Found {len(devices)} PulSar device(s).")
    print(f"Wrote {OUTPUT_FILE.resolve()}")

    if PREVIOUS_FILE.exists():
        print(f"Previous output backed up to {PREVIOUS_FILE.resolve()}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())