#include "mFileSystem.h"

#include "3_Network/21_WebServer/Webpages/Generated/html_sdcard_editor.h"

/************************************************************************************************
 * FILE: SDCard_Web.cpp
 *
 * PURPOSE:
 * - SD-backed generated web editor.
 * - Uses generated gzip HTML page:
 *     PAGE_sd_editor
 *     PAGE_sd_editor_length
 *
 * - No SPIFFSEditor.
 * - No SDCardEditor class.
 * - No addHandler(new ...).
 * - No static helper functions.
 *
 * ROUTE:
 * - /sdedit
 *
 * EXPECTED PAGE BACKEND CALLS:
 * - GET    /sdedit                  -> editor page
 * - GET    /sdedit?list=/           -> JSON file list
 * - GET    /sdedit?edit=/file.txt   -> read file
 * - GET    /sdedit?download=/file   -> download file
 * - PUT    /sdedit                  -> create file, FormData "path"
 * - POST   /sdedit                  -> write/upload file, FormData "data"
 * - DELETE /sdedit                  -> delete file, FormData "path"
 *
 * Date Modified: 17May26
 ************************************************************************************************/

#if defined(USE_MODULE_CORE_FILESYSTEM) && defined(USE_MODULE_FILESYSTEM_SDCARD) && defined(USE_MODULE_NETWORK_WEBSERVER)


/************************************************************************************************
 * FUNCTION: WebPage_Root_AddHandlers
 *
 * SUMMARY:
 * - Registers SD editor page/backend routes using normal async callbacks.
 *
 * CHANGED:
 * - 17May26: Implemented generated-page SD editor route without SPIFFSEditor wrapper.
 ************************************************************************************************/

