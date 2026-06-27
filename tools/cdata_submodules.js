/**
 * tools/cdata_submodules.js
 *
 * SUMMARY
 *   Auto-generate compressed C headers for small “submodule” webpages.
 *
 *   Scans selected PulSar module trees for folders matching:
 *     <ModulePath>/Web/Src
 *
 *   For each hit:
 *     - Ensures <ModulePath>/Web/Gen exists
 *     - Reads all convertible assets in Web/Src (recursively)
 *     - HTML: minify always; optional INLINE of external refs is OPT-IN
 *     - CSS: minify
 *     - JS : minify
 *     - All assets: gzip -> PROGMEM arrays
 *     - Writes ONE header per submodule into Web/Gen:
 *         Web/Gen/generated_web.h
 *
 * OUTPUT CONTRACT (per Web/Src)
 *   Web/Gen/generated_web.h contains, per source asset:
 *     - const uint8_t  <SYMBOL>[] PROGMEM
 *     - const uint16_t <SYMBOL>_length
 *   where <SYMBOL> is stable + unique per submodule and file path.
 *
 * INLINE/MINIFY RULES (SUBMODULES)
 *   - HTML: minified + gzipped ALWAYS
 *   - HTML inlining of external refs (CSS/JS/images) is OPT-IN:
 *       Filename must contain "Inline" (case-sensitive) to inline.
 *     Rationale:
 *       Submodules should default to using shared root helpers (/common.js, /style.css, etc),
 *       and avoid baking shared assets into every module page.
 *   - "NoInline" (case-sensitive) forces NO inline (override, in case both tokens appear).
 *   - CSS: minified + gzipped
 *   - JS : minified + gzipped
 *   - Images/ico/etc: treated as binary + gzipped
 *
 * NOTES
 *   - This intentionally avoids 6_Lights (lights has its own generator).
 *   - WebServer main UI is handled by cdata_webserver.js (separate pipeline).
 *   - This script has NO dependency on deasync/deasync-promise; it is async/await end-to-end.
 *
 * RUN
 *   npm run build:submodules
 *
 * CHANGED
 *   05Jan26  Initial version: recursive discovery of Web/Src -> Web/Gen, single-header output per submodule.
 *   05Jan26  Async/await end-to-end, removed any dependency on deasync/deasync-promise.
 *   05Jan26  Inline behaviour flipped: OPT-IN via "Inline" token in filename.
 *   04Jun26  Added CoreSystem is also scanned
 */

"use strict";

const fs = require("fs");
const path = require("path");
const inliner = require("inliner");
const zlib = require("zlib");
const CleanCSS = require("clean-css");
const MinifyHTML = require("html-minifier-terser").minify;

// -----------------------------------------------------------------------------
// CONFIG
// -----------------------------------------------------------------------------

// Repo root is parent of /tools
const REPO_ROOT = path.resolve(__dirname, "..");
const CODE_ROOT = path.join(REPO_ROOT, "code");

// Top-level module groups to scan for submodules.
// Keep tight to avoid scanning huge trees accidentally.
const TOP_GROUPS_TO_SCAN = [
  "2_CoreSystem",
  "4_Drivers",
  "5_Sensors",
  "7_Energy",
  "8_Displays",
  "9_Controller",
  "10_ConSpec",
];

// Skip anything under this top group (lights has its own generator)
const SKIP_TOP_GROUPS = new Set(["6_Lights"]);

// Web folder contract
const WEB_FOLDER_NAME = "Web";
const WEB_SRC_NAME = "Src";
const WEB_GEN_NAME = "Gen";

// Output header filename per submodule
const OUT_HEADER_NAME = "generated_web.h";

// Gzip compression level (best compression; submodules are small, cost is build-time only)
const GZIP_LEVEL = zlib.constants.Z_BEST_COMPRESSION;

// -----------------------------------------------------------------------------
// UTILS
// -----------------------------------------------------------------------------

function ensureDir(p) {
  if (!fs.existsSync(p)) fs.mkdirSync(p, { recursive: true });
}

function isDir(p) {
  try {
    return fs.statSync(p).isDirectory();
  } catch {
    return false;
  }
}

