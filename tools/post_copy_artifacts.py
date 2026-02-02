# tools/post_copy_artifacts.py
# Copies build artifacts into: <project>/saved_compiles/<env_name>/
#
# Date Modified: 02Feb26

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

def _copy_if_exists(src: str, dst_dir: str) -> bool:
    """
    Copy file if it exists.
    IMPORTANT: do NOT preserve timestamps (Explorer should show updates).
    """
    if not src:
        return False
    if os.path.isfile(src):
        _safe_mkdir(dst_dir)
        dst = os.path.join(dst_dir, os.path.basename(src))

        # copy content only (no metadata), then force mtime to now
        shutil.copyfile(src, dst)
        _touch(dst)

        log(f"COPIED: {src} -> {dst}")
        return True
    else:
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
    # Defensive: ensure we print even if something goes wrong
    try:
        project_dir = env.subst("$PROJECT_DIR")
        build_dir   = env.subst("$BUILD_DIR")   # .pio/build/<env_name>
        env_name    = env.subst("$PIOENV")

        out_dir = os.path.join(project_dir, "saved_compiles", env_name)
        _safe_mkdir(out_dir)

        log("------------------------------------------------------------")
        log(f"ENV   : {env_name}")
        log(f"PROJ  : {project_dir}")
        log(f"BUILD : {build_dir}")
        log(f"OUT   : {out_dir}")

        # Helpful: show what PlatformIO thinks the target is
        try:
            tgt = str(target[0]) if target and len(target) else ""
            log(f"TARGET: {tgt}")
        except Exception:
            pass

        copied = []

        # Common PlatformIO outputs
        if _copy_if_exists(os.path.join(build_dir, "firmware.bin"), out_dir): copied.append("firmware.bin")
        if _copy_if_exists(os.path.join(build_dir, "firmware.elf"), out_dir): copied.append("firmware.elf")
        if _copy_if_exists(os.path.join(build_dir, "firmware.map"), out_dir): copied.append("firmware.map")

        # ESP32 extras (present depending on platform/framework)
        if _copy_if_exists(os.path.join(build_dir, "partitions.bin"), out_dir): copied.append("partitions.bin")
        if _copy_if_exists(os.path.join(build_dir, "bootloader.bin"), out_dir): copied.append("bootloader.bin")

        # Optional map you sometimes enable
        if _copy_if_exists(os.path.join(build_dir, "firmware_inspect.map"), out_dir): copied.append("firmware_inspect.map")

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
        # Make failures visible in the build window
        log(f"ERROR : {e}")
        raise

# Trigger after firmware is produced
# 
# # Trigger after the ELF/BIN are produced (robust across build/upload)
elf_tgt = env.subst("$BUILD_DIR/${PROGNAME}.elf")
bin_tgt = env.subst("$BUILD_DIR/${PROGNAME}.bin")

env.AddPostAction(elf_tgt, after_build)
env.AddPostAction(bin_tgt, after_build)

# Optional: also run after upload target (handy if you mainly click Upload)
env.AddPostAction("upload", after_build)

log(f"HOOKS : {elf_tgt}")
log(f"HOOKS : {bin_tgt}")
log("HOOKS : upload")

