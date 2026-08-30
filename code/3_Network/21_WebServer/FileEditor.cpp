#include "FileEditor.h"
#include <FS.h>

#include "Webpages/Generated/file_editor.h"

#define FILEEDITOR_MAXLENGTH_FILEPATH 32

volatile bool FileEditor::s_fileChanged = false;
String FileEditor::s_lastFile;


/* Exclusion list feature not needed and omitted */

// WEB HANDLER IMPLEMENTATION

FileEditor::FileEditor(const fs::FS& fs, const String& username, const String& password)
:_fs(fs)
,_username(username)
,_password(password)
,_authenticated(false)
,_startTime(0)
{}


bool FileEditor::canHandle(AsyncWebServerRequest *request)
{

  ALOG_INF(PSTR("FileEditor"));

  if(request->url().equalsIgnoreCase(F("/edit")))
  {
    if(request->method() == HTTP_GET)
    {
      if(request->hasParam("list")) return true;

      if(request->hasParam("edit"))
      {
        if(request->arg("edit").indexOf("wsec") > -1) return false;

        request->_tempFile = _fs.open(request->arg("edit"), "r");
        if(!request->_tempFile) return false;

        #ifdef ESP32
        if(request->_tempFile.isDirectory())
        {
          request->_tempFile.close();
          return false;
        }
        #endif
      }

      if(request->hasParam(F("download")))
      {
        if(request->arg(F("download")).indexOf("wsec") > -1) return false;

        request->_tempFile = _fs.open(request->arg(F("download")), "r");
        if(!request->_tempFile) return false;

        #ifdef ESP32
        if(request->_tempFile.isDirectory())
        {
          request->_tempFile.close();
          return false;
        }
        #endif
      }

      request->addInterestingHeader(F("If-Modified-Since"));
      return true;
    }
    else if(request->method() == HTTP_POST) return true;
    else if(request->method() == HTTP_DELETE) return true;
    else if(request->method() == HTTP_PUT) return true;
  }

  return false;
}


void FileEditor::ServePage(AsyncWebServerRequest *request)
{
  if(!request) return;

  const char *buildTime = __DATE__ " " __TIME__ " GMT";

  if(request->header(F("If-Modified-Since")).equals(buildTime))
  {
    request->send(304);
    return;
  }

  AsyncWebServerResponse *response = request->beginResponse_P(200, FPSTR(CONTENT_TYPE_HTML), PAGE_file_editor_web, PAGE_file_editor_web_length);
  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Last-Modified"), buildTime);
  request->send(response);
}


void FileEditor::handleRequest(AsyncWebServerRequest *request)
{
  if(_username.length() && _password.length() && !request->authenticate(_username.c_str(), _password.c_str()))
  {
    return request->requestAuthentication();
  }

  if(request->method() == HTTP_GET)
  {
    if(request->hasParam("list"))
    {
      String path = request->getParam("list")->value();

      #ifdef ESP32
      File dir = _fs.open(path);
      #else
      Dir dir = _fs.openDir(path);
      #endif

      path = String();
      String output = "[";

      #ifdef ESP32
      File entry = dir.openNextFile();
      while(entry)
      #else
      while(dir.next())
      {
        fs::File entry = dir.openFile("r");
      #endif
      {
        String fname = entry.name();

        if(fname.indexOf("wsec") == -1)
        {
          if(output != "[") output += ',';

          output += F("{\"type\":\"file\",\"name\":\"");
          if(fname[0] != '/') output += '/';
          output += fname;
          output += F("\",\"size\":");
          output += String(entry.size());
          output += '}';
        }

        #ifdef ESP32
        entry = dir.openNextFile();
        #else
        entry.close();
        #endif
      }

      #ifdef ESP32
      dir.close();
      #endif

      output += ']';
      request->send(200, FPSTR(CONTENT_TYPE_JSON), output);
      output = String();
      return;
    }

    if(request->hasParam("edit") || request->hasParam(F("download")))
    {
      request->send(request->_tempFile, request->_tempFile.name(), String(), request->hasParam(F("download")));
      return;
    }

    ServePage(request);
    return;
  }

  if(request->method() == HTTP_DELETE)
  {
    if(request->hasParam("path", true))
    {
      String filename = request->getParam("path", true)->value();
      _fs.remove(filename);
      request->send(200, "", "DELETE: " + filename);
    }
    else
    {
      request->send(404);
    }

    return;
  }

  if(request->method() == HTTP_POST)
  {
    if(request->hasParam("data", true, true))
    {
      String fname = request->getParam("data", true, true)->value();

      if(_fs.exists(fname))
      {
        FileEditor::s_lastFile = fname;
        FileEditor::s_fileChanged = true;

        Serial.printf("FileEditor: File changed: %s\n", fname.c_str());

        request->send(200, "", "UPLOADED: " + fname);
      }
      else
      {
        request->send(500);
      }
    }
    else
    {
      request->send(500);
    }

    return;
  }

  if(request->method() == HTTP_PUT)
  {
    if(request->hasParam("path", true))
    {
      String filename = request->getParam("path", true)->value();

      if(_fs.exists(filename))
      {
        request->send(200);
      }
      else
      {
        fs::File f = _fs.open(filename, "w");

        if(f)
        {
          f.write((uint8_t)0x00);
          f.close();

          FileEditor::s_lastFile = filename;
          FileEditor::s_fileChanged = true;

          request->send(200, "", "CREATE: " + filename);
        }
        else
        {
          request->send(500);
        }
      }
    }
    else
    {
      request->send(400);
    }

    return;
  }
}


void FileEditor::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
{
  if(!index)
  {
    if(!_username.length() || request->authenticate(_username.c_str(), _password.c_str()))
    {
      _authenticated = true;
      request->_tempFile = _fs.open(filename, "w");
      _startTime = millis();
    }
  }

  if(_authenticated && request->_tempFile)
  {
    if(len) request->_tempFile.write(data, len);

    if(final)
    {
      request->_tempFile.close();
    }
  }
}