/**
 * walkDirs(rootDir)
 *   Returns list of directories (recursive), including rootDir.
 *   Skips node_modules and .git.
 */
function walkDirs(rootDir) {
  const out = [];
  const stack = [rootDir];

  while (stack.length) {
    const d = stack.pop();
    out.push(d);

    let entries;
    try {
      entries = fs.readdirSync(d, { withFileTypes: true });
    } catch {
      continue;
    }

    for (const e of entries) {
      if (!e.isDirectory()) continue;
      if (e.name === "node_modules" || e.name === ".git") continue;
      stack.push(path.join(d, e.name));
    }
  }

  return out;
}

/**
 * walkFiles(rootDir)
 *   Returns list of files (recursive).
 *   Skips node_modules and .git.
 */
function walkFiles(rootDir) {
  const out = [];
  const stack = [rootDir];

  while (stack.length) {
    const d = stack.pop();

    let entries;
    try {
      entries = fs.readdirSync(d, { withFileTypes: true });
    } catch {
      continue;
    }

    for (const e of entries) {
      const p = path.join(d, e.name);
      if (e.isDirectory()) {
        if (e.name === "node_modules" || e.name === ".git") continue;
        stack.push(p);
      } else if (e.isFile()) {
        out.push(p);
      }
    }
  }

  return out;
}

function fileExtLower(p) {
  return path.extname(p).toLowerCase();
}

/**
 * shouldInlineHtml(filePath)
 *   Submodule policy:
 *     - Inline is OPT-IN: filename must contain "Inline"
 *     - "NoInline" forces no-inline (override)
 */
function shouldInlineHtml(filePath) {
  const base = path.basename(filePath);
  if (base.includes("NoInline")) return false;
  return base.includes("Inline");
}

/**
 * sanitizeIdent(s)
 *   Stable C identifier component:
 *     - normalize slashes
 *     - replace non [a-zA-Z0-9_] with _
 *     - collapse repeats
 */
function sanitizeIdent(s) {
  return s
    .replace(/\\/g, "/")
    .replace(/[^a-zA-Z0-9_]/g, "_")
    .replace(/_+/g, "_")
    .replace(/^_+/, "")
    .replace(/_+$/, "");
}

/**
 * hexdump(buf)
 *   Emit "0x.." comma-separated bytes, 16 bytes per line.
 */
function hexdump(buf) {
  const lines = [];
  for (let i = 0; i < buf.length; i += 16) {
    const block = buf.slice(i, i + 16);
    const hexArray = [];
    for (const v of block) hexArray.push("0x" + v.toString(16).padStart(2, "0"));
    lines.push("  " + hexArray.join(", "));
  }
  return lines.join(",\n");
}

function minifyCss(css) {
  return new CleanCSS({}).minify(css).styles;
}

/**
 * minifyJs(js)
 *   Uses html-minifier-terser by wrapping in <script>..</script> (WLED-style).
 */
function minifyJs(js) {
  return MinifyHTML("<script>" + js + "</script>", {
    collapseWhitespace: true,
    minifyJS: true,
    continueOnParseError: false,
    removeComments: true,
  }).replace(/<[\/]*script>/g, "");
}

/**
 * minifyHtml(html, uiMode)
 *   uiMode=true uses conservativeCollapse, matching WLED’s "ui" minify behaviour.
 */
function minifyHtml(html, uiMode = true) {
  return MinifyHTML(html, {
    collapseWhitespace: true,
    conservativeCollapse: uiMode,
    maxLineLength: 80,
    minifyCSS: true,
    minifyJS: true,
    continueOnParseError: false,
    removeComments: true,
  });
}

function gzipBuf(bufOrString) {
  if (Buffer.isBuffer(bufOrString)) {
    return zlib.gzipSync(bufOrString, { level: GZIP_LEVEL });
  }
  return zlib.gzipSync(Buffer.from(bufOrString, "utf-8"), { level: GZIP_LEVEL });
}

/**
 * buildGzChunkBytes(symbol, zipBuf, srcRelPath)
 *   Emit the C snippet for one gzipped asset.
 */
