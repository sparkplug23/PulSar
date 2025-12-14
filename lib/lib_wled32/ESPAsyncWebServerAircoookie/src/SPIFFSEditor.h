#ifndef SPIFFSEditor_H_
#define SPIFFSEditor_H_
#include <ESPAsyncWebServer.h>
#ifdef ESP8266
#include <LittleFS.h>
#endif

//this indicates that this implementation will not serve the wsec.json file from FS
#define SPIFFS_EDITOR_AIRCOOOKIE

class SPIFFSEditor: public AsyncWebHandler {
  private:
    fs::FS _fs;
    String _username;
    String _password; 
    bool _authenticated;
    uint32_t _startTime;

    
    // --- global change tracking ---
    static volatile bool s_fileChanged;
    static String        s_lastFile;


  public:
    SPIFFSEditor(const fs::FS& fs, const String& username=String(), const String& password=String());
#ifdef ESP8266
    // Alternate constructor, defaults to LittleFS
    SPIFFSEditor(const String& username=String(), const String& password=String(), const fs::FS& fs=LittleFS);
#endif    

    virtual bool canHandle(AsyncWebServerRequest *request) override final;
    virtual void handleRequest(AsyncWebServerRequest *request) override final;
    virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override final;
    virtual bool isRequestHandlerTrivial() override final {return false;}

    // --- NEW: generic helpers ---

    // Has *any* file been edited since last clear?
    static bool Check_AnyFilesEdited() {
      return s_fileChanged;
    }

    // Does the last edited file match this name?
    // Accepts either full path ("/presets.json") or bare name ("presets.json").
    static bool Check_FileEditedIs(const String &name) {
      if (!s_fileChanged) return false;

      // exact match
      if (s_lastFile == name) return true;

      // allow bare filename: compare against basename of s_lastFile
      int slash = s_lastFile.lastIndexOf('/');
      if (slash >= 0) {
        String base = s_lastFile.substring(slash + 1);
        if (base == name) return true;
      }

      return false;
    }

    // Clear the edited flag once all modules have reacted
    static void Check_ClearFilesEditFlag() {
      s_fileChanged = false;
      // optional: also clear name
      // s_lastFile = String();
    }

    static const String& Get_LastEditedFileName() {
      return s_lastFile;
    }
};

#endif
