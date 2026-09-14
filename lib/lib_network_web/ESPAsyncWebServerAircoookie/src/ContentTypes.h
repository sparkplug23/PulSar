/* ContentTypes.h 

A global collection of PROGMEM content type strings, usable by applications

*/

#include "Arduino.h"

// Convenient lookup function
const __FlashStringHelper* contentTypeFor(const String& filename);

// These are all PROGMEM-type variables; use accordingly
extern const char CONTENT_TYPE_PLAIN[];
extern const char HTM_EXTENSION[];
extern const char HTML_EXTENSION[];
extern const char CONTENT_TYPE_HTML[];
extern const char CSS_EXTENSION[];
extern const char CONTENT_TYPE_CSS[];
extern const char JSON_EXTENSION[];
extern const char CONTENT_TYPE_JSON[];
extern const char JS_EXTENSION[];
extern const char CONTENT_TYPE_JAVASCRIPT[];
extern const char PNG_EXTENSION[];
extern const char CONTENT_TYPE_PNG[];
extern const char GIF_EXTENSION[];
extern const char CONTENT_TYPE_GIF[];
extern const char JPG_EXTENSION[];
extern const char CONTENT_TYPE_JPEG[];
extern const char ICO_EXTENSION[];
extern const char CONTENT_TYPE_XICON[];
extern const char SVG_EXTENSION[];
extern const char CONTENT_TYPE_SVG[];
extern const char EOT_EXTENSION[];
extern const char CONTENT_TYPE_EOT[];
extern const char WOFF_EXTENSION[];
extern const char CONTENT_TYPE_WOFF[];
extern const char WOFF2_EXTENSION[];
extern const char CONTENT_TYPE_WOFF2[];
extern const char TTF_EXTENSION[];
extern const char CONTENT_TYPE_TTF[];
extern const char XML_EXTENSION[];
extern const char CONTENT_TYPE_XML[];
extern const char PDF_EXTENSION[];
extern const char CONTENT_TYPE_PDF[];
extern const char ZIP_EXTENSION[];
extern const char CONTENT_TYPE_ZIP[];
extern const char GZIP_EXTENSION[];
extern const char CONTENT_TYPE_GZIP[];