function buildGzChunkBytes(symbol, zipBuf, srcRelPath) {
  const array = hexdump(zipBuf);
  return `
// Autogenerated from ${srcRelPath} (gzip), do not edit!!
const uint16_t ${symbol}_length = ${zipBuf.length};
const uint8_t ${symbol}[] PROGMEM = {
${array}
};
`;
}

// -----------------------------------------------------------------------------
// INLINE: Promise wrapper for inliner (NO extra deps)
// -----------------------------------------------------------------------------

/**
 * inlineFileToString(sourceFile)
 *   Uses inliner to resolve external refs from the filesystem / relative URLs.
 *   IMPORTANT: With submodules, only do this when explicitly requested via filename token.
 */
function inlineFileToString(sourceFile) {
  return new Promise((resolve, reject) => {
    new inliner(sourceFile, (error, html) => {
      if (error) return reject(error);
      resolve(html);
    });
  });
}

// -----------------------------------------------------------------------------
// CORE: per-submodule header build (async)
// -----------------------------------------------------------------------------

async function buildSubmoduleHeader(submoduleRootDir, webSrcDir, webGenDir) {
  const files = walkFiles(webSrcDir);

  // Derive stable module identity:
  //   .../code/10_ConSpec/13_ServerResetRelays
  // groupName  = "10_ConSpec"
  // moduleName = "13_ServerResetRelays"
  const moduleName = path.basename(submoduleRootDir);
  const groupName = path.basename(path.dirname(submoduleRootDir));

  // Prefix for symbols in this submodule (stable + readable)
  const prefix = sanitizeIdent(`WEB_${groupName}_${moduleName}`);

  // Header preamble
  const now = new Date().toISOString();
  let out = `/*
 * AUTO-GENERATED FILE
 *   Generator: tools/cdata_submodules.js
 *
 * Source:
 *   ${path.relative(REPO_ROOT, webSrcDir).replace(/\\/g, "/")}
 *
 * Output:
 *   ${path.relative(REPO_ROOT, webGenDir).replace(/\\/g, "/")}/${OUT_HEADER_NAME}
 *
 * Notes:
 *   - All assets are gzip-compressed for PROGMEM storage.
 *   - HTML inlining is OPT-IN via "Inline" in filename.
 */
#pragma once
`;

  for (const absFile of files) {
    const relFromSrc = path.relative(webSrcDir, absFile).replace(/\\/g, "/");
    const relFromRepo = path.relative(REPO_ROOT, absFile).replace(/\\/g, "/");
    const ext = fileExtLower(absFile);

    // Stable symbol name per submodule + relative file path
    const symbol = sanitizeIdent(`${prefix}__${relFromSrc}`);

    try {
      if (ext === ".htm" || ext === ".html") {
        const doInline = shouldInlineHtml(absFile);

        // HTML:
        //   - default: NO inline, just minify
        //   - opt-in: inline external refs then minify
        let html;
        if (doInline) {
          html = await inlineFileToString(absFile);
        } else {
          html = fs.readFileSync(absFile, "utf-8");
        }

        const minified = minifyHtml(html, true);
        const zip = gzipBuf(minified);
        out += buildGzChunkBytes(symbol, zip, relFromRepo);

      } else if (ext === ".css") {
        const css = fs.readFileSync(absFile, "utf-8");
        const minified = minifyCss(css);
        const zip = gzipBuf(minified);
        out += buildGzChunkBytes(symbol, zip, relFromRepo);

      } else if (ext === ".js") {
        const js = fs.readFileSync(absFile, "utf-8");
        const minified = minifyJs(js);
        const zip = gzipBuf(minified);
        out += buildGzChunkBytes(symbol, zip, relFromRepo);

      } else {
        // Everything else treated as binary, then gzipped.
        const buf = fs.readFileSync(absFile);
        const zip = gzipBuf(buf);
        out += buildGzChunkBytes(symbol, zip, relFromRepo);
      }

    } catch (e) {
      // Keep generator robust: emit a comment and continue so one bad asset does not kill the build.
      out += `
// FAILED to process ${relFromRepo}: ${String(e && e.message ? e.message : e)}
`;
    }
  }

  const outFile = path.join(webGenDir, OUT_HEADER_NAME);
  fs.writeFileSync(outFile, out);
  console.info(`Wrote: ${path.relative(REPO_ROOT, outFile).replace(/\\/g, "/")}`);
}

