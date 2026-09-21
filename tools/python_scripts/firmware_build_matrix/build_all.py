from pathlib import Path
import subprocess
import sys
import time
from datetime import datetime


SCRIPT_DIR = Path(__file__).resolve().parent
PROJECT_ROOT = SCRIPT_DIR.parents[2]

TARGETS_FILE = SCRIPT_DIR / "build_targets.c"

REPORT_ROOT = SCRIPT_DIR / "build_reports"

TIMESTAMP = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
REPORT_DIR = REPORT_ROOT / TIMESTAMP

FULL_LOG_FILE = REPORT_DIR / "build_full.log"
SUMMARY_FILE = REPORT_DIR / "build_summary.md"


def strip_block_comments(text):
    output = []
    index = 0
    in_block_comment = False

    while index < len(text):
        if not in_block_comment and text.startswith("/*", index):
            in_block_comment = True
            index += 2
            continue

        if in_block_comment and text.startswith("*/", index):
            in_block_comment = False
            index += 2
            continue

        if in_block_comment:
            if text[index] == "\n":
                output.append("\n")

            index += 1
            continue

        output.append(text[index])
        index += 1

    return "".join(output)


def strip_line_comments(line):
    comment_positions = []

    slash_index = line.find("//")
    if slash_index >= 0:
        comment_positions.append(slash_index)

    semicolon_index = line.find(";")
    if semicolon_index >= 0:
        comment_positions.append(semicolon_index)

    if comment_positions:
        line = line[:min(comment_positions)]

    return line.strip()


def load_targets():
    targets = []

    if not TARGETS_FILE.exists():
        raise FileNotFoundError(f"Target list not found: {TARGETS_FILE}")

    raw_text = TARGETS_FILE.read_text(encoding="utf-8")

    text_without_block_comments = strip_block_comments(raw_text)

    for raw_line in text_without_block_comments.splitlines():
        line = strip_line_comments(raw_line)

        if not line:
            continue

        targets.append(line)

    return targets


def format_duration(seconds):
    total_ms = int(seconds * 1000)

    hours = total_ms // 3600000
    total_ms %= 3600000

    minutes = total_ms // 60000
    total_ms %= 60000

    secs = total_ms // 1000
    millis = total_ms % 1000

    return f"{hours:02d}:{minutes:02d}:{secs:02d}.{millis:03d}"


def get_command_output(command):
    try:
        result = subprocess.run(
            command,
            cwd=PROJECT_ROOT,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            shell=False,
        )

        if result.returncode == 0:
            return result.stdout.strip()

        return ""

    except Exception:
        return ""


def get_git_info():
    branch = get_command_output(["git", "rev-parse", "--abbrev-ref", "HEAD"])
    commit = get_command_output(["git", "rev-parse", "HEAD"])

    dirty_output = get_command_output(["git", "status", "--porcelain"])
    dirty = "YES" if dirty_output else "NO"

    return branch, commit, dirty


def get_platformio_version():
    return get_command_output(["pio", "--version"])


def extract_failure_reason(output):
    lines = output.splitlines()

    interesting_tokens = (
        "fatal error:",
        "error:",
        "undefined reference",
        "collect2:",
        "ld.exe:",
        "*** [",
        "FAILED",
    )

    matches = []

    for line in lines:
        stripped = line.strip()

        if not stripped:
            continue

        lower = stripped.lower()

        for token in interesting_tokens:
            if token.lower() in lower:
                matches.append(stripped)
                break

    if matches:
        return matches[-1]

    for line in reversed(lines):
        stripped = line.strip()

        if stripped:
            return stripped

    return "No failure reason detected."


def run_build(target, log_handle):
    command = ["pio", "run", "-e", target]

    separator = "=" * 100

    log_handle.write(f"\n{separator}\n")
    log_handle.write(f"BUILD TARGET : {target}\n")
    log_handle.write(f"COMMAND      : {' '.join(command)}\n")
    log_handle.write(f"START        : {datetime.now().isoformat(timespec='seconds')}\n")
    log_handle.write(f"{separator}\n\n")
    log_handle.flush()

    start_time = time.perf_counter()

    process = subprocess.Popen(
        command,
        cwd=PROJECT_ROOT,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding="utf-8",
        errors="replace",
        bufsize=1,
    )

    captured_lines = []

    if process.stdout is not None:
        for line in process.stdout:
            captured_lines.append(line)

            log_handle.write(line)
            log_handle.flush()

            print(line, end="")

    return_code = process.wait()

    duration = time.perf_counter() - start_time

    output = "".join(captured_lines)

    success = return_code == 0

    log_handle.write("\n")
    log_handle.write(f"RESULT       : {'SUCCESS' if success else 'FAILED'}\n")
    log_handle.write(f"RETURN CODE  : {return_code}\n")
    log_handle.write(f"DURATION     : {format_duration(duration)}\n")
    log_handle.write(f"END          : {datetime.now().isoformat(timespec='seconds')}\n")
    log_handle.write(f"{separator}\n")
    log_handle.flush()

    failure_reason = ""

    if not success:
        failure_reason = extract_failure_reason(output)

    return {
        "target": target,
        "success": success,
        "return_code": return_code,
        "duration": duration,
        "failure_reason": failure_reason,
    }


