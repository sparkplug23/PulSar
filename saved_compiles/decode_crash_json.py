#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# saved_compiles/decode_crash_json.py
#
# Decode PulSar crash_report.json using addr2line + firmware.elf.
# Unified format (ONLY):
#   {"ESP32":{
#       "Exception":<int>,
#       "Reason":"<string>",              # optional
#       "EPC":"0x........",               # optional
#       "EXCVADDR":"0x........",           # optional
#       "CallChain":["0x........", ...]    # preferred
#   }}
#
# Date Modified: 02Feb26

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


# -------------------------
# Helpers
# -------------------------

def _find_project_root(start: Path) -> Path:
    """Walk up until we find a folder containing 'saved_compiles'."""
    p = start.resolve()
    for _ in range(12):
        if (p / "saved_compiles").is_dir():
            return p
        if p.parent == p:
            break
        p = p.parent
    raise FileNotFoundError("Could not locate project root (folder containing 'saved_compiles').")


def _load_json(path: Path) -> Dict[str, Any]:
    with path.open("r", encoding="utf-8") as f:
        return json.load(f)


def _hex_u32_any(v: Any) -> int:
    """
    Accept:
      - int
      - hex string "0x........"
      - decimal string
      - bare hex string (>=8 chars) like "400d1234"
    """
    if isinstance(v, int):
        return int(v) & 0xFFFFFFFF

    if not isinstance(v, str):
        raise ValueError(f"Expected hex/int value, got {type(v).__name__}")

    s = v.strip()
    if s.startswith(("0x", "0X")):
        return int(s, 16) & 0xFFFFFFFF

    # allow bare hex or decimal, but treat as hex if it looks like hex and is long enough
    is_hexish = all(c in "0123456789abcdefABCDEF" for c in s)
    if is_hexish and len(s) >= 8:
        return int(s, 16) & 0xFFFFFFFF

    return int(s, 10) & 0xFFFFFFFF


def _fmt_u32_hex(v: Any) -> str:
    """Format v as 0x........ using _hex_u32_any if possible."""
    try:
        x = _hex_u32_any(v)
        return f"0x{x:08x}"
    except Exception:
        # best-effort fallback
        return str(v)


def _which(exe_name: str) -> Optional[str]:
    for p in os.environ.get("PATH", "").split(os.pathsep):
        cand = Path(p) / exe_name
        if cand.is_file():
            return str(cand)
    return None


def _auto_find_addr2line() -> Optional[str]:
    """
    Typical PlatformIO Windows locations:
      C:\\Users\\<user>\\.platformio\\packages\\toolchain-xtensa-esp-elf\\bin\\xtensa-esp32-elf-addr2line.exe
      C:\\Users\\<user>\\.platformio\\packages\\toolchain-xtensa-esp32\\bin\\xtensa-esp32-elf-addr2line.exe
    """
    home = Path.home()
    candidates = [
        home / ".platformio" / "packages" / "toolchain-xtensa-esp-elf" / "bin" / "xtensa-esp32-elf-addr2line.exe",
        home / ".platformio" / "packages" / "toolchain-xtensa-esp32" / "bin" / "xtensa-esp32-elf-addr2line.exe",
    ]
    for c in candidates:
        if c.is_file():
            return str(c)

    w = _which("xtensa-esp32-elf-addr2line.exe")
    if w:
        return w

    return None


def _addr2line(addr2line_exe: str, elf_path: Path, pc: int) -> str:
    """
    addr2line:
      -f function name
      -C demangle
      -p pretty (single line)
      -i inlines
    """
    cmd = [
        addr2line_exe,
        "-f", "-C", "-p", "-i",
        "-e", str(elf_path),
        f"0x{pc:08x}",
    ]
    try:
        out = subprocess.check_output(cmd, stderr=subprocess.STDOUT).decode("utf-8", errors="replace")
        out = out.strip()
        return out if out else "?? ??:0"
    except subprocess.CalledProcessError as ex:
        msg = ex.output.decode("utf-8", errors="replace").strip()
        return f"addr2line failed: {msg}"


def _dedupe_consecutive(pcs: List[int]) -> List[int]:
    if not pcs:
        return pcs
    out = [pcs[0]]
    for x in pcs[1:]:
        if x != out[-1]:
            out.append(x)
    return out


# -------------------------
# New unified parser
# -------------------------

