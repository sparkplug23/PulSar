#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# decode_serial_exception_esp8266.py
#
# Decode pasted ESP8266 serial exception dump using xtensa-lx106-elf-addr2line + firmware.elf
#
# Handles:
#   - ESP8266 "CUT HERE FOR EXCEPTION DECODER" dumps
#   - epc1/epc2/epc3/excvaddr/depc register line
#   - >>>stack>>> / <<<stack<<< blocks
#   - wrapped/split 8-digit hex values caused by serial copy/paste
#   - duplicated or malformed whitespace
#
# Date Modified: 26May26

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple


# ============================================================
# >>> PASTE YOUR FULL ESP8266 SERIAL EXCEPTION DUMP HERE <<<
# ============================================================

def _SerialCrashDumpBlock() -> str:
    return r"""
                                 
                  --------------- CUT HERE FOR EXCEPTION DECODER ---------------

               
                                                                                
Exception (3):
                                                                
epc1=0x4000bf64 epc2=0x00000000 epc3=0x00000000 excvaddr=0x4027f7b9 depc=0x00000
000

                                                                         
                                                                                
>>>stack>>>

                                                                 
                                                                                
ctx: cont
                                                                     
sp: 3ffffb70 end: 3fffffc0 offset: 0190
                                       
3ffffd00:  3ffffd5c 402329c0 3ffe9c84 0000005a  
                              
3ffffd10:  4027f7b9 3fff4271 00000020 40100f73  
                              
3ffffd20:  402329c0 00000000 3fff8474 40216b94  
                              
3ffffd30:  3fff4200 000c001f 3ffffd50 4022c706  
                              
3ffffd40:  3ffe9c00 3fff8b24 00ff1b23 402129c2  
                              
3ffffd50:  3fff9f00 000c000f 00ff8b24 3fff8e34  
                              
3ffffd60:  0000005a 00000001 3fff8474 40216ea5  
                              
3ffffd70:  3ffe9c84 3fff8b24 3fff1b23 40212a1c  
                              
3ffffd80:  3ffe9c84 3fff8d9c 00000bbb 40212542  
                              
3ffffd90:  00000000 3fff8d9c 0000005a 40216075  
                              
3ffffda0:  0000005a 00000000 3fff4adc 3fff8e34  
                              
3ffffdb0:  0000005a 00000001 3fff8e34 4020560d  
                              
3ffffdc0:  3fff4364 3fff93f4 3fff4364 40229afe  
                              
3ffffdd0:  000c000f 3fff93f4 3ffe9c84 40216445  
                              
3ffffde0:  0000005a 3fff8b24 00ff1b23 3fff4200  
                              
3ffffdf0:  001c0000 000c000f 00ff8474 3fff8e34  
                              
3ffffe00:  3fff1a6c 3ffe96a2 3fff8474 40216d72  
                              
3ffffe10:  3fff4200 000c001f 3ffffe30 4022c706  
                              
3ffffe20:  3ffe0000 3fff8b24 00ff1b23 3fff4200  
                              
3ffffe30:  001c001f 000c000f 00ff8b24 402329b0  
                              
3ffffe40:  000000a8 00000001 3fff8474 40216e9a  
                              
3ffffe50:  3ffe9c84 3fff8b24 3fff1b23 40212a1c  
                              
3ffffe60:  3ffe9c84 3fff8d9c 00000bbb 3fff8e34  
                              
3ffffe70:  3ffe9c84 3fff8d9c 000000a8 402160dd  
                              
3ffffe80:  000000a8 00000000 3fff8b24 3fff8e34  
                              
3ffffe90:  000000a8 00000001 3fff8e34 4020560d  
                              
3ffffea0:  402329c0 3fff8d24 3fff8dec 40205497  
                              
3ffffeb0:  3fff07d1 3ffffec0 3ffe9c84 4022c706  
                              
3ffffec0:  000000a8 3fff8d24 3ffe9706 3fff8e34  
                              
3ffffed0:  0000001c 3fff8d24 3fff4a9c 3fff8e34  
                              
3ffffee0:  0000001c 3fff8d24 3fff8b24 40213402  
                              
3ffffef0:  0000001c 3fff8d24 3fff8b24 4021349d  
                              
3fffff00:  0000001c 3fff8d24 3fff8b24 40212cb8  
                              
3fffff10:  0000001c 00000000 3fff8b24 40212eae  
                              
3fffff20:  0000001c 00000000 3fff8e2c 4020560d  
                              
3fffff30:  00000000 00000000 3fff8dfc 40205497  
                              
3fffff40:  0053b5f2 00000000 3ffe9c84 00004865  
                              
3fffff50:  0000001c 3fffff60 00000008 3fff1f64  
                              
3fffff60:  3fffdad0 000003e8 3fff80e8 3fff1f64  
                              
3fffff70:  3fffdad0 00000000 3fff80d4 40218ff3  
                              
3fffff80:  3fffdad0 00000000 3fff80d4 40219226  
                              
3fffff90:  3fffdad0 00000000 3fff1f24 3fff1f64  
                              
3fffffa0:  3fffdad0 00000000 3fff1f24 4022da94  
                              
3fffffb0:  feefeffe feefeffe 3ffe85e0 40100375  
                              
<<<stack<<<
                                                                   

                                                                               
 --------------- CUT HERE FOR EXCEPTION DECODER ---------------
      






""".strip()