// -----------------------------------------------------------------------------
// DISCOVERY: find <submodule>/Web/Src
// -----------------------------------------------------------------------------

function findWebSrcDirs() {
  const hits = [];

  for (const top of TOP_GROUPS_TO_SCAN) {
    if (SKIP_TOP_GROUPS.has(top)) continue;

    const topDir = path.join(CODE_ROOT, top);
    if (!isDir(topDir)) continue;

    // Walk all dirs and detect ".../Web/Src"
    const dirs = walkDirs(topDir);
    for (const d of dirs) {
      if (path.basename(d) !== WEB_SRC_NAME) continue;

      const parent = path.dirname(d);
      if (path.basename(parent) !== WEB_FOLDER_NAME) continue;

      // submoduleRoot = .../<group>/<module>
      const moduleRoot = path.dirname(parent);

      // Extra safety: skip lights tree
      const relToCode = path.relative(CODE_ROOT, moduleRoot).replace(/\\/g, "/");
      if (relToCode.startsWith("6_Lights/")) continue;

      hits.push({
        group: top,
        moduleRoot,
        webSrc: d,
        webGen: path.join(parent, WEB_GEN_NAME),
      });
    }
  }

  return hits;
}

// -----------------------------------------------------------------------------
// MAIN (async)
// -----------------------------------------------------------------------------

async function main() {
  console.info("PulSar cdata_submodules.js");
  console.info("Repo: " + REPO_ROOT.replace(/\\/g, "/"));
  console.info("Code: " + CODE_ROOT.replace(/\\/g, "/"));

  const hits = findWebSrcDirs();
  if (!hits.length) {
    console.info("No Web/Src directories found. Nothing to do.");
    return;
  }

  console.info(`Found ${hits.length} submodule Web/Src directory(ies).`);

  for (const h of hits) {
    const srcRel = path.relative(REPO_ROOT, h.webSrc).replace(/\\/g, "/");
    const genRel = path.relative(REPO_ROOT, h.webGen).replace(/\\/g, "/");
    const modRel = path.relative(REPO_ROOT, h.moduleRoot).replace(/\\/g, "/");

    console.info("------------------------------------------------------------");
    console.info("Module: " + modRel);
    console.info("Src:    " + srcRel);
    console.info("Gen:    " + genRel);

    ensureDir(h.webGen);
    await buildSubmoduleHeader(h.moduleRoot, h.webSrc, h.webGen);
  }

  console.info("Done.");
}

main().catch((e) => {
  console.error("FATAL:", e);
  process.exit(1);
});

// /**
//  * tools/cdata_submodules.js
//  *
//  * SUMMARY
//  *   Auto-generate compressed C headers for small “submodule” webpages.
//  *
//  *   Scans selected PulSar module trees for folders matching:
//  *     <ModulePath>/Web/Src
//  *
//  *   For each hit:
//  *     - Ensures <ModulePath>/Web/Gen exists
//  *     - Reads all convertible assets in Web/Src (recursively)
//  *     - HTML: inline + minify by default (unless filename contains "NoInline")
//  *     - CSS: minify
//  *     - JS : minify
//  *     - All assets: gzip -> PROGMEM arrays
//  *     - Writes ONE header per submodule into Web/Gen:
//  *         Web/Gen/generated_web.h
//  *
//  * OUTPUT CONTRACT (per Web/Src)
//  *   Web/Gen/generated_web.h contains:
//  *     - const uint8_t  <SYMBOL>[] PROGMEM
//  *     - const uint16_t <SYMBOL>_length
//  *   where <SYMBOL> is stable + unique per submodule and file path.
//  *
//  * INLINE/MINIFY RULES
//  *   - HTML: inlined + minified by default
//  *   - HTML with "NoInline" in filename: NOT inlined (still minified + gzipped)
//  *   - CSS: minified + gzipped
//  *   - JS : minified + gzipped
//  *   - Images/ico/etc: treated as binary + gzipped
//  *
//  * NOTES
//  *   - This intentionally avoids 6_Lights (lights has its own pipeline).
//  *   - WebServer main UI is handled by cdata_webserver.js (separate).
//  *
//  * RUN
//  *   npm run build:submodules
//  *
//  * CHANGED
//  *   05Jan26  Initial version: recursive discovery of Web/Src -> Web/Gen, single-header output per submodule.
//  *   05Jan26  Removed any dependency on deasync/deasync-promise; generator is async/await end-to-end.
//  */

