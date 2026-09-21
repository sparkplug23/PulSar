#ifndef FILEEDITOR_H_
#define FILEEDITOR_H_

#include <ESPAsyncWebServer.h>

#include "3_Network/21_WebServer/Webpages/Generated/file_editor.h"

#include "2_CoreSystem/08_Logging/mLogging.h"

#ifdef ESP8266
#include <LittleFS.h>
#endif

class FileEditor : public AsyncWebHandler {
  private:
    fs::FS _fs;
    String _username;
    String _password;
    bool _authenticated;
    uint32_t _startTime;

  public:
    FileEditor(const fs::FS& fs, const String& username=String(), const String& password=String());

    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final;
    virtual bool isRequestHandlerTrivial() override final { return false; }

    static void ServePage(AsyncWebServerRequest *request);

    static volatile bool s_fileChanged;
    static String s_lastFile;

    static bool Check_AnyFilesEdited()
    {
      return s_fileChanged;
    }

    static bool Check_FileEditedIs(const String &name)
    {
      if(!s_fileChanged) return false;

      if(s_lastFile == name) return true;

      int slash = s_lastFile.lastIndexOf('/');
      if(slash >= 0)
      {
        String base = s_lastFile.substring(slash + 1);
        if(base == name) return true;
      }

      return false;
    }

    static void Check_ClearFilesEditFlag()
    {
      s_fileChanged = false;
      // s_lastFile = String();
    }

    static const String& Get_LastEditedFileName()
    {
      return s_lastFile;
    }
};

#endif