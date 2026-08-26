/**
 * Writes compressed C arrays of data files (web interface)
 *
 * 1) Install Node 11+ and npm
 * 2) npm install
 * 3) npm run build:webserver
 *
 * Uses NodeJS packages to minify and GZIP WebUI files.
 */

const fs = require("fs");
const path = require("path");
const os = require("os");
const zlib = require("zlib");
const CleanCSS = require("clean-css");
const MinifyHTML = require("html-minifier-terser").minify;
const packageJson = require("../package.json");
const { execSync } = require("child_process");

let source_path      = "code/3_Network/21_WebServer/Webpages/Source";
let destination_path = "code/3_Network/21_WebServer/Webpages/Generated/";

function ensureDir(p) {
  if (!fs.existsSync(p)) fs.mkdirSync(p, { recursive: true });
}

ensureDir(destination_path);


/************************************************************************************************
 * Build metadata
 ************************************************************************************************/

function strReplace(str, search, replacement) {
  return str.split(search).join(replacement);
}

function getGitBranch() {
  try {
    return execSync("git rev-parse --abbrev-ref HEAD", {
      encoding: "utf8",
      stdio: ["ignore", "pipe", "ignore"]
    }).trim();
  } catch {
    return "";
  }
}

function getGitCommit() {
  try {
    return execSync("git rev-parse --short HEAD", {
      encoding: "utf8",
      stdio: ["ignore", "pipe", "ignore"]
    }).trim();
  } catch {
    return "";
  }
}

function findFirmwareVersionHeader(startDir = "code") {
  function searchDirectory(dir) {
    let entries;

    try {
      entries = fs.readdirSync(dir, { withFileTypes: true });
    } catch {
      return "";
    }

    for (const entry of entries) {
      const fullPath = path.join(dir, entry.name);

      if (entry.isDirectory()) {
        const found = searchDirectory(fullPath);
        if (found) return found;
        continue;
      }

      if (!entry.isFile()) continue;
      if (entry.name.toLowerCase() !== "version.h") continue;

      try {
        const src = fs.readFileSync(fullPath, "utf8");

        if (
          src.includes("FIRMWARE_VERSION_MAJOR") &&
          src.includes("FIRMWARE_VERSION_MINOR") &&
          src.includes("FIRMWARE_VERSION_CORE") &&
          src.includes("FIRMWARE_VERSION_MODULE")
        ) {
          return fullPath;
        }
      } catch {
      }
    }

    return "";
  }

  return searchDirectory(startDir);
}

function getFirmwareVersion() {
  const versionPath = findFirmwareVersionHeader();

  if (!versionPath) {
    console.warn("Firmware version header not found");

    return {
      major: "",
      minor: "",
      core: "",
      module: "",
      branch: "",
      version: ""
    };
  }

  console.info("Firmware version header: " + versionPath);

  const src = fs.readFileSync(versionPath, "utf8");

  function getDefine(name) {
    const match = src.match(
      new RegExp("^\\s*#define\\s+" + name + "\\s+([^\\s/]+)", "m")
    );

    return match ? match[1].trim() : "";
  }

  function getStringDefine(name) {
    const match = src.match(
      new RegExp("^\\s*#define\\s+" + name + "\\s+\"([^\"]*)\"", "m")
    );

    return match ? match[1].trim() : "";
  }

  const major = getDefine("FIRMWARE_VERSION_MAJOR");
  const minor = getDefine("FIRMWARE_VERSION_MINOR");
  const core = getDefine("FIRMWARE_VERSION_CORE");
  const module = getDefine("FIRMWARE_VERSION_MODULE");
  const branch = getStringDefine("FIRMWARE_BRANCH_NAME_STRING");

  return {
    major,
    minor,
    core,
    module,
    branch,
    version: [major, minor, core, module].join(".")
  };
}

function pad2(value) {
  return String(value).padStart(2, "0");
}