// "use strict";

// const fs = require("fs");
// const path = require("path");
// const inliner = require("inliner");
// const zlib = require("zlib");
// const CleanCSS = require("clean-css");
// const MinifyHTML = require("html-minifier-terser").minify;

// // -----------------------------------------------------------------------------
// // CONFIG
// // -----------------------------------------------------------------------------
// //
// // Repo root is the parent of /tools.
// // This generator is intended to be launched from repo root via npm scripts.
// // However, all filesystem operations here are anchored using REPO_ROOT to avoid
// // dependency on current working directory.
// //
// const REPO_ROOT = path.resolve(__dirname, "..");
// const CODE_ROOT = path.join(REPO_ROOT, "code");

// // Top-level module groups to scan for submodules.
// // Keep tight to avoid accidental traversal of unrelated trees.
// const TOP_GROUPS_TO_SCAN = [
//   "4_Drivers",
//   "5_Sensors",
//   "7_Energy",
//   "8_Displays",
//   "9_Controller",
//   "10_ConSpec",
// ];

// // Extra defensive skip list. Even if later added to TOP_GROUPS_TO_SCAN,
// // anything in SKIP_TOP_GROUPS will be ignored.
// const SKIP_TOP_GROUPS = new Set([
//   "6_Lights", // lights are handled by cdata_lights.js pipeline
// ]);

// // Web folder contract (case-sensitive on some platforms).
// // A submodule webpage pack is discovered only when:
// //   .../<ModuleRoot>/Web/Src
// // exists.
// const WEB_FOLDER_NAME = "Web";
// const WEB_SRC_NAME    = "Src";
// const WEB_GEN_NAME    = "Gen";

// // Output header filename per submodule.
// const OUT_HEADER_NAME = "generated_web.h";

// // Gzip compression level. Match WLED behaviour: smallest output.
// const GZIP_LEVEL = zlib.constants.Z_BEST_COMPRESSION;

// // Guardrails: submodule pages should be small.
// // If somebody drops a massive binary into Web/Src, this keeps the build from
// // exploding unexpectedly.
// // Set to 0 to disable (not recommended).
// const MAX_SINGLE_FILE_BYTES = 5 * 1024 * 1024; // 5MB

// // -----------------------------------------------------------------------------
// // UTILS: basic filesystem helpers
// // -----------------------------------------------------------------------------

// function ensureDir(p) {
//   if (!fs.existsSync(p)) fs.mkdirSync(p, { recursive: true });
// }

// function isDir(p) {
//   try { return fs.statSync(p).isDirectory(); } catch { return false; }
// }

// function isFile(p) {
//   try { return fs.statSync(p).isFile(); } catch { return false; }
// }

// // walkDirs(rootDir)
// //   Returns a list of directories recursively including rootDir itself.
// //   Implementation is iterative to avoid call stack depth issues.
// function walkDirs(rootDir) {
//   const out = [];
//   const stack = [rootDir];

//   while (stack.length) {
//     const d = stack.pop();
//     out.push(d);

//     let entries;
//     try {
//       entries = fs.readdirSync(d, { withFileTypes: true });
//     } catch {
//       continue;
//     }

//     for (const e of entries) {
//       if (!e.isDirectory()) continue;
//       if (e.name === "node_modules" || e.name === ".git") continue;
//       stack.push(path.join(d, e.name));
//     }
//   }
//   return out;
// }

// // walkFiles(rootDir)
// //   Returns a list of files recursively.
// //   Implementation is iterative to avoid recursion depth issues.
// function walkFiles(rootDir) {
//   const out = [];
//   const stack = [rootDir];

//   while (stack.length) {
//     const d = stack.pop();

//     let entries;
//     try {
//       entries = fs.readdirSync(d, { withFileTypes: true });
//     } catch {
//       continue;
//     }

