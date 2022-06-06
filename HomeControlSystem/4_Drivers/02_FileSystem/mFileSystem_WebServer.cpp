
/*********************************************************************************************\
 * Web support
\*********************************************************************************************/

#ifdef USE_WEBSERVER

const char UFS_WEB_DIR[] PROGMEM =
  "<p><form action='" "ufsd" "' method='get'><button>" "%s" "</button></form></p>";

const char UFS_FORM_FILE_UPLOAD[] PROGMEM =
  "<div id='f1' name='f1' style='display:block;'>"
  "<fieldset><legend><b>&nbsp;" D_MANAGE_FILE_SYSTEM "&nbsp;</b></legend>";
const char UFS_FORM_FILE_UPGc[] PROGMEM =
  "<div style='text-align:left;color:#%06x;'>" D_FS_SIZE " %s MB - " D_FS_FREE " %s MB";

const char UFS_FORM_FILE_UPGc1[] PROGMEM =
    " &nbsp;&nbsp;<a href='http://%_I/ufsd?dir=%d'>%s</a>";

const char UFS_FORM_FILE_UPGc2[] PROGMEM =
  "</div>";

const char UFS_FORM_FILE_UPG[] PROGMEM =
  "<form method='post' action='ufsu' enctype='multipart/form-data'>"
  "<br><input type='file' name='ufsu'><br>"
  "<br><button type='submit' onclick='eb(\"f1\").style.display=\"none\";eb(\"f2\").style.display=\"block\";this.form.submit();'>" D_START " %s</button></form>"
  "<br>";
const char UFS_FORM_SDC_DIRa[] PROGMEM =
  "<div style='text-align:left;overflow:auto;height:250px;'>";
const char UFS_FORM_SDC_DIRc[] PROGMEM =
  "</div>";
const char UFS_FORM_FILE_UPGb[] PROGMEM =
  "</fieldset>"
  "</div>"
  "<div id='f2' name='f2' style='display:none;text-align:center;'><b>" D_UPLOAD_STARTED " ...</b></div>";
const char UFS_FORM_SDC_DIRd[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a></pre>";
const char UFS_FORM_SDC_DIRb[] PROGMEM =
  "<pre><a href='%s' file='%s'>%s</a> %s %8d %s</pre>";
const char UFS_FORM_SDC_HREF[] PROGMEM =
  "http://%_I/ufsd?download=%s/%s";
#ifdef GUI_TRASH_FILE
const char UFS_FORM_SDC_HREFdel[] PROGMEM =
  //"<a href=http://%_I/ufsd?delete=%s/%s>&#128465;</a>";
  "<a href=http://%_I/ufsd?delete=%s/%s>&#128293;</a>"; // 🔥
#endif // GUI_TRASH_FILE

void UfsDirectory(void) {
  if (!HttpCheckPriviledgedAccess()) { return; }

  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HTTP D_MANAGE_FILE_SYSTEM));

  uint8_t depth = 0;

  strcpy(ufs_path, "/");

  if (Webserver->hasArg(F("download"))) {
    String stmp = Webserver->arg(F("download"));
    char *cp = (char*)stmp.c_str();
    if (UfsDownloadFile(cp)) {
      // is directory
      strcpy(ufs_path, cp);
    } else {
      return;
    }
  }

  if (Webserver->hasArg(F("dir"))) {
    String stmp = Webserver->arg(F("dir"));
    ufs_dir = atoi(stmp.c_str());
    if (ufs_dir == 1) {
      dfsp = ufsp;
    } else {
      if (ffsp) {
        dfsp = ffsp;
      }
    }
  }

  if (Webserver->hasArg(F("delete"))) {
    String stmp = Webserver->arg(F("delete"));
    char *cp = (char*)stmp.c_str();
    dfsp->remove(cp);
  }

  WSContentStart_P(PSTR(D_MANAGE_FILE_SYSTEM));
  WSContentSendStyle();
  WSContentSend_P(UFS_FORM_FILE_UPLOAD);

  char ts[FLOATSZ];
  dtostrfd((float)UfsInfo(0, ufs_dir == 2 ? 1:0) / 1000, 3, ts);
  char fs[FLOATSZ];
  dtostrfd((float)UfsInfo(1, ufs_dir == 2 ? 1:0) / 1000, 3, fs);
  WSContentSend_PD(UFS_FORM_FILE_UPGc, WebColor(COL_TEXT), ts, fs);

  if (ufs_dir) {
    WSContentSend_P(UFS_FORM_FILE_UPGc1, (uint32_t)WiFi.localIP(), (ufs_dir == 1)?2:1, (ufs_dir == 1)?PSTR("SDCard"):PSTR("FlashFS"));
  }
  WSContentSend_P(UFS_FORM_FILE_UPGc2);

  WSContentSend_P(UFS_FORM_FILE_UPG, PSTR(D_SCRIPT_UPLOAD));

  WSContentSend_P(UFS_FORM_SDC_DIRa);
  if (ufs_type) {
    UfsListDir(ufs_path, depth);
  }
  WSContentSend_P(UFS_FORM_SDC_DIRc);
  WSContentSend_P(UFS_FORM_FILE_UPGb);
  WSContentSpaceButton(BUTTON_CONFIGURATION);
  WSContentStop();

  Web.upload_file_type = UPL_UFSFILE;
}