def write_summary(results, overall_duration, branch, commit, dirty, pio_version):
    passed = sum(1 for result in results if result["success"])
    failed = len(results) - passed

    lines = []

    lines.append("# PulSar Firmware Build Matrix")
    lines.append("")
    lines.append(f"- Date: {datetime.now().isoformat(timespec='seconds')}")
    lines.append(f"- Git branch: `{branch or 'unknown'}`")
    lines.append(f"- Git commit: `{commit or 'unknown'}`")
    lines.append(f"- Working tree dirty: `{dirty}`")
    lines.append(f"- PlatformIO: `{pio_version or 'unknown'}`")
    lines.append(f"- Targets file: `{TARGETS_FILE.name}`")
    lines.append(f"- Targets: `{len(results)}`")
    lines.append(f"- Passed: `{passed}`")
    lines.append(f"- Failed: `{failed}`")
    lines.append(f"- Total duration: `{format_duration(overall_duration)}`")
    lines.append("")
    lines.append("## Results")
    lines.append("")
    lines.append("| Environment | Result | Duration |")
    lines.append("|---|---:|---:|")

    for result in results:
        status = "SUCCESS" if result["success"] else "FAILED"
        duration = format_duration(result["duration"])

        lines.append(f"| `{result['target']}` | {status} | `{duration}` |")

    failures = [result for result in results if not result["success"]]

    if failures:
        lines.append("")
        lines.append("## Failures")
        lines.append("")

        for result in failures:
            lines.append(f"### {result['target']}")
            lines.append("")
            lines.append(f"- Return code: `{result['return_code']}`")
            lines.append(f"- Duration: `{format_duration(result['duration'])}`")
            lines.append("")
            lines.append("```text")
            lines.append(result["failure_reason"])
            lines.append("```")
            lines.append("")

    lines.append("")
    lines.append("Full terminal output:")
    lines.append("")
    lines.append(f"`{FULL_LOG_FILE.name}`")
    lines.append("")

    SUMMARY_FILE.write_text("\n".join(lines), encoding="utf-8")


def main():
    REPORT_DIR.mkdir(parents=True, exist_ok=True)

    targets = load_targets()

    if not targets:
        print(f"No active build targets found in: {TARGETS_FILE}")
        return 1

    branch, commit, dirty = get_git_info()
    pio_version = get_platformio_version()

    print("")
    print("PulSar Firmware Build Matrix")
    print(f"Project : {PROJECT_ROOT}")
    print(f"Targets : {len(targets)}")
    print(f"Source  : {TARGETS_FILE}")
    print(f"Reports : {REPORT_DIR}")
    print("")

    results = []

    overall_start = time.perf_counter()

    with FULL_LOG_FILE.open("w", encoding="utf-8", errors="replace") as log_handle:
        log_handle.write("PulSar Firmware Build Matrix\n")
        log_handle.write(f"Date              : {datetime.now().isoformat(timespec='seconds')}\n")
        log_handle.write(f"Project root      : {PROJECT_ROOT}\n")
        log_handle.write(f"Targets file      : {TARGETS_FILE}\n")
        log_handle.write(f"Git branch        : {branch}\n")
        log_handle.write(f"Git commit        : {commit}\n")
        log_handle.write(f"Working tree dirty: {dirty}\n")
        log_handle.write(f"PlatformIO        : {pio_version}\n")
        log_handle.write(f"Targets           : {len(targets)}\n")

        for index, target in enumerate(targets, start=1):
            print("")
            print(f"[{index}/{len(targets)}] Building {target}")
            print("-" * 100)

            result = run_build(target, log_handle)
            results.append(result)

            status = "SUCCESS" if result["success"] else "FAILED"

            print("")
            print(f"{target:<45} {status:<9} {format_duration(result['duration'])}")

            if not result["success"]:
                print(f"Reason: {result['failure_reason']}")

    overall_duration = time.perf_counter() - overall_start

    write_summary(
        results,
        overall_duration,
        branch,
        commit,
        dirty,
        pio_version,
    )

    passed = sum(1 for result in results if result["success"])
    failed = len(results) - passed

    print("")
    print("=" * 100)
    print("BUILD MATRIX COMPLETE")
    print("=" * 100)
    print(f"Passed   : {passed}")
    print(f"Failed   : {failed}")
    print(f"Duration : {format_duration(overall_duration)}")
    print(f"Summary  : {SUMMARY_FILE}")
    print(f"Full log : {FULL_LOG_FILE}")
    print("=" * 100)

    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())