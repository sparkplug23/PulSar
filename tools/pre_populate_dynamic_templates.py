# tools/pre_populate_dynamic_templates.py
#
# Minimal dynamic template pre-build generator.
#
# Purpose:
#   Take active-device DEF_TEMPLATE__* macros, replace %TOKEN% placeholders
#   using G1_mUserConfig_Secret.h and PlatformIO build flags, then emit the
#   final legacy-compatible PROGMEM const symbols:
#
#     DEF_TEMPLATE__NETWORK_CONFIG -> NETWORK_TEMPLATE
#     DEF_TEMPLATE__MODULE         -> MODULE_TEMPLATE
#     DEF_TEMPLATE__FUNCTION       -> FUNCTION_TEMPLATE
#     DEF_TEMPLATE__RULES          -> RULES_TEMPLATE
#     DEF_TEMPLATE__LIGHTING       -> LIGHTING_TEMPLATE
#     DEF_TEMPLATE__DISPLAY        -> DISPLAY_TEMPLATE
#
# Notes:
#   - This is not a general C preprocessor.
#   - Only active #ifdef DEVICE_* blocks are scanned.
#   - DEF_TEMPLATE__* must be adjacent C/C++ string literals.
#   - Secret header values are read from simple #define lines.
#   - PlatformIO build_flags / CPPDEFINES override secret values.

Import("env")

import os
import re


# -----------------------------------------------------------------------------
# Paths
# -----------------------------------------------------------------------------

PROJECT_DIR = env.subst("$PROJECT_DIR")

SECRET_HEADER = os.path.join(
    PROJECT_DIR,
    "code",
    "0_ConfigUser",
    "G1_mUserConfig_Secret.h"
)

CONFIG_SCAN_ROOT = os.path.join(
    PROJECT_DIR,
    "code",
    "0_ConfigUser"
)

GENERATED_DIR = os.path.join(
    PROJECT_DIR,
    "code",
    "0_ConfigUser",
    "99_Generated"
)

GENERATED_HEADER = os.path.join(
    GENERATED_DIR,
    "Generated_DynamicTemplates.h"
)


# -----------------------------------------------------------------------------
# Regex
# -----------------------------------------------------------------------------

DEFINE_RE = re.compile(
    r'^\s*#define\s+([A-Za-z_][A-Za-z0-9_]*)\s*(.*?)\s*$'
)

TOKEN_RE = re.compile(
    r'%([A-Za-z_][A-Za-z0-9_]*)%'
)

TEMPLATE_DEFINE_START_RE = re.compile(
    r'^\s*#define\s+(DEF_TEMPLATE__[A-Za-z_][A-Za-z0-9_]*)\b(.*)$'
)


# -----------------------------------------------------------------------------
# Value / define parsing
# -----------------------------------------------------------------------------

def clean_define_value(value):
    value = str(value).strip()

    # Remove simple trailing C++ comments.
    value = re.sub(r'\s+//.*$', '', value).strip()

    # Remove wrapping parentheses.
    if len(value) >= 2 and value[0] == "(" and value[-1] == ")":
        value = value[1:-1].strip()

    # Strip normal C string quotes.
    if len(value) >= 2 and value[0] == '"' and value[-1] == '"':
        value = value[1:-1]

    # Strip PlatformIO escaped quotes.
    if len(value) >= 4 and value.startswith('\\"') and value.endswith('\\"'):
        value = value[2:-2]

    return value


def read_simple_defines_from_file(path):
    symbols = {}

    if not os.path.exists(path):
        return symbols

    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            m = DEFINE_RE.match(line)
            if not m:
                continue

            key = m.group(1)
            value = m.group(2)

            if value == "":
                symbols[key] = "1"
            else:
                symbols[key] = clean_define_value(value)

    return symbols


def read_cppdefines_from_env():
    symbols = {}

    # Already populated CPPDEFINES.
    for item in env.get("CPPDEFINES", []):
        if isinstance(item, tuple):
            key = str(item[0])
            value = clean_define_value(item[1])
            symbols[key] = value
        else:
            symbols[str(item)] = "1"

    # Raw build_flags, needed because pre scripts can run before CPPDEFINES is
    # fully normalised.
    try:
        build_flags = env.GetProjectOption("build_flags", "")
    except Exception:
        build_flags = ""

    if isinstance(build_flags, list):
        build_flags_text = "\n".join(build_flags)
    else:
        build_flags_text = str(build_flags)

    cleaned_lines = []

    for line in build_flags_text.splitlines():
        line = line.split(";", 1)[0].strip()

        if line:
            cleaned_lines.append(line)

    build_flags_text = " ".join(cleaned_lines)

    define_re = re.compile(
        r'(?:^|\s)-D\s*([A-Za-z_][A-Za-z0-9_]*)(?:=("[^"]*"|\\"[^"]*\\"|[^\s]+))?'
    )

    for m in define_re.finditer(build_flags_text):
        key = m.group(1)
        value = m.group(2)

        if value is None:
            symbols[key] = "1"
        else:
            symbols[key] = clean_define_value(value)

    return symbols