void UfsListDir(char *path, uint8_t depth) {
  char name[32];
  char npath[128];
  char format[12];
  sprintf(format, PSTR("%%-%ds"), 24 - depth);

  File dir = dfsp->open(path, UFS_FILE_READ);
  if (dir) {
    dir.rewindDirectory();
    if (strlen(path)>1) {
      ext_snprintf_P(npath, sizeof(npath), PSTR("http://%_I/ufsd?download=%s"), (uint32_t)WiFi.localIP(), path);
      for (uint32_t cnt = strlen(npath) - 1; cnt > 0; cnt--) {
        if (npath[cnt] == '/') {
          if (npath[cnt - 1] == '=') {
            npath[cnt + 1] = 0;
          } else {
            npath[cnt] = 0;
          }
          break;
        }
      }
      WSContentSend_P(UFS_FORM_SDC_DIRd, npath, path, PSTR(".."));
    }
    char *ep;
    while (true) {
      File entry = dir.openNextFile();
      if (!entry) {
        break;
      }
      // esp32 returns path here, shorten to filename
      ep = (char*)entry.name();
      if (*ep == '/') { ep++; }
      char *lcp = strrchr(ep,'/');
      if (lcp) {
        ep = lcp + 1;
      }

      uint32_t tm = entry.getLastWrite();
      String tstr = GetDT(tm);

      char *pp = path;
      if (!*(pp + 1)) { pp++; }
      char *cp = name;
      // osx formatted disks contain a lot of stuff we dont want
      if (!UfsReject((char*)ep)) {

        for (uint8_t cnt = 0; cnt<depth; cnt++) {
          *cp++ = '-';
        }

        sprintf(cp, format, ep);
        if (entry.isDirectory()) {
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRd, npath, ep, name);
          uint8_t plen = strlen(path);
          if (plen > 1) {
            strcat(path, "/");
          }
          strcat(path, ep);
          UfsListDir(path, depth + 4);
          path[plen] = 0;
        } else {
#ifdef GUI_TRASH_FILE
          char delpath[128];
          ext_snprintf_P(delpath, sizeof(delpath), UFS_FORM_SDC_HREFdel, (uint32_t)WiFi.localIP(), pp, ep);
#else
          char delpath[2];
          delpath[0]=0;
#endif // GUI_TRASH_FILE
          ext_snprintf_P(npath, sizeof(npath), UFS_FORM_SDC_HREF, (uint32_t)WiFi.localIP(), pp, ep);
          WSContentSend_P(UFS_FORM_SDC_DIRb, npath, ep, name, tstr.c_str(), entry.size(), delpath);
        }
      }
      entry.close();
    }
    dir.close();
  }
}

#ifdef ESP32
#define ESP32_DOWNLOAD_TASK
#endif // ESP32

