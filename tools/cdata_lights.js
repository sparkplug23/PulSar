/**
 * Writes compressed C arrays of data files (web interface)
 * How to use it?
 *
 * 1) Install Node 11+ and npm
 * 2) npm install
 * 3) npm run build:lights
 *
 * "npm run build:lights" to run this file directly and generate the *.h files in
 * code/6_Lights/03_Animator/webpages_generated/
 *
 * If you change data folder often, you can run it in monitoring mode:
 *
 * > npm run dev
 *
 * It uses NodeJS packages to inline, minify and GZIP files.
 */

const fs = require("fs");
const zlib = require("zlib");
const CleanCSS = require("clean-css");
const MinifyHTML = require("html-minifier-terser").minify;
const packageJson = require("../package.json");
const { execSync } = require("child_process");
const inliner = require("inliner");

let source_path      = "code/6_Lights/03_Animator/source";
let destination_path = "code/6_Lights/03_Animator/webpages_generated/";

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

function adoptBuildMetadata(str) {
  const repo = packageJson.repository?.url
    ?.replace(/^git\+/, "")
    ?.replace(/\.git$/, "");

  const values = {
    "##PROJECT##":    packageJson.name || "PulSar",
    "##VERSION##":    packageJson.version || "",
    "##BUILD_DATE##": process.env.BUILD_DATE || new Date().toISOString().slice(0, 10),
    "##BUILD_ENV##":  process.env.PIOENV || process.env.BUILD_ENV || "",
    "##GIT_BRANCH##": process.env.GIT_BRANCH || getGitBranch(),
    "##GIT_COMMIT##": process.env.GIT_COMMIT || getGitCommit(),
    "##REPO##":       repo || "",
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

      for (let j = 0; j < block.length; j += 2) {
        hexArray.push("0x" + block.slice(j, j + 2));
      }
    } else {
      block = buffer.slice(i, i + 16);

      for (let value of block) {
        hexArray.push("0x" + value.toString(16).padStart(2, "0"));
      }
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


function writeHtmlGzipped(sourceFile, resultFile, page2) {
  const page = page2;

  console.info("Reading " + sourceFile);

  new inliner(sourceFile, function (error, html) {
    if (error) {
      console.warn(error);
      throw error;
    }

    console.info("Inlined " + html.length + " characters");

    html = filter(html, "html-minify-ui");

    console.info("Minified to " + html.length + " characters");

    zlib.gzip(html, { level: zlib.constants.Z_BEST_COMPRESSION }, function (error, result) {
      if (error) {
        console.warn(error);
        throw error;
      }

      console.info("Compressed " + result.length + " bytes");

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

      console.info("Writing " + resultFile);
      fs.writeFileSync(resultFile, src);
    });
  });
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

    const zip = zlib.gzipSync(
      filter(str, s.filter),
      { level: zlib.constants.Z_BEST_COMPRESSION }
    );

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
 * This file is auto generated, please don't make any changes manually via "cdata_lights.js and npm run"
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

  console.info("Writing " + src.length + " characters into " + resultFile);
  fs.writeFileSync(resultFile, src);
}


/************************************************************************************************
 * Main Lights UI
 ************************************************************************************************/

writeHtmlGzipped(
  source_path + "/index.htm",
  destination_path + "html_ui.h",
  "index"
);

writeHtmlGzipped(
  source_path + "/pixart/pixart.htm",
  destination_path + "html_pixart.h",
  "pixart"
);

writeHtmlGzipped(
  source_path + "/cpal/cpal.htm",
  destination_path + "html_cpal.h",
  "cpal"
);

writeHtmlGzipped(
  source_path + "/pxmagic/pxmagic.htm",
  destination_path + "html_pxmagic.h",
  "pxmagic"
);


/************************************************************************************************
 * Lights settings
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "style.css",
      name: "PAGE_settingsCss",
      method: "gzip",
      filter: "css-minify",
      mangle: (str) => str.replace("%%", "%")
    },
    {
      file: "common.js",
      name: "JS_common",
      method: "gzip",
      filter: "js-minify",
    },
    {
      file: "settings.htm",
      name: "PAGE_settings",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_leds.htm",
      name: "PAGE_settings_leds",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_dmx.htm",
      name: "PAGE_settings_dmx",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_ui.htm",
      name: "PAGE_settings_ui",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_sync.htm",
      name: "PAGE_settings_sync",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "settings_2D.htm",
      name: "PAGE_settings_2D",
      method: "gzip",
      filter: "html-minify",
    }
  ],
  destination_path + "html_settings.h"
);


/************************************************************************************************
 * Other Lights pages/assets
 ************************************************************************************************/

writeChunks(
  source_path,
  [
    {
      file: "msg.htm",
      name: "PAGE_msg",
      prepend: "=====(",
      append: ")=====",
      method: "plaintext",
      filter: "html-minify",
      mangle: (str) => str.replace(/\<h2\>.*\<\/body\>/gms, "<h2>%MSG%</body>"),
    },
    {
      file: "dmxmap.htm",
      name: "PAGE_dmxmap",
      prepend: "=====(",
      append: ")=====",
      method: "plaintext",
      filter: "html-minify",
      mangle: (str) => `
#ifdef ENABLE_FEATURE_LIGHTING__DMX
${str.replace(/function FM\(\)[ ]?\{/gms, "function FM() {%DMXVARS%\n")}
#else
const char PAGETEST_dmxmap_lights[] PROGMEM = R"=====()=====";
#endif
`,
    },
    {
      file: "liveview.htm",
      name: "PAGE_liveview",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "liveviewws2D.htm",
      name: "PAGE_liveviewws2D",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "virtualview.htm",
      name: "PAGE_virtualview",
      method: "gzip",
      filter: "html-minify",
    },
    {
      file: "favicon_pulsar.ico",
      name: "favicon",
      method: "binary",
    }
  ],
  destination_path + "html_other.h"
);


writeChunks(
  source_path,
  [
    {
      file: "iro.js",
      name: "JS_iro",
      method: "gzip",
      filter: "plain", // no minification, it is already minified
      mangle: (s) => s.replace(/^\/\*![\s\S]*?\*\//, '') // remove license comment at the top
    }
  ],
  destination_path + "js_iro.h"
);

