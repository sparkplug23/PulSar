#include "ContentTypes.h"

static inline bool matches_p(const char* str, const char* progmem_str) {
  return strcmp_P(str, progmem_str) == 0;
}

const __FlashStringHelper* contentTypeFor(const String& path) {
  // Find extension part of path
  auto idx = path.lastIndexOf('.');
  if (idx >= 0) {
    auto ext_str = path.begin() + idx + 1;

    if (matches_p(ext_str, HTML_EXTENSION)) return FPSTR(CONTENT_TYPE_HTML);
    else if (matches_p(ext_str, HTM_EXTENSION)) return FPSTR(CONTENT_TYPE_HTML);
    else if (matches_p(ext_str, CSS_EXTENSION)) return FPSTR(CONTENT_TYPE_CSS);
    else if (matches_p(ext_str, JSON_EXTENSION)) return FPSTR(CONTENT_TYPE_JSON);
    else if (matches_p(ext_str, JS_EXTENSION)) return FPSTR(CONTENT_TYPE_JAVASCRIPT);
    else if (matches_p(ext_str, PNG_EXTENSION)) return FPSTR(CONTENT_TYPE_PNG);
    else if (matches_p(ext_str, GIF_EXTENSION)) return FPSTR(CONTENT_TYPE_GIF);
    else if (matches_p(ext_str, JPG_EXTENSION)) return FPSTR(CONTENT_TYPE_JPEG);
    else if (matches_p(ext_str, ICO_EXTENSION)) return FPSTR(CONTENT_TYPE_XICON);
    else if (matches_p(ext_str, SVG_EXTENSION)) return FPSTR(CONTENT_TYPE_SVG);
    else if (matches_p(ext_str, EOT_EXTENSION)) return FPSTR(CONTENT_TYPE_EOT);
    else if (matches_p(ext_str, WOFF_EXTENSION)) return FPSTR(CONTENT_TYPE_WOFF);
    else if (matches_p(ext_str, WOFF2_EXTENSION)) return FPSTR(CONTENT_TYPE_WOFF2);
    else if (matches_p(ext_str, TTF_EXTENSION)) return FPSTR(CONTENT_TYPE_TTF);
    else if (matches_p(ext_str, XML_EXTENSION)) return FPSTR(CONTENT_TYPE_XML);
    else if (matches_p(ext_str, PDF_EXTENSION)) return FPSTR(CONTENT_TYPE_PDF);
    else if (matches_p(ext_str, ZIP_EXTENSION)) return FPSTR(CONTENT_TYPE_ZIP);
    else if (matches_p(ext_str, GZIP_EXTENSION)) return FPSTR(CONTENT_TYPE_GZIP);    
  } 
  return FPSTR(CONTENT_TYPE_PLAIN);
}

const char CONTENT_TYPE_PLAIN[] PROGMEM = "text/plain";
const char HTM_EXTENSION[] PROGMEM = "htm";
const char HTML_EXTENSION[] PROGMEM = "html";
const char CONTENT_TYPE_HTML[] PROGMEM = "text/html";
const char CSS_EXTENSION[] PROGMEM = "css";
const char CONTENT_TYPE_CSS[] PROGMEM = "text/css";
const char JSON_EXTENSION[] PROGMEM = "json";
const char CONTENT_TYPE_JSON[] PROGMEM = "application/json";
const char JS_EXTENSION[] PROGMEM = "js";
const char CONTENT_TYPE_JAVASCRIPT[] PROGMEM = "application/javascript";
const char PNG_EXTENSION[] PROGMEM = "png";
const char CONTENT_TYPE_PNG[] PROGMEM = "image/png";
const char GIF_EXTENSION[] PROGMEM = "gif";
const char CONTENT_TYPE_GIF[] PROGMEM = "image/gif";
const char JPG_EXTENSION[] PROGMEM = "jpg";
const char CONTENT_TYPE_JPEG[] PROGMEM = "image/jpeg";
const char ICO_EXTENSION[] PROGMEM = "ico";
const char CONTENT_TYPE_XICON[] PROGMEM = "image/x-icon";
const char SVG_EXTENSION[] PROGMEM = "svg";
const char CONTENT_TYPE_SVG[] PROGMEM = "image/svg+xml";
const char EOT_EXTENSION[] PROGMEM = "eot";
const char CONTENT_TYPE_EOT[] PROGMEM = "font/eot";
const char WOFF_EXTENSION[] PROGMEM = "woff";
const char CONTENT_TYPE_WOFF[] PROGMEM = "font/woff";
const char WOFF2_EXTENSION[] PROGMEM = "woff2";
const char CONTENT_TYPE_WOFF2[] PROGMEM = "font/woff2";
const char TTF_EXTENSION[] PROGMEM = "ttf";
const char CONTENT_TYPE_TTF[] PROGMEM = "font/ttf";
const char XML_EXTENSION[] PROGMEM = "xml";
const char CONTENT_TYPE_XML[] PROGMEM = "text/xml";
const char PDF_EXTENSION[] PROGMEM = "pdf";
const char CONTENT_TYPE_PDF[] PROGMEM = "application/pdf";
const char ZIP_EXTENSION[] PROGMEM = "zip";
const char CONTENT_TYPE_ZIP[] PROGMEM = "application/zip";
const char GZIP_EXTENSION[] PROGMEM = "gz";
const char CONTENT_TYPE_GZIP[] PROGMEM = "application/x-gzip";
