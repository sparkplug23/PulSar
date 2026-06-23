#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# decode_serial_crashdump.py
#
# Decode pasted ESP32 serial crash dump using addr2line + firmware.elf
#
# Handles:
#   - full Guru Meditation dumps
#   - wrapped/split hex values, e.g. 0x3f\nfb26c0
#   - missing spaces between backtrace entries, e.g. 0x3ffb26c00x401282a3
#   - missing sections
#   - malformed serial captures
#
# Date Modified: 16May26

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple


# ============================================================
# >>> PASTE YOUR FULL SERIAL CRASH DUMP HERE <<<
# ============================================================

def _SerialCrashDumpBlock() -> str:
    return r"""
    

                                                                               
 Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandl
ed.
                                                                           
     
                                                                         
Core  1 register dump:
                                                        
PC      : 0x4012ffdb  PS      : 0x00060e30  A0      : 0x80130147  A1      : 0x3f
fb2710  
                                                                      
A2      : 0x00000000  A3      : 0x00000023  A4      : 0x3ffb398c  A5      : 0x00
000004  
                                                                      
A6      : 0x3ffb26b0  A7      : 0x00000008  A8      : 0x800d4228  A9      : 0x3f
fb26e0  
                                                                      
A10     : 0x00000001  A11     : 0x3ffb94f8  A12     : 0xffffffff  A13     : 0xff
ffffff  
                                                                      
A14     : 0x3ffc82e4  A15     : 0x00000009  SAR     : 0x0000001d  EXCCAUSE: 0x00
00001c  
                                                                      
EXCVADDR: 0x00000040  LBEG    : 0x4008d1d1  LEND    : 0x4008d1e1  LCOUNT  : 0xff
fffffd  
                                                                      
          
                                                                    
            
                                                                  
Backtrace:0x4012ffd8:0x3ffb27100x40130144:0x3ffb2730 0x400f1525:0x3ffb2750 0x400
f1688:0x3ffb2770 0x400f18bb:0x3ffb2790 0x400d4376:0x3ffb27b0 0x40128b2c:0x3ffb27
f0 0x4014500e:0x3ffb2820 
                                                     
                           
                                                   
                             
                                                 
                               
                                               
                                 
                          

                        


      
 
      

                                                 



""".strip()


# ============================================================
# CONFIGURATION
# ============================================================

ENV_NAME = "testbed_01__sensors_nextion"   # <<< change if needed
PROJECT_ROOT = None                   # None = auto-detect folder containing saved_compiles

DEDUPE_CONSECUTIVE_ONLY = True
INCLUDE_PC_REGISTER_IN_DECODE = True


# ============================================================
# Basic helpers
# ============================================================

def _find_project_root(start: Path) -> Path:
    p = start.resolve()

    for _ in range(14):
        if (p / "saved_compiles").is_dir():
            return p

        if p.parent == p:
            break

        p = p.parent

    raise FileNotFoundError("Could not locate project root containing 'saved_compiles'.")


def _normalise_serial_text(text: str) -> str:
    """
    Make serial dump parseable without destroying meaning.

    Main fixes:
      1. Remove whitespace inside broken 8-digit hex values:
           0x3f\nfb26c0 -> 0x3ffb26c0
           0x00\n00001c -> 0x0000001c

      2. Add spaces between adjacent hex tokens:
           0x3ffb26c00x401282a3 -> 0x3ffb26c0 0x401282a3

      3. Collapse excessive whitespace for easier regex parsing.
    """

    if not text:
        return ""

    s = text.replace("\r", "\n")

    # Repeatedly repair split hex tokens until stable.
    # Example: 0x401\n229f6 -> 0x401229f6
    split_hex_pattern = re.compile(
        r"0x([0-9a-fA-F]{1,7})\s+([0-9a-fA-F]{1,7})"
    )

    changed = True
    while changed:
        changed = False

        def _join_if_total_8(match: re.Match) -> str:
            nonlocal changed

            a = match.group(1)
            b = match.group(2)

            if len(a) + len(b) == 8:
                changed = True
                return "0x" + a + b

            return match.group(0)

        s_new = split_hex_pattern.sub(_join_if_total_8, s)
        s = s_new

    # Add separator between adjacent hex values.
    # Example: 0x3ffb26c00x401282a3
    s = re.sub(r"(0x[0-9a-fA-F]{8})(?=0x[0-9a-fA-F]{8})", r"\1 ", s)

    # Standardise whitespace.
    s = re.sub(r"[ \t]+", " ", s)
    s = re.sub(r"\n+", "\n", s)

    return s.strip()