//     for (const e of entries) {
//       const p = path.join(d, e.name);
//       if (e.isDirectory()) {
//         if (e.name === "node_modules" || e.name === ".git") continue;
//         stack.push(p);
//       } else if (e.isFile()) {
//         out.push(p);
//       }
//     }
//   }
//   return out;
// }

// function fileExtLower(p) {
//   return path.extname(p).toLowerCase();
// }

// // shouldInlineHtml(filePath)
// //   Inline-by-default, except when filename contains "NoInline".
// //   This matches your current stated intent: submodules can inline if they want,
// //   but they can explicitly prevent it for pages intended to load helpers via URL.
// function shouldInlineHtml(filePath) {
//   const base = path.basename(filePath);
//   return base.indexOf("NoInline") === -1;
// }

// // sanitizeIdent(s)
// //   Converts arbitrary file-relative paths into stable C identifier components.
// //   Important constraints:
// //     - Must be deterministic
// //     - Must avoid illegal chars
// //     - Must avoid repeated underscores
// function sanitizeIdent(s) {
//   return s
//     .replace(/\\/g, "/")
//     .replace(/[^a-zA-Z0-9_]/g, "_")
//     .replace(/_+/g, "_")
//     .replace(/^_+/, "")
//     .replace(/_+$/, "");
// }

// // hexdump(buf)
// //   Converts a Buffer into a comma-separated 0xNN listing, 16 bytes per line,
// //   suitable for `const uint8_t [] PROGMEM`.
// function hexdump(buf) {
//   const lines = [];
//   for (let i = 0; i < buf.length; i += 16) {
//     const block = buf.slice(i, i + 16);
//     const hexArray = [];
//     for (const v of block) hexArray.push("0x" + v.toString(16).padStart(2, "0"));
//     lines.push("  " + hexArray.join(", "));
//   }
//   return lines.join(",\n");
// }

// // -----------------------------------------------------------------------------
// // MINIFY / COMPRESS (WLED-style)
// // -----------------------------------------------------------------------------

// function minifyCss(css) {
//   return new CleanCSS({}).minify(css).styles;
// }

// // minifyJs(js)
// //   Uses html-minifier-terser’s JS minifier by wrapping in <script> tag.
// //   Mirrors WLED’s approach (compact + robust).
// function minifyJs(js) {
//   return MinifyHTML("<script>" + js + "</script>", {
//     collapseWhitespace: true,
//     minifyJS: true,
//     continueOnParseError: false,
//     removeComments: true,
//   }).replace(/<[\/]*script>/g, "");
// }

// // minifyHtml(html, uiMode)
// //   uiMode=true maps to WLED "html-minify-ui" semantics (more conservative collapse)
// //   since UI pages often rely on subtle whitespace. For submodules, uiMode=true is
// //   usually correct.
// function minifyHtml(html, uiMode = true) {
//   return MinifyHTML(html, {
//     collapseWhitespace: true,
//     conservativeCollapse: uiMode,
//     maxLineLength: 80,
//     minifyCSS: true,
//     minifyJS: true,
//     continueOnParseError: false,
//     removeComments: true,
//   });
// }

// // gzipBuf(bufOrString)
// //   Always gzip. If string, treat as UTF-8.
// function gzipBuf(bufOrString) {
//   if (Buffer.isBuffer(bufOrString)) {
//     return zlib.gzipSync(bufOrString, { level: GZIP_LEVEL });
//   }
//   return zlib.gzipSync(Buffer.from(bufOrString, "utf-8"), { level: GZIP_LEVEL });
// }

// // buildGzChunkBytes(symbol, zipBuf, srcRelPath)
// //   Emits C decls for this gzip asset.
// function buildGzChunkBytes(symbol, zipBuf, srcRelPath) {
//   const array = hexdump(zipBuf);
//   return `
// // Autogenerated from ${srcRelPath} (gzip), do not edit!!
// const uint16_t ${symbol}_length = ${zipBuf.length};
// const uint8_t ${symbol}[] PROGMEM = {
// ${array}
// };
// `;
// }

