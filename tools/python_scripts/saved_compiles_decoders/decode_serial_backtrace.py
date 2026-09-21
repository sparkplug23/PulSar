#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# decode_serial_backtrace.py
#
# Decode pasted ESP32 serial Backtrace block using addr2line + firmware.elf
#
# Date Modified: 21Feb26

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path
from typing import List, Optional


# ============================================================
# >>> PASTE YOUR SERIAL BACKTRACE BLOCK HERE <<<
# ============================================================

def _SerialBacktraceBlock() -> str:
    return r"""
Backtrace:0x40085835:0x3ffb24d00x4008fc2d:0x3ffb24f0 0x40095681:0x3ffb2510 0x401
a5833:0x3ffb2590 0x401a587a:0x3ffb25b0 0x401a57db:0x3ffb25d0 0x401a5b3e:0x3ffb25
f0 0x401a6361:0x3ffb2610 0x4011eb93:0x3ffb2630 0x400dbc67:0x3ffb2670 0x400dbe4d:
0x3ffb2750 0x400dbf42:0x3ffb2780 0x400d4007:0x3ffb27a0 0x4011bd20:0x3ffb27e0 0x4
011bfee:0x3ffb2800 0x401334d1:0x3ffb2820
""".strip()


# ============================================================
# CONFIGURATION
# ============================================================

ENV_NAME = "cm_lgt__esp32_coneH_1D"   # <<< change if needed
PROJECT_ROOT = None  # None = auto-detect (folder containing saved_compiles)


# ============================================================
# Helpers
# ============================================================

def _find_project_root(start: Path) -> Path:
    p = start.resolve()
    for _ in range(12):
        if (p / "saved_compiles").is_dir():
            return p
        if p.parent == p:
            break
        p = p.parent
    raise FileNotFoundError("Could not locate project root (folder containing 'saved_compiles').")


def _extract_pcs(text: str) -> List[int]:
    """
    Extract PC values from Backtrace line(s).
    Handles broken lines like:
      0x401
      a5833
    """
    cleaned = text.replace("\n", "")
    matches = re.findall(r"0x([0-9a-fA-F]{8})", cleaned)
    pcs = [int(m, 16) for m in matches]
    return pcs


def _dedupe_consecutive(pcs: List[int]) -> List[int]:
    if not pcs:
        return pcs
    out = [pcs[0]]
    for x in pcs[1:]:
        if x != out[-1]:
            out.append(x)
    return out


def _detect_arch_from_elf(elf_path: Path) -> str:
    """
    Detect architecture from ELF header.
    Returns 'xtensa' or 'riscv'.
    """
    with elf_path.open("rb") as f:
        hdr = f.read(20)

    # e_machine offset = 18-19 (little endian)
    e_machine = int.from_bytes(hdr[18:20], "little")

    if e_machine == 94:   # EM_XTENSA
        return "xtensa"
    if e_machine == 243:  # EM_RISCV
        return "riscv"

    raise RuntimeError(f"Unknown ELF e_machine={e_machine}")


def _auto_find_addr2line(arch: str) -> str:
    home = Path.home()

    if arch == "xtensa":
        candidates = [
            home / ".platformio" / "packages" / "toolchain-xtensa-esp-elf" / "bin" / "xtensa-esp32-elf-addr2line.exe",
            home / ".platformio" / "packages" / "toolchain-xtensa-esp32" / "bin" / "xtensa-esp32-elf-addr2line.exe",
        ]
        exe_name = "xtensa-esp32-elf-addr2line.exe"
    else:
        candidates = [
            home / ".platformio" / "packages" / "toolchain-riscv32-esp" / "bin" / "riscv32-esp-elf-addr2line.exe",
        ]
        exe_name = "riscv32-esp-elf-addr2line.exe"

    for c in candidates:
        if c.is_file():
            return str(c)

    for p in os.environ.get("PATH", "").split(os.pathsep):
        cand = Path(p) / exe_name
        if cand.is_file():
            return str(cand)

    raise FileNotFoundError(f"Could not find addr2line for arch '{arch}'")


def _addr2line(addr2line_exe: str, elf_path: Path, pc: int) -> str:
    cmd = [
        addr2line_exe,
        "-f", "-C", "-p", "-i",
        "-e", str(elf_path),
        f"0x{pc:08x}",
    ]
    try:
        out = subprocess.check_output(cmd, stderr=subprocess.STDOUT)
        return out.decode("utf-8", errors="replace").strip()
    except subprocess.CalledProcessError as ex:
        return ex.output.decode("utf-8", errors="replace").strip()


# ============================================================
# Main
# ============================================================

def main() -> int:

    script_path = Path(__file__).resolve()
    project_root = Path(PROJECT_ROOT).resolve() if PROJECT_ROOT else _find_project_root(script_path)

    elf_path = project_root / "saved_compiles" / ENV_NAME / "firmware.elf"
    if not elf_path.is_file():
        raise FileNotFoundError(f"Missing firmware ELF: {elf_path}")

    block = _SerialBacktraceBlock()
    if "Backtrace:" not in block:
        print("No Backtrace found in pasted block.")
        return 1

    pcs = _extract_pcs(block)
    pcs = _dedupe_consecutive(pcs)

    if not pcs:
        print("No PCs extracted.")
        return 1

    arch = _detect_arch_from_elf(elf_path)
    addr2line_exe = _auto_find_addr2line(arch)

    print("------------------------------------------------------------")
    print(f"ENV_NAME     : {ENV_NAME}")
    print(f"FIRMWARE_ELF : {elf_path}")
    print(f"ARCH         : {arch}")
    print(f"ADDR2LINE    : {addr2line_exe}")
    print("------------------------------------------------------------\n")

    print("PCs:")
    for i, pc in enumerate(pcs):
        print(f"  [{i:02d}] 0x{pc:08x}")

    print("\nDecoded:\n")

    for pc in pcs:
        line = _addr2line(addr2line_exe, elf_path, pc)
        print(f"0x{pc:08x}: {line}")

    return 0


if __name__ == "__main__":
    sys.exit(main())