def _parse_unified(report: Dict[str, Any]) -> Tuple[int, Optional[str], str, str, List[int]]:
    """
    Returns:
      (exception_code, reason_str_or_None, epc_str, excvaddr_str, pcs[])
    """
    if "ESP32" not in report or not isinstance(report["ESP32"], dict):
        raise ValueError("Expected top-level key 'ESP32' containing an object")

    e = report["ESP32"]

    exc = int(e.get("Exception", 0))
    reason = e.get("Reason", None)
    if reason is not None and not isinstance(reason, str):
        reason = str(reason)

    epc = _fmt_u32_hex(e.get("EPC", "0x00000000"))
    excv = _fmt_u32_hex(e.get("EXCVADDR", "0x00000000"))

    # Preferred: CallChain
    callchain = e.get("CallChain", None)
    pcs: List[int] = []

    if callchain is None:
        # Unified format expects CallChain, but don't hard-crash: treat as "no data"
        return exc, reason, epc, excv, pcs

    if not isinstance(callchain, list):
        raise ValueError("Expected 'ESP32.CallChain' to be a list")

    for item in callchain:
        pcs.append(_hex_u32_any(item))

    return exc, reason, epc, excv, pcs


# -------------------------
# Main
# -------------------------

def main() -> int:
    ap = argparse.ArgumentParser()

    
    # ap.add_argument("--env", default="testbed_01__sensors_nextion", help="PlatformIO env name")
    # ap.add_argument("--env", default="tg_lgt__esp32_matrix_32w8h", help="PlatformIO env name")
    ap.add_argument("--env", default="cm_office__ws2815panel12v", help="PlatformIO env name")
    # ap.add_argument("--env", default="cm_lgt__esp32_coneV_1D", help="PlatformIO env name")
    # ap.add_argument("--env", default="cm_office__rgbww_vertical", help="PlatformIO env name")
    # ap.add_argument("--env", default="cm_kitchen__glass_box", help="PlatformIO env name")
    # ap.add_argument("--env", default="tg_lgt__esp32_matrix_32w8h_weight", help="PlatformIO env name")












    ap.add_argument("--project-root", default=None, help="PulSar project root (folder containing saved_compiles)")
    ap.add_argument("--report", default=None, help="crash_report.json path (overrides env default)")
    ap.add_argument("--elf", default=None, help="firmware.elf path (overrides env default)")
    ap.add_argument("--addr2line", default=None, help="full path to xtensa-esp32-elf-addr2line.exe")
    ap.add_argument("--no-dedupe", action="store_true", help="do not remove consecutive duplicate PCs")
    args = ap.parse_args()

    script_path = Path(__file__).resolve()
    project_root = Path(args.project_root).resolve() if args.project_root else _find_project_root(script_path)

    env_name = args.env
    crash_report = Path(args.report).resolve() if args.report else (project_root / "saved_compiles" / env_name / "crash_report.json")
    firmware_elf = Path(args.elf).resolve() if args.elf else (project_root / "saved_compiles" / env_name / "firmware.elf")

    if not crash_report.is_file():
        raise FileNotFoundError(f"Missing crash report: {crash_report}")
    if not firmware_elf.is_file():
        raise FileNotFoundError(f"Missing firmware ELF: {firmware_elf}")

    addr2line_exe = args.addr2line or os.environ.get("ADDR2LINE_EXE") or _auto_find_addr2line()
    if not addr2line_exe or not Path(addr2line_exe).is_file():
        raise FileNotFoundError(
            "Could not find addr2line automatically.\n"
            "Set --addr2line <full path> or environment variable ADDR2LINE_EXE."
        )

    report = _load_json(crash_report)
    exc, reason, epc, excv, pcs = _parse_unified(report)

    if not args.no_dedupe:
        pcs = _dedupe_consecutive(pcs)

    print("------------------------------------------------------------")
    print(f"PROJECT_ROOT : {project_root}")
    print(f"ENV_NAME      : {env_name}")
    print(f"CRASH_REPORT  : {crash_report}")
    print(f"FIRMWARE_ELF  : {firmware_elf}")
    print(f"ADDR2LINE     : {addr2line_exe}")
    print(f"Exception     : {exc}")
    if reason is not None:
        print(f"Reason        : {reason}")
    print(f"EPC           : {epc}")
    print(f"EXCVADDR      : {excv}")

    if not pcs:
        print("PCs: <none>")
        print("------------------------------------------------------------")
        print()
        print("No CallChain addresses found in crash_report.json.")
        print("This usually means either:")
        print("  - the crash recorder did not capture (wrappers not hit), or")
        print("  - RTC record was cleared/never written, or")
        print("  - you produced a reset (not an exception/panic path).")
        return 2

    print("PCs:")
    for i, pc in enumerate(pcs):
        print(f"  [{i:02d}] 0x{pc:08x}")
    print("------------------------------------------------------------\n")

    for pc in pcs:
        line = _addr2line(addr2line_exe, firmware_elf, pc)
        print(f"0x{pc:08x}: {line}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