void mFileSystem::WebPage_Root_AddHandlers()
{
  SPGM_CTR(PM_URL_FILESYSTEM_SDEDIT) "/sdedit";

  /************************************************************************************************
   * SECTION: GET /sdedit
   *
   * Handles:
   * - /sdedit
   * - /sdedit?list=/
   * - /sdedit?edit=/file.txt
   * - /sdedit?download=/file.txt
   ************************************************************************************************/

  tkr_web->server->on(PM_URL_FILESYSTEM_SDEDIT, HTTP_GET, [this](AsyncWebServerRequest *request)
  {
    this->Web_SDCardEditor_GET(request);
  });


  /************************************************************************************************
   * SECTION: PUT /sdedit
   *
   * Handles:
   * - create file
   *
   * Expected FormData:
   * - path=/file.txt
   ************************************************************************************************/

  tkr_web->server->on(PM_URL_FILESYSTEM_SDEDIT, HTTP_PUT, [this](AsyncWebServerRequest *request)
  {
    this->Web_SDCardEditor_PUT(request);
  });


  /************************************************************************************************
   * SECTION: DELETE /sdedit
   *
   * Handles:
   * - delete file
   *
   * Expected FormData:
   * - path=/file.txt
   ************************************************************************************************/

  tkr_web->server->on(PM_URL_FILESYSTEM_SDEDIT, HTTP_DELETE, [this](AsyncWebServerRequest *request)
  {
    this->Web_SDCardEditor_DELETE(request);
  });


  /************************************************************************************************
   * SECTION: POST /sdedit
   *
   * Handles:
   * - save/upload file
   *
   * The page sends:
   *   formData.append("data", Blob/File, filename)
   *
   * The upload callback receives filename as the target SD path.
   ************************************************************************************************/

  tkr_web->server->on(
    PM_URL_FILESYSTEM_SDEDIT,
    HTTP_POST,
    [this](AsyncWebServerRequest *request)
    {
      this->Web_SDCardEditor_POST_Final(request);
    },
    [this](
      AsyncWebServerRequest *request,
      const String& filename,
      size_t index,
      uint8_t *data,
      size_t len,
      bool final
    )
    {
      this->Web_SDCardEditor_POST_Upload(request, filename, index, data, len, final);
    }
  );

  AddURLtoList(PM_URL_FILESYSTEM_SDEDIT, HTTP_GET);
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_NormalisePath
 *
 * SUMMARY:
 * - Normalises SD-local paths.
 *
 * EXAMPLES:
 * - "test.txt"  -> "/test.txt"
 * - "/test.txt" -> "/test.txt"
 *
 * CHANGED:
 * - 17May26: Initial version.
 ************************************************************************************************/

bool mFileSystem::Web_SDCardEditor_NormalisePath(String& path)
{
  path.trim();

  if (!path.length()) {
    return false;
  }

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  if (path.indexOf("..") >= 0) {
    return false;
  }

  return true;
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_ContentTypeFromPath
 *
 * SUMMARY:
 * - Resolves browser content type for SD file serving.
 *
 * CHANGED:
 * - 17May26: Initial version.
 ************************************************************************************************/

String mFileSystem::Web_SDCardEditor_ContentTypeFromPath(const String& path)
{
  if (path.endsWith(".html") || path.endsWith(".htm"))     { return F("text/html"); }
  if (path.endsWith(".css"))                               { return F("text/css"); }
  if (path.endsWith(".js"))                                { return F("application/javascript"); }
  if (path.endsWith(".json") || path.endsWith(".ndjson"))  { return F("application/json"); }
  if (path.endsWith(".txt") || path.endsWith(".log"))      { return F("text/plain"); }
  if (path.endsWith(".csv"))                               { return F("text/csv"); }
  if (path.endsWith(".xml"))                               { return F("text/xml"); }
  if (path.endsWith(".png"))                               { return F("image/png"); }
  if (path.endsWith(".jpg") || path.endsWith(".jpeg"))     { return F("image/jpeg"); }
  if (path.endsWith(".gif"))                               { return F("image/gif"); }
  if (path.endsWith(".bmp"))                               { return F("image/bmp"); }
  if (path.endsWith(".ico"))                               { return F("image/x-icon"); }

  return F("application/octet-stream");
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_JSONEscape
 *
 * SUMMARY:
 * - Minimal JSON string escaping for directory listings.
 *
 * CHANGED:
 * - 17May26: Initial version.
 ************************************************************************************************/

String mFileSystem::Web_SDCardEditor_JSONEscape(const String& in)
{
  String out;
  out.reserve(in.length() + 8);

  for (uint16_t i = 0; i < in.length(); i++)
  {
    const char c = in[i];

    switch (c)
    {
      case '\\': out += F("\\\\"); break;
      case '"':  out += F("\\\""); break;
      case '\n': out += F("\\n");  break;
      case '\r': out += F("\\r");  break;
      case '\t': out += F("\\t");  break;

      default:
        if ((uint8_t)c < 32) {
          out += ' ';
        } else {
          out += c;
        }
      break;
    }
  }

  return out;
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_GET
 *
 * SUMMARY:
 * - Handles all GET requests for /sdedit.
 *
 * HANDLES:
 * - /sdedit
 * - /sdedit?list=/
 * - /sdedit?edit=/file.txt
 * - /sdedit?download=/file.txt
 *
 * CHANGED:
 * - 17May26: Initial generated-page SD editor GET handler.
 ************************************************************************************************/

void mFileSystem::Web_SDCardEditor_GET(AsyncWebServerRequest* request)
{
  if (!request) {
    return;
  }

  if (!SDCard_IsMounted())
  {
    if (!SDCard_Mount())
    {
      request->send(503, "text/plain", "SD card not mounted");
      return;
    }
  }


  /************************************************************************************************
   * SECTION: LIST DIRECTORY
   ************************************************************************************************/

  if (request->hasParam("list"))
  {
    String path = request->getParam("list")->value();

    if (!path.length()) {
      path = "/";
    }

    if (!Web_SDCardEditor_NormalisePath(path))
    {
      request->send(400, "application/json", "[]");
      return;
    }

    File dir = SD.open(path);

    if (!dir || !dir.isDirectory())
    {
      if (dir) {
        dir.close();
      }

      request->send(404, "application/json", "[]");
      return;
    }

    String output;
    output.reserve(4096);
    output += '[';

    File entry = dir.openNextFile();

    while (entry)
    {
      String fname = entry.name();

      if (!fname.startsWith("/")) {
        fname = "/" + fname;
      }

      if (output.length() > 1) {
        output += ',';
      }

      output += F("{\"type\":\"");
      output += entry.isDirectory() ? F("dir") : F("file");
      output += F("\",\"name\":\"");
      output += Web_SDCardEditor_JSONEscape(fname);
      output += F("\",\"size\":");
      output += String((uint32_t)entry.size());
      output += '}';

      entry = dir.openNextFile();

      if (output.length() > 20000) {
        break;
      }
    }

    dir.close();

    output += ']';

    request->send(200, "application/json", output);
    return;
  }


  /************************************************************************************************
   * SECTION: READ FILE FOR EDITOR
   ************************************************************************************************/

  if (request->hasParam("edit"))
  {
    String path = request->getParam("edit")->value();

    if (!Web_SDCardEditor_NormalisePath(path))
    {
      request->send(400, "text/plain", "Invalid path");
      return;
    }

    if (!SD.exists(path))
    {
      request->send(404, "text/plain", "File not found");
      return;
    }

    File file = SD.open(path, FILE_READ);

    if (!file)
    {
      request->send(404, "text/plain", "File open failed");
      return;
    }

    if (file.isDirectory())
    {
      file.close();
      request->send(400, "text/plain", "Path is a directory");
      return;
    }

    request->send(
      file,
      path,
      Web_SDCardEditor_ContentTypeFromPath(path),
      false
    );

    return;
  }


  /************************************************************************************************
   * SECTION: DOWNLOAD FILE
   ************************************************************************************************/

  if (request->hasParam("download"))
  {
    String path = request->getParam("download")->value();

    if (!Web_SDCardEditor_NormalisePath(path))
    {
      request->send(400, "text/plain", "Invalid path");
      return;
    }

    if (!SD.exists(path))
    {
      request->send(404, "text/plain", "File not found");
      return;
    }

    File file = SD.open(path, FILE_READ);

    if (!file)
    {
      request->send(404, "text/plain", "File open failed");
      return;
    }

    if (file.isDirectory())
    {
      file.close();
      request->send(400, "text/plain", "Path is a directory");
      return;
    }

    request->send(
      file,
      path,
      Web_SDCardEditor_ContentTypeFromPath(path),
      true
    );

    return;
  }


  /************************************************************************************************
   * SECTION: SEND SD EDITOR PAGE
   ************************************************************************************************/

  const char* buildTime = __DATE__ " " __TIME__ " GMT";

  if (request->header(F("If-Modified-Since")).equals(buildTime))
  {
    request->send(304);
    return;
  }

  AsyncWebServerResponse* response = request->beginResponse_P(
    200,
    "text/html",
    PAGE_sd_editor,
    PAGE_sd_editor_length
  );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Last-Modified"), buildTime);

  request->send(response);
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_PUT
 *
 * SUMMARY:
 * - Creates an empty file on SD.
 *
 * EXPECTED:
 * - FormData:
 *     path=/file.txt
 *
 * CHANGED:
 * - 17May26: Initial generated-page SD editor PUT handler.
 ************************************************************************************************/

void mFileSystem::Web_SDCardEditor_PUT(AsyncWebServerRequest* request)
{
  if (!request) {
    return;
  }

  if (!SDCard_IsMounted())
  {
    if (!SDCard_Mount())
    {
      request->send(503, "text/plain", "SD card not mounted");
      return;
    }
  }

  if (!request->hasParam("path", true))
  {
    request->send(400, "text/plain", "Missing path");
    return;
  }

  String path = request->getParam("path", true)->value();

  if (!Web_SDCardEditor_NormalisePath(path))
  {
    request->send(400, "text/plain", "Invalid path");
    return;
  }

  if (SD.exists(path))
  {
    request->send(200, "text/plain", "Exists: " + path);
    return;
  }

  File file = SD.open(path, FILE_WRITE);

  if (!file)
  {
    request->send(500, "text/plain", "Create failed: " + path);
    return;
  }

  file.close();

  request->send(200, "text/plain", "CREATE: " + path);
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_DELETE
 *
 * SUMMARY:
 * - Deletes a file from SD.
 *
 * EXPECTED:
 * - FormData:
 *     path=/file.txt
 *
 * CHANGED:
 * - 17May26: Initial generated-page SD editor DELETE handler.
 ************************************************************************************************/

void mFileSystem::Web_SDCardEditor_DELETE(AsyncWebServerRequest* request)
{
  if (!request) {
    return;
  }

  if (!SDCard_IsMounted())
  {
    if (!SDCard_Mount())
    {
      request->send(503, "text/plain", "SD card not mounted");
      return;
    }
  }

  if (!request->hasParam("path", true))
  {
    request->send(400, "text/plain", "Missing path");
    return;
  }

  String path = request->getParam("path", true)->value();

  if (!Web_SDCardEditor_NormalisePath(path))
  {
    request->send(400, "text/plain", "Invalid path");
    return;
  }

  if (!SD.exists(path))
  {
    request->send(404, "text/plain", "File not found: " + path);
    return;
  }

  if (SD.remove(path))
  {
    request->send(200, "text/plain", "DELETE: " + path);
  }
  else
  {
    request->send(500, "text/plain", "Delete failed: " + path);
  }
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_POST_Upload
 *
 * SUMMARY:
 * - Handles POST upload/save chunks.
 *
 * EXPECTED FROM PAGE:
 * - FormData:
 *     data = Blob/File, filename = target SD path
 *
 * CHANGED:
 * - 17May26: Initial generated-page SD editor POST upload handler.
 ************************************************************************************************/

void mFileSystem::Web_SDCardEditor_POST_Upload(
  AsyncWebServerRequest* request,
  const String& filename,
  size_t index,
  uint8_t* data,
  size_t len,
  bool final
)
{
  if (!request) {
    return;
  }

  if (!SDCard_IsMounted())
  {
    if (!SDCard_Mount()) {
      return;
    }
  }

  if (!index)
  {
    String path = filename;

    if (!Web_SDCardEditor_NormalisePath(path)) {
      return;
    }

    request->_tempFile = SD.open(path, FILE_WRITE);
  }

  if (request->_tempFile)
  {
    if (len) {
      request->_tempFile.write(data, len);
    }

    if (final) {
      request->_tempFile.close();
    }
  }
}


/************************************************************************************************
 * FUNCTION: Web_SDCardEditor_POST_Final
 *
 * SUMMARY:
 * - Final POST response after upload/save.
 *
 * CHANGED:
 * - 17May26: Initial generated-page SD editor POST final handler.
 ************************************************************************************************/

void mFileSystem::Web_SDCardEditor_POST_Final(AsyncWebServerRequest* request)
{
  if (!request) {
    return;
  }

  request->send(200, "text/plain", "OK");
}

#endif // USE_MODULE_CORE_FILESYSTEM && USE_MODULE_FILESYSTEM_SDCARD && USE_MODULE_NETWORK_WEBSERVER