def find_active_device_define(symbols):
    device_defs = []

    for key, value in symbols.items():
        if not key.startswith("DEVICE_"):
            continue

        if str(value) in ("0", "false", "False", "FALSE"):
            continue

        device_defs.append(key)

    if len(device_defs) == 0:
        return None

    if len(device_defs) > 1:
        print("WARNING: multiple DEVICE_* defines found, using first:")
        for item in device_defs:
            print("  " + item)

    return device_defs[0]


# -----------------------------------------------------------------------------
# Active device block extraction
# -----------------------------------------------------------------------------

def is_if_start(line):
    return re.match(r'^\s*#\s*(if|ifdef|ifndef)\b', line) is not None


def is_endif(line):
    return re.match(r'^\s*#\s*endif\b', line) is not None


def is_active_device_ifdef(line, active_device_define):
    pattern = r'^\s*#\s*ifdef\s+' + re.escape(active_device_define) + r'\b'
    return re.match(pattern, line) is not None


def extract_active_device_blocks_from_file(path, active_device_define):
    blocks = []

    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        lines = f.readlines()

    i = 0

    while i < len(lines):
        if not is_active_device_ifdef(lines[i], active_device_define):
            i += 1
            continue

        start = i
        depth = 0
        block = []

        while i < len(lines):
            line = lines[i]

            if is_if_start(line):
                depth += 1

            block.append(line)

            if is_endif(line):
                depth -= 1

                if depth == 0:
                    break

            i += 1

        blocks.append({
            "path": path,
            "start_line": start + 1,
            "text": "".join(block)
        })

        i += 1

    return blocks


# -----------------------------------------------------------------------------
# Template macro extraction
# -----------------------------------------------------------------------------

def line_continues(line):
    return line.rstrip("\r\n").endswith("\\")


def remove_macro_line_slash(line):
    stripped = line.rstrip("\r\n")

    if stripped.endswith("\\"):
        stripped = stripped[:-1]

    return stripped.rstrip()


def collect_template_macros_from_text(text, path, base_line_number):
    templates = {}
    lines = text.splitlines(True)

    i = 0

    while i < len(lines):
        line = lines[i]
        m = TEMPLATE_DEFINE_START_RE.match(line)

        if not m:
            i += 1
            continue

        macro_name = m.group(1)
        start_line = base_line_number + i

        macro_lines = []
        macro_lines.append(remove_macro_line_slash(line))

        while line_continues(lines[i]) and (i + 1) < len(lines):
            i += 1
            macro_lines.append(remove_macro_line_slash(lines[i]))

        templates[macro_name] = {
            "path": path,
            "line": start_line,
            "text": "\n".join(macro_lines)
        }

        i += 1

    return templates


def collect_template_macros_for_active_device(root, active_device_define):
    templates = {}

    for dirpath, _, filenames in os.walk(root):
        if "99_Generated" in dirpath:
            continue

        for filename in filenames:
            if not filename.endswith((".h", ".hpp")):
                continue

            path = os.path.join(dirpath, filename)

            active_blocks = extract_active_device_blocks_from_file(
                path,
                active_device_define
            )

            for block in active_blocks:
                found = collect_template_macros_from_text(
                    block["text"],
                    path,
                    block["start_line"]
                )

                for name, data in found.items():
                    if name in templates:
                        print("WARNING: duplicate active template macro found:")
                        print("  " + name)
                        print("  old: " + templates[name]["path"])
                        print("  new: " + data["path"])

                    templates[name] = data

    return templates


def extract_macro_body(full_macro_text, macro_name):
    lines = full_macro_text.splitlines()

    if len(lines) == 0:
        return ""

    first_line = lines[0].strip()
    prefix = "#define " + macro_name

    if first_line.startswith(prefix):
        first_line = first_line[len(prefix):].strip()
    else:
        first_line = ""

    body_lines = []

    if first_line:
        body_lines.append(first_line)

    if len(lines) > 1:
        body_lines.extend(lines[1:])

    return "\n".join(body_lines).strip()


def unescape_cpp_adjacent_string_macro_body(body):
    # Extracts only C/C++ string literal fragments and concatenates them.
    string_parts = re.findall(r'"((?:\\.|[^"\\])*)"', body, flags=re.DOTALL)

    out = ""

    for part in string_parts:
        out += bytes(part, "utf-8").decode("unicode_escape")

    return out