function getBuildDateTime() {
  if (process.env.BUILD_DATETIME) return process.env.BUILD_DATETIME;

  const now = new Date();

  return (
    now.getFullYear() + "-" +
    pad2(now.getMonth() + 1) + "-" +
    pad2(now.getDate()) + " " +
    pad2(now.getHours()) + ":" +
    pad2(now.getMinutes()) + ":" +
    pad2(now.getSeconds())
  );
}

function getBuildEnvironment() {
  const env =
    process.env.PIOENV ||
    process.env.BUILD_ENV ||
    process.env.PLATFORMIO_ENV ||
    process.env.PLATFORMIO_ENVIRONMENT ||
    process.env.npm_config_pioenv ||
    "";

  if (env) return env;

  for (let i = 0; i < process.argv.length; i++) {
    const arg = process.argv[i];

    if ((arg === "-e" || arg === "--environment") && process.argv[i + 1]) {
      return process.argv[i + 1];
    }

    if (arg.startsWith("--environment=")) {
      return arg.substring("--environment=".length);
    }

    if (arg.startsWith("--env=")) {
      return arg.substring("--env=".length);
    }
  }

  return "unknown";
}

const firmwareMetadata = getFirmwareVersion();
const buildMetadata = {
  project: packageJson.name || "PulSar",
  version: firmwareMetadata.version,
  firmwareBranch: firmwareMetadata.branch,
  buildDateTime: getBuildDateTime(),
  buildEnvironment: getBuildEnvironment(),
  buildHost: os.hostname(),
  gitBranch: process.env.GIT_BRANCH || getGitBranch(),
  gitCommit: process.env.GIT_COMMIT || getGitCommit(),
  repo: packageJson.repository?.url
    ?.replace(/^git\+/, "")
    ?.replace(/\.git$/, "") || ""
};

console.info("");
console.info("Build metadata:");
console.info("  Project         : " + buildMetadata.project);
console.info("  Version         : " + buildMetadata.version);
console.info("  Firmware Branch : " + buildMetadata.firmwareBranch);
console.info("  Build           : " + buildMetadata.buildDateTime);
console.info("  Environment     : " + buildMetadata.buildEnvironment);
console.info("  Host            : " + buildMetadata.buildHost);
console.info("  Git Branch      : " + buildMetadata.gitBranch);
console.info("  Git Commit      : " + buildMetadata.gitCommit);
console.info("");

function adoptBuildMetadata(str) {
  const values = {
    "##PROJECT##":         buildMetadata.project,
    "##VERSION##":         buildMetadata.version,
    "##FIRMWARE_BRANCH##": buildMetadata.firmwareBranch,
    "##BUILD_DATETIME##":  buildMetadata.buildDateTime,
    "##BUILD_DATE##":      buildMetadata.buildDateTime,
    "##BUILD_ENV##":       buildMetadata.buildEnvironment,
    "##BUILD_HOST##":      buildMetadata.buildHost,
    "##GIT_BRANCH##":      buildMetadata.gitBranch,
    "##GIT_COMMIT##":      buildMetadata.gitCommit,
    "##REPO##":            buildMetadata.repo,
  };

  for (const [key, value] of Object.entries(values)) str = strReplace(str, key, value);
  return str;
}


/************************************************************************************************
 * Conversion helpers
 ************************************************************************************************/

function hexdump(buffer, isHex = false) {
  let lines = [];

  for (let i = 0; i < buffer.length; i += (isHex ? 32 : 16)) {
    let block;
    let hexArray = [];

    if (isHex) {
      block = buffer.slice(i, i + 32);
      for (let j = 0; j < block.length; j += 2) hexArray.push("0x" + block.slice(j, j + 2));
    } else {
      block = buffer.slice(i, i + 16);
      for (let value of block) hexArray.push("0x" + value.toString(16).padStart(2, "0"));
    }

    lines.push(`  ${hexArray.join(", ")}`);
  }

  return lines.join(",\n");
}

