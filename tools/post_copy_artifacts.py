# tools/post_copy_artifacts.py
# Copies build artifacts into: <project>/saved_compiles/<env_name>/
#
# Date Modified: 16Sep26

Import("env")

import os
import shutil
import json
import subprocess
import time
from datetime import datetime


TAG = "[post_copy_artifacts]"


def log(msg: str) -> None:
    print(f"{TAG} {msg}")


def _safe_mkdir(path: str) -> None:
    os.makedirs(path, exist_ok=True)


def _touch(path: str) -> None:
    try:
        now = time.time()
        os.utime(path, (now, now))
    except Exception as e:
        log(f"TOUCH : failed {path} ({e})")


def _copy_file(src: str, dst: str) -> bool:
    if not src or not os.path.isfile(src):
        return False

    try:
        _safe_mkdir(os.path.dirname(dst))
        shutil.copyfile(src, dst)
        _touch(dst)
        log(f"COPIED: {src} -> {dst}")
        return True
    except Exception as e:
        log(f"COPY  : failed {src} -> {dst} ({e})")
        return False


def _copy_if_exists(src: str, dst_dir: str) -> bool:
    if not src:
        return False

    if os.path.isfile(src):
        dst = os.path.join(dst_dir, os.path.basename(src))
        return _copy_file(src, dst)

    log(f"MISS  : {src}")
    return False


def _copy_first_existing(candidates: list, dst: str) -> bool:
    for src in candidates:
        if src and os.path.isfile(src):
            return _copy_file(src, dst)

    for src in candidates:
        if src:
            log(f"MISS  : {src}")

    return False


def _run_git(project_dir: str, args: list) -> str:
    try:
        out = subprocess.check_output(
            ["git", "-C", project_dir] + args,
            stderr=subprocess.DEVNULL
        ).decode("utf-8", errors="replace").strip()

        return out
    except Exception as e:
        log(f"GIT   : failed {' '.join(args)} ({e})")
        return ""


def after_build(source, target, env):
    try:
        project_dir = env.subst("$PROJECT_DIR")
        build_dir = env.subst("$BUILD_DIR")
        env_name = env.subst("$PIOENV")
        progname = env.subst("$PROGNAME")

        out_dir = os.path.join(project_dir, "saved_compiles", env_name)
        _safe_mkdir(out_dir)

        log("------------------------------------------------------------")
        log(f"ENV   : {env_name}")
        log(f"PROJ  : {project_dir}")
        log(f"BUILD : {build_dir}")
        log(f"OUT   : {out_dir}")

        try:
            tgt = str(target[0]) if target and len(target) else ""
            log(f"TARGET: {tgt}")
        except Exception:
            pass

        copied = []

        bin_src = os.path.join(build_dir, f"{progname}.bin")
        elf_src = os.path.join(build_dir, f"{progname}.elf")

        if _copy_if_exists(bin_src, out_dir):
            copied.append("firmware.bin")

        if _copy_if_exists(elf_src, out_dir):
            copied.append("firmware.elf")

        map_candidates = [
            os.path.join(build_dir, f"{progname}.map"),
            os.path.join(build_dir, "firmware.map"),
            os.path.join(project_dir, "firmware.map"),
            os.path.join(project_dir, "build_output", "map", f"{env_name}.map"),
        ]

        map_dst = os.path.join(out_dir, "firmware.map")

        if _copy_first_existing(map_candidates, map_dst):
            copied.append("firmware.map")

        inspect_map_candidates = [
            os.path.join(build_dir, "firmware_inspect.map"),
            os.path.join(project_dir, "firmware_inspect.map"),
            os.path.join(project_dir, "build_output", "map", f"{env_name}_inspect.map"),
        ]

        inspect_map_dst = os.path.join(out_dir, "firmware_inspect.map")

        if _copy_first_existing(inspect_map_candidates, inspect_map_dst):
            copied.append("firmware_inspect.map")

        partitions_src = os.path.join(build_dir, "partitions.bin")
        bootloader_src = os.path.join(build_dir, "bootloader.bin")

        if _copy_if_exists(partitions_src, out_dir):
            copied.append("partitions.bin")

        if _copy_if_exists(bootloader_src, out_dir):
            copied.append("bootloader.bin")

        meta = {
            "env_name": env_name,
            "created_utc": datetime.utcnow().isoformat(timespec="seconds") + "Z",
            "created_epoch": int(time.time()),
            "project_dir": project_dir,
            "build_dir": build_dir,
            "platform": env.get("PIOPLATFORM"),
            "board": env.get("BOARD"),
            "framework": env.get("PIOFRAMEWORK"),
            "copied": copied,
            "git": {
                "branch": _run_git(project_dir, ["rev-parse", "--abbrev-ref", "HEAD"]),
                "commit": _run_git(project_dir, ["rev-parse", "HEAD"]),
                "describe": _run_git(project_dir, ["describe", "--always", "--dirty"]),
                "status_short": _run_git(project_dir, ["status", "--porcelain"])
            }
        }

        meta_path = os.path.join(out_dir, "build_meta.json")

        with open(meta_path, "w", encoding="utf-8") as f:
            json.dump(meta, f, indent=2)
            f.write("\n")

        _touch(meta_path)

        log(f"WROTE : {meta_path}")

        if not copied:
            log("WARN  : No artifacts copied. Check BUILD_DIR path and filenames.")
        else:
            log(f"DONE  : copied={copied}")

        log("------------------------------------------------------------")

    except Exception as e:
        log(f"ERROR : {e}")
        raise


elf_tgt = env.subst("$BUILD_DIR/${PROGNAME}.elf")
bin_tgt = env.subst("$BUILD_DIR/${PROGNAME}.bin")

env.AddPostAction(elf_tgt, after_build)
env.AddPostAction(bin_tgt, after_build)
env.AddPostAction("upload", after_build)

log(f"HOOKS : {elf_tgt}")
log(f"HOOKS : {bin_tgt}")
log("HOOKS : upload")