# -----------------------------------------------------------------------------
# Token replacement
# -----------------------------------------------------------------------------

def replace_tokens(template_text, symbols):
    missing = set()

    def repl(match):
        token = match.group(1)

        if token == "device":
            value = (
                symbols.get("device") or
                symbols.get("DEVICENAME_CTR") or
                symbols.get("DEVICENAME_FRIENDLY_CTR") or
                ""
            )
        else:
            value = symbols.get(token, "")

        if value == "":
            missing.add(token)

        return str(value)

    return TOKEN_RE.sub(repl, template_text), sorted(missing)


# -----------------------------------------------------------------------------
# C++ output
# -----------------------------------------------------------------------------

def escape_cpp_string_line(line):
    line = line.replace("\\", "\\\\")
    line = line.replace("\"", "\\\"")
    return line


def template_to_symbol_guard(macro_name):
    suffix = macro_name.replace("DEF_TEMPLATE__", "", 1)
    return "TEMPLATE_SYMBOL__" + suffix + "__DEFINED"


def template_to_output_symbol(macro_name):
    suffix = macro_name.replace("DEF_TEMPLATE__", "", 1)

    mapping = {
        "NETWORK_CONFIG": "NETWORK_TEMPLATE",
        "MODULE": "MODULE_TEMPLATE",
        "FUNCTION": "FUNCTION_TEMPLATE",
        "RULES": "RULES_TEMPLATE",
        "LIGHTING": "LIGHTING_TEMPLATE",
        "DISPLAY": "DISPLAY_TEMPLATE",
    }

    if suffix in mapping:
        return mapping[suffix]

    return suffix + "_TEMPLATE"


def make_progmem_const(symbol_name, text):
    # Keep output compact: one escaped C++ string literal.
    escaped = escape_cpp_string_line(text)
    return "const char " + symbol_name + "[] PROGMEM =\n\"" + escaped + "\";"


# -----------------------------------------------------------------------------
# Main
# -----------------------------------------------------------------------------

symbols = {}

# Defaults only where useful.
symbols.update({
    "MQTT_PORT": "1883",
    "MQTT_PORT_CELLULAR": "1883",
})

# Secret values.
symbols.update(read_simple_defines_from_file(SECRET_HEADER))

# Env/build_flags overrides.
symbols.update(read_cppdefines_from_env())

print("Detected DEVICE_* symbols:")
for key in sorted(symbols.keys()):
    if key.startswith("DEVICE_"):
        print("  " + key + " = " + str(symbols[key]))

active_device_define = find_active_device_define(symbols)

os.makedirs(GENERATED_DIR, exist_ok=True)

out = []
out.append("// -----------------------------------------------------------------------------")
out.append("// Auto-generated by tools/pre_populate_dynamic_templates.py")
out.append("// Do not edit manually.")
out.append("// -----------------------------------------------------------------------------")
out.append("#pragma once")
out.append("")

templates = {}

if active_device_define is None:
    print("No active DEVICE_* define found. Dynamic template generation skipped.")
    out.append("// No active DEVICE_* define found.")
else:
    print("Active device define:")
    print("  " + active_device_define)

    templates = collect_template_macros_for_active_device(
        CONFIG_SCAN_ROOT,
        active_device_define
    )

    out.append("// Active device: " + active_device_define)
    out.append("")

    if len(templates) == 0:
        out.append("// No active DEF_TEMPLATE__* macros found.")
    else:
        for macro_name in sorted(templates.keys()):
            data = templates[macro_name]

            body = extract_macro_body(data["text"], macro_name)
            template_json = unescape_cpp_adjacent_string_macro_body(body)
            populated, missing = replace_tokens(template_json, symbols)

            rel_path = os.path.relpath(data["path"], PROJECT_DIR).replace("\\", "/")

            output_symbol = template_to_output_symbol(macro_name)
            guard_symbol = template_to_symbol_guard(macro_name)

            out.append("// Source: " + rel_path + ":" + str(data["line"]))

            if missing:
                out.append("// Missing tokens: " + ", ".join(missing))

            out.append("#ifndef " + guard_symbol)
            out.append("  #define " + guard_symbol)
            out.append(make_progmem_const(output_symbol, populated))
            out.append("#endif")
            out.append("")

with open(GENERATED_HEADER, "w", encoding="utf-8", newline="\n") as f:
    f.write("\n".join(out))

print("Generated dynamic template header:")
print("  " + GENERATED_HEADER)

print("Templates found:")
for name in sorted(templates.keys()):
    print("  " + name)