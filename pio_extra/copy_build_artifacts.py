Import("env")
import os
import shutil
import subprocess
from datetime import datetime

def _safe_mkdir(path: str):
    os.makedirs(path, exist_ok=True)

def _run_git_cmd(args):
    try:
        out = subprocess.check_output(args, stderr=subprocess.DEVNULL).decode("utf-8").strip()
        return out
    except Exception:
        return "nogit"

def _get_git_sha_short():
    return _run_git_cmd(["git", "rev-parse", "--short", "HEAD"])

def _get_git_dirty_flag():
    s = _run_git_cmd(["git", "status", "--porcelain"])
    return "dirty" if s else "clean"

def after_build(source, target, env_):
    # PlatformIO env name (e.g. tg_boardbuild__esp8266_nodemcu)
    env_name = env_.substitutions.get("PIOENV", "unknown_env")

    # Build output paths
    build_dir = env_.substitutions.get("BUILD_DIR")
    progname = env_.substitutions.get("PROGNAME", "firmware")

    elf_src = os.path.join(build_dir, f"{progname}.elf")
    bin_src = os.path.join(build_dir, f"{progname}.bin")

    # Archive destination
    ts = datetime.now().strftime("%Y%m%d_%H%M%S")
    sha = _get_git_sha_short()
    dirty = _get_git_dirty_flag()

    dest_root = os.path.join(env_.substitutions.get("PROJECT_DIR"), ".elf_archive")
    dest_dir  = os.path.join(dest_root, env_name, f"{ts}_{sha}_{dirty}")
    _safe_mkdir(dest_dir)

    # Copy ELF/BIN if present
    if os.path.isfile(elf_src):
        shutil.copy2(elf_src, os.path.join(dest_dir, os.path.basename(elf_src)))
    if os.path.isfile(bin_src):
        shutil.copy2(bin_src, os.path.join(dest_dir, os.path.basename(bin_src)))

    # Save metadata for later decoding context
    meta_path = os.path.join(dest_dir, "meta.txt")
    try:
        with open(meta_path, "w", encoding="utf-8") as f:
            f.write(f"env={env_name}\n")
            f.write(f"timestamp={ts}\n")
            f.write(f"git_sha={sha}\n")
            f.write(f"git_state={dirty}\n")
            f.write(f"board={env_.substitutions.get('BOARD', '')}\n")
            f.write(f"platform={env_.substitutions.get('PIOPLATFORM', '')}\n")
            f.write(f"framework={env_.substitutions.get('PIOFRAMEWORK', '')}\n")
            f.write(f"progname={progname}\n")
            f.write(f"build_dir={build_dir}\n")
            # BUILD_FLAGS is huge; still useful to have
            f.write("build_flags=" + " ".join(env_.Flatten(env_.get("BUILD_FLAGS", []))) + "\n")
    except Exception:
        pass

    print(f"[copy_build_artifacts] Archived to: {dest_dir}")

# Runs after the firmware is linked/built
env.AddPostAction("buildprog", after_build)