# ============================================================
# CONFIGURATION
# ============================================================

ENV_NAME = "valid__esp8266__minimal__nodemcu_withfs1m"   # PlatformIO environment name
PROJECT_ROOT = None                           # None = auto-detect folder containing saved_compiles

DEDUPE_CONSECUTIVE_ONLY = False
INCLUDE_EPC_REGISTERS_IN_DECODE = True
INCLUDE_STACK_CODE_ADDRESSES_IN_DECODE = True

# Decode only likely ESP8266 code addresses from the stack.
# 0x402xxxxx = flash mapped code
# 0x401xxxxx = IRAM code
# 0x400xxxxx = ROM/SDK region, usually not useful with your firmware. Disabled by default.
DECODE_FLASH_CODE = True
DECODE_IRAM_CODE = True
DECODE_ROM_CODE = False


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
    Make ESP8266 serial dumps parseable without destroying meaning.

    Main fixes:
      1. Remove whitespace inside broken 8-digit hex values:
           0x00000\n000 -> 0x00000000
           402\n17ccd    -> 40217ccd

      2. Add predictable spacing around stack markers.

      3. Collapse excessive whitespace without flattening stack rows.
    """

    if not text:
        return ""

    s = text.replace("\r", "\n")

    # Repair 0x-prefixed split hex values, e.g. 0x00000\n000 -> 0x00000000.
    split_0x_pattern = re.compile(
        r"0x([0-9a-fA-F]{1,7})\s+([0-9a-fA-F]{1,7})"
    )

    changed = True
    while changed:
        changed = False

        def _join_0x_if_total_8(match: re.Match) -> str:
            nonlocal changed
            a = match.group(1)
            b = match.group(2)

            if len(a) + len(b) == 8:
                changed = True
                return "0x" + a + b

            return match.group(0)

        s = split_0x_pattern.sub(_join_0x_if_total_8, s)

    # Repair bare split stack values, e.g. 402\n17ccd -> 40217ccd.
    # Keep this conservative: only join when total is exactly 8 hex chars.
    split_bare_pattern = re.compile(
        r"(?<![0-9a-fA-Fx])([0-9a-fA-F]{1,7})\s+([0-9a-fA-F]{1,7})(?![0-9a-fA-F])"
    )

    changed = True
    while changed:
        changed = False

        def _join_bare_if_total_8(match: re.Match) -> str:
            nonlocal changed
            a = match.group(1)
            b = match.group(2)

            if len(a) + len(b) == 8:
                changed = True
                return a + b

            return match.group(0)

        s = split_bare_pattern.sub(_join_bare_if_total_8, s)

    # Ensure adjacent 0x values are separated.
    s = re.sub(r"(0x[0-9a-fA-F]{8})(?=0x[0-9a-fA-F]{8})", r"\1 ", s)

    # Strip trailing spaces from each line and collapse excessive blank lines.
    lines = [line.strip() for line in s.split("\n")]
    s = "\n".join(lines)
    s = re.sub(r"\n{3,}", "\n\n", s)

    return s.strip()


def _extract_first_regex(pattern: str, text: str, flags: int = 0) -> Optional[str]:
    m = re.search(pattern, text, flags)
    if not m:
        return None
    return m.group(1)


def _hex_to_int_or_none(value: Optional[str]) -> Optional[int]:
    if not value:
        return None

    value = value.strip()

    try:
        if value.lower().startswith("0x"):
            return int(value, 16)
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
# ESP8266 crash extraction
# ============================================================

def _extract_exception_number(text: str) -> Optional[int]:
    x = _extract_first_regex(r"\bException\s*\((\d+)\)\s*:", text, re.IGNORECASE)
    if x is None:
        return None

    try:
        return int(x)
    except ValueError:
        return None


def _extract_esp8266_registers(text: str) -> Dict[str, int]:
    """
    Extract ESP8266 exception registers:
      epc1 epc2 epc3 excvaddr depc
    """

    regs: Dict[str, int] = {}

    for name in ["epc1", "epc2", "epc3", "excvaddr", "depc"]:
        m = re.search(rf"\b{name}\s*=\s*(0x[0-9a-fA-F]{{8}})", text, re.IGNORECASE)
        if not m:
            continue

        value = _hex_to_int_or_none(m.group(1))
        if value is not None:
            regs[name.lower()] = value

    return regs


def _extract_stack_metadata(text: str) -> Dict[str, Optional[str]]:
    meta: Dict[str, Optional[str]] = {
        "ctx": None,
        "sp": None,
        "end": None,
        "offset": None,
    }

    ctx = _extract_first_regex(r"\bctx\s*:\s*([A-Za-z0-9_]+)", text, re.IGNORECASE)
    if ctx:
        meta["ctx"] = ctx

    m = re.search(
        r"\bsp\s*:\s*([0-9a-fA-F]{8})\s+end\s*:\s*([0-9a-fA-F]{8})\s+offset\s*:\s*([0-9a-fA-F]{4})",
        text,
        re.IGNORECASE,
    )

    if m:
        meta["sp"] = "0x" + m.group(1).lower()
        meta["end"] = "0x" + m.group(2).lower()
        meta["offset"] = "0x" + m.group(3).lower()

    return meta


def _extract_stack_block(text: str) -> str:
    m = re.search(r">>>stack>>>(.*?)<<<stack<<<", text, re.IGNORECASE | re.DOTALL)
    if not m:
        return ""

    return m.group(1)


def _extract_stack_words(text: str) -> List[Tuple[int, int]]:
    """
    Extract stack words.

    Returns list of:
      (stack_address, value)

    Example input row:
      3fffff60:  00000000 40217ccd 3fff93d4 402071ab
    """

    stack_block = _extract_stack_block(text)
    if not stack_block:
        return []

    words: List[Tuple[int, int]] = []

    for line in stack_block.splitlines():
        m = re.match(r"^\s*([0-9a-fA-F]{8})\s*:\s*(.*)$", line)
        if not m:
            continue

        try:
            row_address = int(m.group(1), 16)
        except ValueError:
            continue

        row_values = re.findall(r"\b([0-9a-fA-F]{8})\b", m.group(2))

        for i, raw_value in enumerate(row_values):
            try:
                value = int(raw_value, 16)
            except ValueError:
                continue

            word_address = row_address + (i * 4)
            words.append((word_address, value))

    return words


def _is_probable_code_address(value: int) -> bool:
    if DECODE_FLASH_CODE and 0x40200000 <= value <= 0x40300000:
        return True

    if DECODE_IRAM_CODE and 0x40100000 <= value <= 0x40200000:
        return True

    if DECODE_ROM_CODE and 0x40000000 <= value <= 0x40100000:
        return True

    return False


def _extract_stack_code_addresses(text: str) -> List[int]:
    words = _extract_stack_words(text)
    return [value for _, value in words if _is_probable_code_address(value)]


def _exception_meaning(exception_number: Optional[int], regs: Dict[str, int]) -> str:
    """
    ESP8266 exception cause quick interpretation.

    Exception (3) is LoadStoreError on Xtensa LX106 in common ESP8266 decoder output.
    Keep this intentionally practical rather than pretending every cause is certain from text alone.
    """

    excvaddr = regs.get("excvaddr")

    if exception_number is None:
        return "No ESP8266 exception number found."

    if exception_number == 0:
        return "Illegal instruction, often corrupted function pointer, bad return address, or invalid code execution."

    if exception_number == 3:
        if excvaddr == 0:
            return "Load/store error with null address involvement."
        return "Load/store error. Often invalid memory access, bad pointer, or access to an invalid region."

    if exception_number == 9:
        return "Unaligned memory access."

    if exception_number == 28:
        if excvaddr == 0:
            return "LoadProhibited-style invalid read, likely null pointer dereference."
        return "LoadProhibited-style invalid read."

    if exception_number == 29:
        if excvaddr == 0:
            return "StoreProhibited-style invalid write, likely null pointer dereference."
        return "StoreProhibited-style invalid write."

    return "ESP8266 exception cause extracted, but no specific interpretation is mapped in this script."


# ============================================================
# addr2line helpers
# ============================================================

def _auto_find_esp8266_addr2line() -> str:
    """
    Find xtensa-lx106-elf-addr2line from PlatformIO or PATH.
    """

    home = Path.home()

    exe_names = [
        "xtensa-lx106-elf-addr2line.exe",
        "xtensa-lx106-elf-addr2line",
    ]

    candidate_dirs = [
        home / ".platformio" / "packages" / "toolchain-xtensa" / "bin",
        home / ".platformio" / "packages" / "toolchain-xtensa-lx106" / "bin",
        home / ".platformio" / "packages" / "toolchain-xtensa-esp8266" / "bin",
    ]

    for d in candidate_dirs:
        for exe_name in exe_names:
            c = d / exe_name
            if c.is_file():
                return str(c)

    for p in os.environ.get("PATH", "").split(os.pathsep):
        for exe_name in exe_names:
            cand = Path(p) / exe_name
            if cand.is_file():
                return str(cand)

    raise FileNotFoundError("Could not find xtensa-lx106-elf-addr2line in PlatformIO packages or PATH.")


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

def _print_summary(exception_number: Optional[int], regs: Dict[str, int], stack_meta: Dict[str, Optional[str]]) -> None:
    print("ESP8266 Exception Summary:")
    print(f"  Exception   : {exception_number if exception_number is not None else 'N/A'}")
    print(f"  Meaning     : {_exception_meaning(exception_number, regs)}")

    print("\nRegisters:")
    for name in ["epc1", "epc2", "epc3", "excvaddr", "depc"]:
        print(f"  {name:<8}: {_fmt_hex(regs.get(name))}")

    print("\nStack Metadata:")
    print(f"  ctx         : {stack_meta.get('ctx') or 'N/A'}")
    print(f"  sp          : {stack_meta.get('sp') or 'N/A'}")
    print(f"  end         : {stack_meta.get('end') or 'N/A'}")
    print(f"  offset      : {stack_meta.get('offset') or 'N/A'}")


def _print_decode_header(project_root: Path, elf_path: Path, addr2line_exe: str) -> None:
    print("\nDecode Config:")
    print(f"  ENV_NAME     : {ENV_NAME}")
    print(f"  PROJECT_ROOT : {project_root}")
    print(f"  FIRMWARE_ELF : {elf_path}")
    print(f"  ADDR2LINE    : {addr2line_exe}")


# ============================================================
# Main
# ============================================================

def main() -> int:
    raw = _SerialCrashDumpBlock()
    cleaned = _normalise_serial_text(raw)

    exception_number = _extract_exception_number(cleaned)
    regs = _extract_esp8266_registers(cleaned)
    stack_meta = _extract_stack_metadata(cleaned)
    stack_words = _extract_stack_words(cleaned)
    stack_code_addresses = _extract_stack_code_addresses(cleaned)

    decode_pcs: List[int] = []

    if INCLUDE_EPC_REGISTERS_IN_DECODE:
        for name in ["epc1", "epc2", "epc3", "depc"]:
            value = regs.get(name)

            if value is None:
                continue

            if value == 0:
                continue

            if not _is_probable_code_address(value):
                # Keep epc/depc printed in the register summary, but avoid noisy addr2line decode.
                continue

            decode_pcs.append(value)

    if INCLUDE_STACK_CODE_ADDRESSES_IN_DECODE:
        decode_pcs.extend(stack_code_addresses)

    if DEDUPE_CONSECUTIVE_ONLY:
        decode_pcs = _dedupe_consecutive(decode_pcs)
    else:
        decode_pcs = _dedupe_all_keep_order(decode_pcs)

    print("------------------------------------------------------------")
    print("ESP8266 Serial Exception Decoder")
    print("------------------------------------------------------------\n")

    print("Sanitised Crash Snippet:")
    print(cleaned)
    print("\n------------------------------------------------------------\n")

    _print_summary(exception_number, regs, stack_meta)

    print("\nStack Words:")
    if stack_words:
        for stack_address, value in stack_words:
            marker = " CODE" if _is_probable_code_address(value) else ""
            print(f"  0x{stack_address:08x}: 0x{value:08x}{marker}")
    else:
        print("  No stack words found.")

    print("\nDecode PCs:")
    if decode_pcs:
        for i, pc in enumerate(decode_pcs):
            print(f"  [{i:02d}] 0x{pc:08x}")
    else:
        print("  No likely firmware code addresses found for addr2line decode.")

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

        addr2line_exe = _auto_find_esp8266_addr2line()

        _print_decode_header(project_root, elf_path, addr2line_exe)

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
