# pio/build_webpages.py

# run maunally with "npm run build:webpages" or automatically as a PlatformIO pre-build extra_script (configured in platformio.ini).
#
# PURPOSE
#   PlatformIO extra_script (pre-build) to build ALL embedded webpages for PulSar using npm.
#
#   Orchestration model:
#     - Python invokes npm once (single entrypoint).
#     - npm scripts orchestrate running all cdata generators in the required order.
#
#   Generator intent (ordered for priority / long-term architecture):
#     1) cdata_webserver.js   (primary, core WebServer UI)
#     2) cdata_submodules.js  (distributed small pages in drivers/sensors/modules)
#     3) cdata_lights.js      (secondary, WLED-derived lights UI; isolated long-term)
#
# -----------------------------------------------------------------------------
# FLOW CHARTS (WHO CALLS WHAT)
# -----------------------------------------------------------------------------
#
# A) Legacy method (lights-only, module-cwd dependent)
#
#   PlatformIO build
#     |
#     |-- platformio.ini: extra_scripts = pre:pio/<old_build_web*.py>
#     |
#     V
#   pio/<old_build_web*.py>
#     |
#     |-- Check tool exists: npm
#     |
#     |-- Determine module folder:
#     |     source_dir = <repo_root>/code/6_Lights/03_Animator/source
#     |
#     |-- chdir(source_dir)        [cwd forced into lights module]
#     |
#     |-- npm install --only=dev   [often re-run; noisy]
#     |
#     |-- npm run build
#            |
#            |-- package.json "scripts"."build"
#            V
#          node tools/cdata_lights.js
#            |
#            |-- cdata_lights.js loads manifest:
#            |     require("./cdata_lights.json") (formerly cdata.json)
#            |
#            |-- Reads assets from source_path
#            |-- Writes generated headers into destination_path
#            V
#          Generated headers written into lights output folder (e.g. webpages_generated/...)
#
#
# B) New method (B1) - npm orchestrates all generators (repo-root entrypoint)
#
#   PlatformIO build
#     |
#     |-- platformio.ini: extra_scripts = pre:pio/build_webpages.py
#     |
#     V
#   pio/build_webpages.py
#     |
#     |-- Check tool exists: npm
#     |
#     |-- chdir(<repo root that contains package.json>)
#     |
#     |-- Optional dependency bootstrap (only when needed):
#     |     if <repo_root>/node_modules missing:
#     |       npm ci  (if package-lock.json exists)  OR
#     |       npm install
#     |
#     |-- npm run build:webpages
#            |
#            |-- package.json scripts:
#            |     build:webpages =
#            |       build:webserver && build:submodules && build:lights
#            |
#            |-- npm run build:webserver   -> node tools/cdata_webserver.js
#            |-- npm run build:submodules  -> node tools/cdata_submodules.js
#            |-- npm run build:lights      -> node tools/cdata_lights.js
#                   |
#                   |-- Each cdata_*.js either:
#                   |     (a) loads a manifest cdata_*.json (manual control), or
#                   |     (b) scans directories (auto-discovery for submodules)
#                   |
#                   |-- Each cdata_*.js reads its own source_path and writes its own destination_path
#                   V
#            Generated headers written into:
#              - WebServer primary generated folder(s)
#              - Submodule /Webpages/Gen folders (per module found)
#              - Lights generated folder(s)
#
# -----------------------------------------------------------------------------
# NOTES ON CWD (CURRENT WORKING DIRECTORY)
# -----------------------------------------------------------------------------
# - Legacy pipeline relied on chdir(module_folder) because:
#     (1) npm needed to find package.json in that folder (depending on layout), and/or
#     (2) generators used relative paths anchored to cwd.
#
# - This pipeline relies on chdir(repo_root) only:
#     - npm is executed from the repo root containing package.json
#     - generators should resolve their own paths deterministically (prefer __dirname or repo-root anchoring)
#
# -----------------------------------------------------------------------------
# REQUIRED npm scripts (package.json)
# -----------------------------------------------------------------------------
# Expected scripts:
#   "build:webserver":  "node tools/cdata_webserver.js"
#   "build:submodules": "node tools/cdata_submodules.js"
#   "build:lights":     "node tools/cdata_lights.js"
#   "build:webpages":   "npm run build:webserver && npm run build:submodules && npm run build:lights"
#
# -----------------------------------------------------------------------------