uint8_t UfsDownloadFile(char *file) {
  File download_file;

  if (!dfsp->exists(file)) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: File not found"));
    return 0;
  }

  download_file = dfsp->open(file, UFS_FILE_READ);
  if (!download_file) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Could not open file"));
    return 0;
  }

  if (download_file.isDirectory()) {
    download_file.close();
    return 1;
  }

#ifndef ESP32_DOWNLOAD_TASK
  WiFiClient download_Client;
  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp;
  for (uint32_t cnt = strlen(file); cnt >= 0; cnt--) {
    if (file[cnt] == '/') {
      cp = &file[cnt + 1];
      break;
    }
  }
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
  Webserver->sendHeader(F("Content-Disposition"), attachment);
  WSSend(200, CT_APP_STREAM, "");

  uint8_t buff[512];
  uint32_t bread;
  // transfer is about 150kb/s
  uint32_t cnt = 0;
  while (download_file.available()) {
    bread = download_file.read(buff, sizeof(buff));
    uint32_t bw = download_Client.write((const char*)buff, bread);
    if (!bw) { break; }
    cnt++;
    if (cnt > 7) {
      cnt = 0;
      //if (glob_script_mem.script_loglevel & 0x80) {
        // this indeed multitasks, but is slower 50 kB/s
      //  loop();
      //}
    }
    delay(0);
    OsWatchLoop();
  }
  download_file.close();
  download_Client.stop();
#endif // ESP32_DOWNLOAD_TASK


#ifdef ESP32_DOWNLOAD_TASK
  download_file.close();

  if (UfsData.download_busy == true) {
    AddLog(LOG_LEVEL_INFO, PSTR("UFS: Download is busy"));
    return 0;
  }

  UfsData.download_busy = true;
  char *path = (char*)malloc(128);
  strcpy(path,file);
  xTaskCreatePinnedToCore(donload_task, "DT", 6000, (void*)path, 3, NULL, 1);
#endif // ESP32_DOWNLOAD_TASK

  return 0;
}


#ifdef ESP32_DOWNLOAD_TASK
#ifndef DOWNLOAD_SIZE
#define DOWNLOAD_SIZE 4096
#endif // DOWNLOAD_SIZE
void donload_task(void *path) {
  File download_file;
  WiFiClient download_Client;
  char *file = (char*) path;

  download_file = dfsp->open(file, UFS_FILE_READ);
  free(file);

  uint32_t flen = download_file.size();

  download_Client = Webserver->client();
  Webserver->setContentLength(flen);

  char attachment[100];
  char *cp;
  for (uint32_t cnt = strlen(file); cnt >= 0; cnt--) {
    if (file[cnt] == '/') {
      cp = &file[cnt + 1];
      break;
    }
  }
  snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=%s"), cp);
  Webserver->sendHeader(F("Content-Disposition"), attachment);
  WSSend(200, CT_APP_STREAM, "");

  uint8_t *buff = (uint8_t*)malloc(DOWNLOAD_SIZE);
  if (buff) {
    uint32_t bread;
    while (download_file.available()) {
      bread = download_file.read(buff, DOWNLOAD_SIZE);
      uint32_t bw = download_Client.write((const char*)buff, bread);
      if (!bw) { break; }
    }
    free(buff);
  }
  download_file.close();
  download_Client.stop();
  UfsData.download_busy = false;
  vTaskDelete( NULL );
}
#endif //  ESP32_DOWNLOAD_TASK


bool UfsUploadFileOpen(const char* upload_filename) {
  char npath[48];
  snprintf_P(npath, sizeof(npath), PSTR("%s/%s"), ufs_path, upload_filename);
  dfsp->remove(npath);
  ufs_upload_file = dfsp->open(npath, UFS_FILE_WRITE);
  return (ufs_upload_file);
}

bool UfsUploadFileWrite(uint8_t *upload_buf, size_t current_size) {
  if (ufs_upload_file) {
    ufs_upload_file.write(upload_buf, current_size);
  } else {
    return false;
  }
  return true;
}

void UfsUploadFileClose(void) {
  ufs_upload_file.close();
}

#endif  // USE_WEBSERVER