function filter(str, type) {
  str = adoptBuildMetadata(str);

  if (type === undefined) {
    return str;
  }
  else if (type == "css-minify") {
    return new CleanCSS({}).minify(str).styles;
  }
  else if (type == "js-minify") {
    return MinifyHTML("<script>" + str + "</script>", {
      collapseWhitespace: true,
      minifyJS: true,
      continueOnParseError: false,
      removeComments: true,
    }).replace(/<[\/]*script>/g, "");
  }
  else if (type == "html-minify") {
    return MinifyHTML(str, {
      collapseWhitespace: true,
      maxLineLength: 80,
      minifyCSS: true,
      minifyJS: true,
      continueOnParseError: false,
      removeComments: true,
    });
  }
  else if (type == "html-minify-ui") {
    return MinifyHTML(str, {
      collapseWhitespace: true,
      conservativeCollapse: true,
      maxLineLength: 80,
      minifyCSS: true,
      minifyJS: true,
      continueOnParseError: false,
      removeComments: true,
    });
  }

  console.warn("Unknown filter: " + type);
  return str;
}

function writeHtmlGzipped(sourceFile, resultFile, page) {
  console.info("Reading " + sourceFile);

  let html = fs.readFileSync(sourceFile, "utf8");
  console.info("Read " + html.length + " characters");

  html = filter(html, "html-minify-ui");
  console.info("Minified to " + html.length + " characters");

  const result = zlib.gzipSync(html, { level: zlib.constants.Z_BEST_COMPRESSION });
  const array = hexdump(result);

  const src = `/*
 * Binary array for the Web UI.
 * gzip is used for smaller size and improved speeds.
 */

#pragma once

// Autogenerated from ${sourceFile}, do not edit!!
const uint16_t PAGE_${page}_L = ${result.length};
const uint8_t PAGE_${page}[] PROGMEM = {
${array}
};
`;

  console.info(">>>Writing " + resultFile);
  fs.writeFileSync(resultFile, src);
}

function specToChunk(srcDir, s) {
  if (s.method == "plaintext") {
    const buf = fs.readFileSync(srcDir + "/" + s.file);
    const str = buf.toString("utf-8");

    const chunk = `

// Autogenerated from ${srcDir}/${s.file}, do not edit!!
const char ${s.name}[] PROGMEM = R"${s.prepend || ""}${filter(str, s.filter)}${s.append || ""}";

`;

    return s.mangle ? s.mangle(chunk) : chunk;
  }

  else if (s.method == "gzip") {
    const buf = fs.readFileSync(srcDir + "/" + s.file);
    let str = buf.toString("utf-8");

    if (s.mangle) str = s.mangle(str);

    const zip = zlib.gzipSync(filter(str, s.filter), { level: zlib.constants.Z_BEST_COMPRESSION });
    const result = hexdump(zip.toString("hex"), true);

    return `
// Autogenerated from ${srcDir}/${s.file}, do not edit!!
const uint16_t ${s.name}_length = ${zip.length};
const uint8_t ${s.name}[] PROGMEM = {
${result}
};

`;
  }

  else if (s.method == "binary") {
    const buf = fs.readFileSync(srcDir + "/" + s.file);
    const result = hexdump(buf);

    return `
// Autogenerated from ${srcDir}/${s.file}, do not edit!!
const uint16_t ${s.name}_length = ${buf.length};
const uint8_t ${s.name}[] PROGMEM = {
${result}
};

`;
  }

  console.warn("Unknown method: " + s.method);
  return undefined;
}

function writeChunks(srcDir, specs, resultFile) {
  let src = `/*
 * This file is auto generated, please don't make any changes manually.
 * Generator: cdata_webserver.js
 */
#pragma once
`;

  specs.forEach((s) => {
    try {
      console.info("Reading " + srcDir + "/" + s.file + " as " + s.name);
      src += specToChunk(srcDir, s);
    } catch (e) {
      console.warn(
        "Failed " + s.name + " from " + srcDir + "/" + s.file,
        e.message.length > 60 ? e.message.substring(0, 60) : e.message
      );
    }
  });

  console.info(">>>Writing " + src.length + " characters into " + resultFile);
  fs.writeFileSync(resultFile, src);
}


/************************************************************************************************
 * Main WebUI
 ************************************************************************************************/

writeHtmlGzipped(
  source_path + "/root_main.htm",
  destination_path + "root_main.h",
  "root_main_web"
);