def _extract_first_regex(pattern: str, text: str, flags: int = 0) -> Optional[str]:
    m = re.search(pattern, text, flags)
    if not m:
        return None
    return m.group(1)


def _hex_to_int_or_none(value: Optional[str]) -> Optional[int]:
    if not value:
        return None

    try:
        return int(value, 16)
    except ValueError:
        return None


def _fmt_hex(value: Optional[int]) -> str:
    if value is None:
        return "N/A"
    return f"0x{value:08x}"


def _dedupe_consecutive(values: List[int]) -> List[int]:
    if not values:
        return []

    out = [values[0]]

    for x in values[1:]:
        if x != out[-1]:
            out.append(x)

    return out


def _dedupe_all_keep_order(values: List[int]) -> List[int]:
    seen = set()
    out = []

    for x in values:
        if x in seen:
            continue

        seen.add(x)
        out.append(x)

    return out


# ============================================================
# Crash extraction
# ============================================================

def _extract_crash_summary(text: str) -> Dict[str, Optional[str]]:
    """
    Extract high-level crash fields.
    All fields are optional, script should continue if missing.
    """

    summary: Dict[str, Optional[str]] = {
        "guru_line": None,
        "core": None,
        "panic_reason": None,
        "exception_text": None,
        "pc": None,
        "exccause": None,
        "excvaddr": None,
    }

    guru_line = _extract_first_regex(
        r"(Guru Meditation Error:.*?)(?:\n|$)",
        text,
        re.IGNORECASE,
    )

    if guru_line:
        summary["guru_line"] = guru_line.strip()

        core = _extract_first_regex(
            r"Core\s+(\d+)\s+panic",
            guru_line,
            re.IGNORECASE,
        )
        reason = _extract_first_regex(
            r"panic'ed\s+\(([^)]+)\)",
            guru_line,
            re.IGNORECASE,
        )
        exception_text = _extract_first_regex(
            r"\)\.\s*(.*)$",
            guru_line,
            re.IGNORECASE,
        )

        summary["core"] = core
        summary["panic_reason"] = reason
        summary["exception_text"] = exception_text

    pc = _extract_first_regex(r"\bPC\s*:\s*(0x[0-9a-fA-F]{8})", text)
    exccause = _extract_first_regex(r"\bEXCCAUSE\s*:\s*(0x[0-9a-fA-F]{8})", text)
    excvaddr = _extract_first_regex(r"\bEXCVADDR\s*:\s*(0x[0-9a-fA-F]{8})", text)

    summary["pc"] = pc
    summary["exccause"] = exccause
    summary["excvaddr"] = excvaddr

    return summary


def _extract_registers(text: str) -> Dict[str, int]:
    """
    Extract register dump values.

    Returns any recognised register fields.
    Missing fields are ignored.
    """

    register_names = [
        "PC", "PS", "A0", "A1", "A2", "A3", "A4", "A5",
        "A6", "A7", "A8", "A9", "A10", "A11", "A12",
        "A13", "A14", "A15", "SAR", "EXCCAUSE", "EXCVADDR",
        "LBEG", "LEND", "LCOUNT",
    ]

    regs: Dict[str, int] = {}

    for name in register_names:
        m = re.search(rf"\b{name}\s*:\s*(0x[0-9a-fA-F]{{8}})", text)
        if not m:
            continue

        try:
            regs[name] = int(m.group(1), 16)
        except ValueError:
            pass

    return regs


def _extract_backtrace_pcs(text: str) -> List[int]:
    """
    Extract only PC-side addresses from the Backtrace section.

    Backtrace format:
      Backtrace:0xPC:0xSP 0xPC:0xSP ...

    Important:
      We do NOT want stack pointer addresses.
    """

    m = re.search(r"Backtrace\s*:\s*(.*)", text, re.IGNORECASE | re.DOTALL)
    if not m:
        return []

    backtrace_text = m.group(1)

    # Keep PC from PC:SP pairs.
    matches = re.findall(
        r"(0x[0-9a-fA-F]{8})\s*:\s*0x[0-9a-fA-F]{8}",
        backtrace_text,
    )

    pcs: List[int] = []

    for x in matches:
        try:
            pcs.append(int(x, 16))
        except ValueError:
            pass

    return pcs