from subprocess import check_output, CalledProcessError
import os
import platform

Import("env")


# -----------------------------
# CONFIG
# -----------------------------

# Failure policy:
#   False -> log error and continue (compiles using last generated headers)
#   True  -> fail build immediately if webpage generation fails
FAIL_HARD = False

# Dependency bootstrap policy:
#   If True: install npm deps only when node_modules is missing (robust on new machines / clean clones)
#   If False: never auto-install (assumes deps pre-installed)
AUTO_INSTALL_NODE_MODULES = True

# Prefer npm ci if package-lock.json exists (deterministic installs)
PREFER_NPM_CI = True

# npm script that orchestrates all webpage builds
NPM_BUILD_ALL_SCRIPT = "build:webpages"


# -----------------------------
# Helpers
# -----------------------------

def _which_cmd():
    return "where" if platform.system() == "Windows" else "which"


def is_tool(name):
    try:
        check_output([_which_cmd(), name])
        return True
    except Exception:
        return False


def npm_exe():
    return "npm.cmd" if platform.system() == "Windows" else "npm"


def run_cmd(args, label):
    print(f"[WEB] {label}")
    out = check_output(args, shell=False)
    try:
        txt = out.decode("utf-8", errors="replace")
    except Exception:
        txt = str(out)
    if txt.strip():
        print(txt)
    return 0


# -----------------------------
# Main
# -----------------------------

def build_webpages():
    npm = npm_exe()

    # Tool presence gate
    if not is_tool(npm):
        msg = f"[WEB] npm not available ({npm} not found in PATH). Skipping webpage generation."
        if FAIL_HARD:
            raise RuntimeError(msg)
        print(msg)
        return

    # Project root (must contain package.json)
    project_dir = env["PROJECT_DIR"]

    prev_cwd = os.getcwd()
    print(f"[WEB] Changing to project directory: {project_dir}")
    os.chdir(project_dir)

    try:
        # Dependency bootstrap only when node_modules is absent
        if AUTO_INSTALL_NODE_MODULES:
            node_modules_dir = os.path.join(project_dir, "node_modules")
            if not os.path.isdir(node_modules_dir):
                print("[WEB] node_modules not found, installing npm dependencies...")
                lock_path = os.path.join(project_dir, "package-lock.json")
                if PREFER_NPM_CI and os.path.exists(lock_path):
                    run_cmd([npm, "ci"], "npm ci (deterministic install)")
                else:
                    run_cmd([npm, "install"], "npm install")
            else:
                print("[WEB] node_modules present, skipping npm install")

        # Build all webpages via npm orchestration
        run_cmd([npm, "run", NPM_BUILD_ALL_SCRIPT], f"npm run {NPM_BUILD_ALL_SCRIPT}")

        print("[WEB] Webpage generation complete.")

    except CalledProcessError as e:
        print("[WEB] ERROR: webpage generation failed (npm returned non-zero).")
        try:
            print(e.output.decode("utf-8", errors="replace"))
        except Exception:
            print(str(e.output))

        if FAIL_HARD:
            raise
        print("[WEB] WARNING: continuing build using last generated headers.")

    except Exception as e:
        print(f"[WEB] ERROR: unexpected exception ({type(e).__name__}): {e}")
        if FAIL_HARD:
            raise
        print("[WEB] WARNING: continuing build using last generated headers.")

    finally:
        os.chdir(prev_cwd)
        print(f"[WEB] Returning to previous directory: {prev_cwd}")


build_webpages()