// // -----------------------------------------------------------------------------
// // INLINE: Promise wrapper for inliner (NO extra deps)
// // -----------------------------------------------------------------------------
// //
// // inliner is callback-based. We wrap it into a Promise and await it.
// // This avoids any need for deasync/deasync-promise and avoids spinlocks.
// //
// function inlineFileToString(sourceFile) {
//   return new Promise((resolve, reject) => {
//     // NOTE: passing file path allows inliner to resolve relative refs
//     new inliner(sourceFile, (error, html) => {
//       if (error) return reject(error);
//       resolve(html);
//     });
//   });
// }

// // -----------------------------------------------------------------------------
// // CORE: per-submodule header build (async/await)
// // -----------------------------------------------------------------------------
// //
// // buildSubmoduleHeader(submoduleRootDir, webSrcDir, webGenDir)
// //   - enumerates all files under webSrcDir recursively
// //   - chooses processing by extension
// //   - emits one header
// //
// async function buildSubmoduleHeader(submoduleRootDir, webSrcDir, webGenDir) {
//   const files = walkFiles(webSrcDir);

//   // Identify module/group for symbol prefix.
//   // submoduleRootDir example:
//   //   .../code/10_ConSpec/13_ServerResetRelays
//   const moduleName = path.basename(submoduleRootDir);
//   const groupName  = path.basename(path.dirname(submoduleRootDir));

//   // Prefix ensures symbols from different submodules never collide.
//   const prefix = sanitizeIdent(`WEB_${groupName}_${moduleName}`);

//   // Header preamble: timestamp, source, output path.
//   const now = new Date().toISOString();
//   let out = `/*
//  * AUTO-GENERATED FILE
//  *   Generator: tools/cdata_submodules.js
//  *   Time:      ${now}
//  *
//  * Source:
//  *   ${path.relative(REPO_ROOT, webSrcDir).replace(/\\/g, "/")}
//  *
//  * Output:
//  *   ${path.relative(REPO_ROOT, webGenDir).replace(/\\/g, "/")}/${OUT_HEADER_NAME}
//  *
//  * Notes:
//  *   - All assets are gzip-compressed for PROGMEM storage.
//  *   - HTML is inlined+minified unless filename contains "NoInline".
//  */
// #pragma once
// `;

//   // Process each file deterministically (in filesystem order).
//   // If you want strict determinism across platforms, you can sort by rel path.
//   files.sort((a, b) => a.localeCompare(b));

//   for (const absFile of files) {
//     const relFromSrc  = path.relative(webSrcDir, absFile).replace(/\\/g, "/");
//     const relFromRepo = path.relative(REPO_ROOT, absFile).replace(/\\/g, "/");
//     const ext = fileExtLower(absFile);

//     // Stable symbol name per submodule + relative file path.
//     // Example:
//     //   WEB_10_ConSpec_13_ServerResetRelays__test_htm
//     const symbol = sanitizeIdent(`${prefix}__${relFromSrc}`);

//     try {
//       // Basic file sanity check (prevents surprises).
//       const st = fs.statSync(absFile);
//       if (st.size > MAX_SINGLE_FILE_BYTES && MAX_SINGLE_FILE_BYTES > 0) {
//         throw new Error(`File too large (${st.size} bytes > ${MAX_SINGLE_FILE_BYTES}): ${relFromRepo}`);
//       }

//       // HTML
//       if (ext === ".htm" || ext === ".html") {
//         const doInline = shouldInlineHtml(absFile);

//         let html;
//         if (doInline) {
//           // Inline external refs using inliner (async)
//           html = await inlineFileToString(absFile);
//         } else {
//           // NoInline: read raw HTML as-is
//           html = fs.readFileSync(absFile, "utf-8");
//         }

//         // Minify then gzip
//         const minified = minifyHtml(html, true);
//         const zip = gzipBuf(minified);
//         out += buildGzChunkBytes(symbol, zip, relFromRepo);

//       // CSS
//       } else if (ext === ".css") {
//         const css = fs.readFileSync(absFile, "utf-8");
//         const minified = minifyCss(css);
//         const zip = gzipBuf(minified);
//         out += buildGzChunkBytes(symbol, zip, relFromRepo);

//       // JS
//       } else if (ext === ".js") {
//         const js = fs.readFileSync(absFile, "utf-8");
//         const minified = minifyJs(js);
//         const zip = gzipBuf(minified);
//         out += buildGzChunkBytes(symbol, zip, relFromRepo);