def _classify_exception(summary: Dict[str, Optional[str]]) -> str:
    reason = summary.get("panic_reason") or ""
    excvaddr = _hex_to_int_or_none(summary.get("excvaddr"))

    if reason == "LoadProhibited":
        if excvaddr == 0:
            return "Likely NULL pointer read/dereference."
        return "Invalid memory read."

    if reason == "StoreProhibited":
        if excvaddr == 0:
            return "Likely NULL pointer write/dereference."
        return "Invalid memory write."

    if reason == "InstrFetchProhibited":
        return "CPU attempted to execute from an invalid function/code address."

    if reason == "IllegalInstruction":
        return "Illegal instruction, often bad function pointer, corrupted return address, or flash/code issue."

    if reason == "LoadStoreAlignment":
        return "Unaligned memory access."

    if reason == "LoadStoreError":
        return "Invalid load/store width or memory region access."

    return "No specific interpretation available."


# ============================================================
# addr2line helpers
# ============================================================

def _detect_arch_from_elf(elf_path: Path) -> str:
    """
    Detect architecture from ELF header.
    Returns:
      - xtensa
      - riscv
    """

    with elf_path.open("rb") as f:
        hdr = f.read(20)

    if len(hdr) < 20:
        raise RuntimeError(f"Invalid ELF header: {elf_path}")

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

    elif arch == "riscv":
        candidates = [
            home / ".platformio" / "packages" / "toolchain-riscv32-esp" / "bin" / "riscv32-esp-elf-addr2line.exe",
        ]
        exe_name = "riscv32-esp-elf-addr2line.exe"

    else:
        raise RuntimeError(f"Unsupported architecture: {arch}")

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

    except Exception as ex:
        return f"addr2line failed: {ex}"


# ============================================================
# Print helpers
# ============================================================

def _print_summary(summary: Dict[str, Optional[str]], regs: Dict[str, int]) -> None:
    print("Crash Summary:")
    print(f"  Guru Line   : {summary.get('guru_line') or 'N/A'}")
    print(f"  Core        : {summary.get('core') or 'N/A'}")
    print(f"  Panic       : {summary.get('panic_reason') or 'N/A'}")
    print(f"  Exception   : {summary.get('exception_text') or 'N/A'}")
    print(f"  PC          : {summary.get('pc') or 'N/A'}")
    print(f"  EXCCAUSE    : {summary.get('exccause') or 'N/A'}")
    print(f"  EXCVADDR    : {summary.get('excvaddr') or 'N/A'}")
    print(f"  Meaning     : {_classify_exception(summary)}")

    if regs:
        print("\nRegisters Found:")
        for name in [
            "PC", "PS", "A0", "A1", "A2", "A3", "A4", "A5",
            "A6", "A7", "A8", "A9", "A10", "A11", "A12",
            "A13", "A14", "A15", "SAR", "EXCCAUSE", "EXCVADDR",
            "LBEG", "LEND", "LCOUNT",
        ]:
            if name in regs:
                print(f"  {name:<8}: 0x{regs[name]:08x}")


def _print_decode_header(project_root: Path, elf_path: Path, arch: Optional[str], addr2line_exe: Optional[str]) -> None:
    print("\nDecode Config:")
    print(f"  ENV_NAME     : {ENV_NAME}")
    print(f"  PROJECT_ROOT : {project_root}")
    print(f"  FIRMWARE_ELF : {elf_path}")
    print(f"  ARCH         : {arch or 'N/A'}")
    print(f"  ADDR2LINE    : {addr2line_exe or 'N/A'}")


# ============================================================
# Main
# ============================================================