/************************************************************************************************
 * Settings
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "style.css",
      name: "PAGE_settingsCss_web",
      method: "gzip",
      filter: "css-minify",
      mangle: (str) => str.replace("%%", "%")
    },
    {
      file: "common.js",
      name: "JS_common_web",
      method: "gzip",
      filter: "js-minify",
    },
    {
      file: "settings.htm",
      name: "PAGE_settings_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_network.htm",
      name: "PAGE_settings_network_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_hardware.htm",
      name: "PAGE_settings_hardware_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_system.htm",
      name: "PAGE_settings_system_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_modules.htm",
      name: "PAGE_settings_modules_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_storage.htm",
      name: "PAGE_settings_storage_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_logging.htm",
      name: "PAGE_settings_logging_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_security.htm",
      name: "PAGE_settings_security_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_unlock.htm",
      name: "PAGE_settings_unlock_web",
      method: "gzip",
      filter: "html-minify",
    }
  ],
  destination_path + "html_settings.h"
);


/************************************************************************************************
 * Other core pages
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "msg.htm",
      name: "PAGE_msg_web",
      prepend: "=====(",
      append: ")=====",
      method: "plaintext",
      filter: "html-minify",
      mangle: (str) => str.replace(/\<h2\>.*\<\/body\>/gms, "<h2>%MSG%</body>"),
    },
    {
      file: "update.htm",
      name: "PAGE_update_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "welcome.htm",
      name: "PAGE_welcome_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "404.htm",
      name: "PAGE_404_web",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "favicon_pulsar.ico",
      name: "favicon2_web",
      method: "binary",
    }
  ],
  destination_path + "html_other.h"
);


/************************************************************************************************
 * Consoles
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "Consoles/console_ws.htm",
      name: "PAGE_console_ws_web",
      method: "gzip",
      filter: "html-minify-ui",
    },
    {
      file: "Consoles/console_polling.htm",
      name: "PAGE_console_polling_web",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "pages_console_esp32.h"
);

writeChunks(
  source_path,
  [
    {
      file: "Consoles/console_polling_esp8266.htm",
      name: "PAGE_console_polling",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "pages_console_esp8266.h"
);


/************************************************************************************************
 * Debug / utility pages
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "url_list.htm",
      name: "PAGE_url_list",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "pages_url_debugs.h"
);

writeChunks(
  source_path,
  [
    {
      file: "sd_editor.htm",
      name: "PAGE_sd_editor",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "html_sdcard_editor.h"
);

/*** Generate only once when change is needed. Then copy and insert into library */
writeChunks(
  source_path,
  [
    {
      file: "edit_fs_darkmode.htm",
      name: "PAGE_sedit_fs_darkmode",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "html_edit_fs_darkmode.h"
);


/************************************************************************************************
 * Shared submodule assets/pages
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "submodule_style.css",
      name: "PAGE_submodule_style_web",
      method: "gzip",
      filter: "css-minify",
      mangle: (str) => str.replace(/%%/g, "%")
    }
  ],
  destination_path + "submodule_assets.h"
);

// writeChunks(
//   source_path,
//   [
//     {
//       file: "system_controls.htm",
//       name: "PAGE_system_controls_web",
//       method: "gzip",
//       filter: "html-minify-ui",
//     }
//   ],
//   destination_path + "submodule_unified_pages.h"
// );

writeChunks(
  source_path,
  [
    {
      file: "debug_main.htm",
      name: "PAGE_debug_main_web",
      method: "gzip",
      filter: "html-minify-ui",
    },
    {
      file: "debug_telemetry.htm",
      name: "PAGE_debug_telemetry_web",
      method: "gzip",
      filter: "html-minify-ui",
    },
    {
      file: "debug_telemetry_metrics.htm",
      name: "PAGE_debug_telemetry_metrics_web",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "debug_pages.h"
);




writeChunks(
  source_path,
  [
    {
      file: "file_editor.htm",
      name: "PAGE_file_editor_web",
      method: "gzip",
      filter: "html-minify-ui",
    }
  ],
  destination_path + "file_editor.h"
);