//       // Everything else => binary gzip
//       } else {
//         const buf = fs.readFileSync(absFile);
//         const zip = gzipBuf(buf);
//         out += buildGzChunkBytes(symbol, zip, relFromRepo);
//       }

//     } catch (e) {
//       // Robust build: emit comment and continue
//       out += `
// // FAILED to process ${relFromRepo}: ${String(e && e.message ? e.message : e)}
// `;
//     }
//   }

//   const outFile = path.join(webGenDir, OUT_HEADER_NAME);
//   fs.writeFileSync(outFile, out);
//   console.info(`Wrote: ${path.relative(REPO_ROOT, outFile).replace(/\\/g, "/")}`);
// }

// // -----------------------------------------------------------------------------
// // DISCOVERY: find <submodule>/Web/Src
// // -----------------------------------------------------------------------------
// //
// // findWebSrcDirs()
// //   - walks each selected top group
// //   - searches for any directory named "Src" whose parent is "Web"
// //   - records moduleRoot, webSrc, webGen
// //
// function findWebSrcDirs() {
//   const hits = [];

//   for (const top of TOP_GROUPS_TO_SCAN) {
//     if (SKIP_TOP_GROUPS.has(top)) continue;

//     const topDir = path.join(CODE_ROOT, top);
//     if (!isDir(topDir)) continue;

//     // walk all directories and look for .../Web/Src
//     const dirs = walkDirs(topDir);
//     for (const d of dirs) {
//       // Fast check: directory name must be "Src" and parent must be "Web"
//       if (path.basename(d) !== WEB_SRC_NAME) continue;
//       const parent = path.dirname(d);
//       if (path.basename(parent) !== WEB_FOLDER_NAME) continue;

//       // moduleRoot = .../<Group>/<Module>
//       const moduleRoot = path.dirname(parent);

//       // Extra safety: skip if moduleRoot is within Lights tree
//       const relToCode = path.relative(CODE_ROOT, moduleRoot).replace(/\\/g, "/");
//       if (relToCode.startsWith("6_Lights/")) continue;

//       hits.push({
//         group: top,
//         moduleRoot,
//         webSrc: d,
//         webGen: path.join(parent, WEB_GEN_NAME),
//       });
//     }
//   }

//   // Deterministic ordering (useful when logs are compared across machines)
//   hits.sort((a, b) => a.moduleRoot.localeCompare(b.moduleRoot));
//   return hits;
// }

// // -----------------------------------------------------------------------------
// // MAIN (async)
// // -----------------------------------------------------------------------------
// //
// // main()
// //   - discovers all Web/Src directories
// //   - for each, ensures Web/Gen exists
// //   - generates one header per submodule
// //
// async function main() {
//   console.info("PulSar cdata_submodules.js");
//   console.info("Repo: " + REPO_ROOT.replace(/\\/g, "/"));
//   console.info("Code: " + CODE_ROOT.replace(/\\/g, "/"));

//   const hits = findWebSrcDirs();
//   if (!hits.length) {
//     console.info("No Web/Src directories found. Nothing to do.");
//     return;
//   }

//   console.info(`Found ${hits.length} submodule Web/Src directory(ies).`);

//   for (const h of hits) {
//     const srcRel = path.relative(REPO_ROOT, h.webSrc).replace(/\\/g, "/");
//     const genRel = path.relative(REPO_ROOT, h.webGen).replace(/\\/g, "/");
//     const modRel = path.relative(REPO_ROOT, h.moduleRoot).replace(/\\/g, "/");

//     console.info("------------------------------------------------------------");
//     console.info("Module: " + modRel);
//     console.info("Src:    " + srcRel);
//     console.info("Gen:    " + genRel);

//     ensureDir(h.webGen);

//     // Build header for this submodule
//     await buildSubmoduleHeader(h.moduleRoot, h.webSrc, h.webGen);
//   }

//   console.info("Done.");
// }

// // Top-level launcher with fatal handling.
// // If this fails, npm will return non-zero so your python wrapper can decide
// // FAIL_HARD vs continue.
// main().catch((e) => {
//   console.error("FATAL:", e);
//   process.exit(1);
// });