def main() -> int:
    raw = _SerialCrashDumpBlock()
    cleaned = _normalise_serial_text(raw)

    summary = _extract_crash_summary(cleaned)
    regs = _extract_registers(cleaned)
    backtrace_pcs = _extract_backtrace_pcs(cleaned)

    if DEDUPE_CONSECUTIVE_ONLY:
        backtrace_pcs = _dedupe_consecutive(backtrace_pcs)
    else:
        backtrace_pcs = _dedupe_all_keep_order(backtrace_pcs)

    decode_pcs: List[int] = []

    pc_reg = _hex_to_int_or_none(summary.get("pc"))

    if INCLUDE_PC_REGISTER_IN_DECODE and pc_reg is not None:
        decode_pcs.append(pc_reg)

    decode_pcs.extend(backtrace_pcs)

    if DEDUPE_CONSECUTIVE_ONLY:
        decode_pcs = _dedupe_consecutive(decode_pcs)
    else:
        decode_pcs = _dedupe_all_keep_order(decode_pcs)

    print("------------------------------------------------------------")
    print("ESP32 Serial Crash Dump Decoder")
    print("------------------------------------------------------------\n")

    print("Sanitised Crash Snippet:")
    print(cleaned)
    print("\n------------------------------------------------------------\n")

    _print_summary(summary, regs)

    print("\nBacktrace PCs:")
    if backtrace_pcs:
        for i, pc in enumerate(backtrace_pcs):
            print(f"  BT[{i:02d}] 0x{pc:08x}")
    else:
        print("  No Backtrace PCs found.")

    print("\nDecode PCs:")
    if decode_pcs:
        for i, pc in enumerate(decode_pcs):
            label = "PC" if i == 0 and pc == pc_reg else f"{i:02d}"
            print(f"  [{label}] 0x{pc:08x}")
    else:
        print("  No decode PCs found.")

    # Continue without failing if decode material is missing.
    if not decode_pcs:
        print("\nNo addresses available for addr2line decode.")
        return 0

    try:
        script_path = Path(__file__).resolve()
        project_root = Path(PROJECT_ROOT).resolve() if PROJECT_ROOT else _find_project_root(script_path)

        elf_path = project_root / "saved_compiles" / ENV_NAME / "firmware.elf"
        if not elf_path.is_file():
            print(f"\nWARNING: Missing firmware ELF: {elf_path}")
            print("Crash parsing completed, but addr2line decode was skipped.")
            return 0

        arch = _detect_arch_from_elf(elf_path)
        addr2line_exe = _auto_find_addr2line(arch)

        _print_decode_header(project_root, elf_path, arch, addr2line_exe)

        print("\nDecoded:")
        for pc in decode_pcs:
            line = _addr2line(addr2line_exe, elf_path, pc)
            print(f"  0x{pc:08x}: {line}")

    except Exception as ex:
        print(f"\nWARNING: addr2line decode skipped due to error: {ex}")
        print("Crash parsing still completed successfully.")
        return 0

    return 0


if __name__ == "__main__":
    sys.exit(main())



    
# ============================================================
# >>> EXAMPLE YOUR FULL SERIAL CRASH DUMP HERE <<<
# ============================================================

def _SerialCrashDumpBlock_Example() -> str:
    return r"""
Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandle
d.

Core  1 register dump:

PC      : 0x40127e06  PS      : 0x00060430  A0      : 0x801282a6  A1      : 0x3f
fb26c0

A2      : 0x00000000  A3      : 0x00001f40  A4      : 0x000000a5  A5      : 0x00
00000e

A6      : 0x00000028  A7      : 0x00000009  A8      : 0x80127e04  A9      : 0x3f
fb26a0

A10     : 0x3ffb462c  A11     : 0x00001f40  A12     : 0x3ffb2670  A13     : 0x00
000004

A14     : 0x3ffc82ac  A15     : 0x3ffb260c  SAR     : 0x00000011  EXCCAUSE: 0x00
00001c

EXCVADDR: 0x00000000  LBEG    : 0x4008d1a5  LEND    : 0x4008d1b5  LCOUNT  : 0xff
fffff9

Backtrace:0x40127e03:0x3ffb26c00x401282a3:0x3ffb26f0 0x400d423b:0x3ffb2710 0x401
229f6:0x3ffb2750 0x4012331e:0x3ffb2770 0x400d423b:0x3ffb2790 0x401293af:0x3ffb27
d0 0x40146166:0x3ffb2820
""".strip()
