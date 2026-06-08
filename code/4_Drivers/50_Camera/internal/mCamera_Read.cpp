#include "../mCamera.h"

#ifdef USE_MODULE_DRIVERS__CAMERA

/************************************************************************************************
 * SECTION: Camera Read / Frame Capture / Runtime Loop
 ************************************************************************************************/

bool mCamera::Picture_Alloc(struct mCamera::PICSTORE *ps, int width, int height, int jpegsize, int format, int allocstyle){
  int len = 0;
  bool res = false;
  uint8_t* orgbuf = nullptr;
  int orglen = 0;

  if (!ps)
  {
    ALOG_ERR(PSTR(D_LOG_CAMERA "Picture_Alloc ps null"));
    return false;
  }

  switch(format){
    default:                  len = 0;                  break;
    case PIXFORMAT_JPEG:      len = jpegsize;           break;
    case PIXFORMAT_GRAYSCALE: len = width*height + 4;   break;
    case PIXFORMAT_RGB888:    len = width*height*3 + 4; break;
    case PIXFORMAT_RGB565:    len = width*height*2 + 4; break;
  }

  if (!len)
  {
    ALOG_ERR(PSTR(D_LOG_CAMERA "Picture_Alloc invalid format or len 0"));
    return false;
  }

  switch(allocstyle & 0xf){
    case WC_ALLOC_ALWAYS: // just free & allocate, unless no change
      if (len != ps->allocatedLen){
        ps->allocatedLen = -1; // force realloc
      }
      break;
    case WC_REALLOC_IF_SMALLER: // allocate if buf not big enough
      // will use ps->allocatedLen to check below
      break;
    case WC_REALLOC_WITH_COPY: // allocate and copy data if buf not big enough
      orgbuf = ps->buff;
      orglen = ps->allocatedLen;
      break;
  }

  if (ps->allocatedLen < len){
    free(ps->buff);
    memset(ps, 0, sizeof(*ps));
  }
  if (!ps->buff){
    if (len){
      if ((allocstyle & WC_USE_FAST_MEM) || !rt.psram){
        ps->buff = (uint8_t *)malloc(len);
      } else {
        ps->buff = (uint8_t *)heap_caps_aligned_alloc(4, len, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
      }
      if (ps->buff) ps->allocatedLen = len;
      if (orgbuf && orglen){
        memcpy(ps->buff, orgbuf, orglen);
      }
    }
  }
  if (ps->buff){
    ps->len = len;
    // don't change  if 0 in, as jpeg decode sets at start only
    if (width) ps->width = width;
    if (height) ps->height = height;
    ps->format = format;
  }

  if (ps->buff) res = true;
  return res;
}

bool mCamera::Picture_Free(struct mCamera::PICSTORE *ps){
  bool res = false;
  if (ps){
    if (ps->buff){
      free(ps->buff);
      res = true;
    }
    memset(ps, 0, sizeof(*ps));
  }
  return res;
}

bool mCamera::Picture_AllocPtr(struct mCamera::PICSTORE **pps, int width, int height, int jpegsize, int format, int allocstyle){
  if (!*pps){
    *pps = (struct mCamera::PICSTORE *)malloc(sizeof(struct mCamera::PICSTORE));
    memset(*pps, 0, sizeof(struct mCamera::PICSTORE));
  }
  if (*pps){
    return Picture_Alloc(*pps, width, height, jpegsize, format, allocstyle);
  }
  return false;
}

bool mCamera::Picture_FreePtr(struct mCamera::PICSTORE **pps){
  bool res = false;
  if (*pps){
    res = Picture_Free(*pps);
    free(*pps);
    *pps = nullptr;
  }
  return res;
}

bool mCamera::Picture_IsPixelFormatSupported(int format){
  switch(format){
    case PIXFORMAT_JPEG:      return true;
    case PIXFORMAT_GRAYSCALE: return true;
#ifdef WC_USE_RGB_DECODE    
    case PIXFORMAT_RGB888:    return true;
    case PIXFORMAT_RGB565:    return true;
#endif
  }
  return false;
}

void mCamera::Stream_RemoveDeadClients()
{
  // iterate over clients removing dead ones
  mCamera::wc_client *client = rt.client_p;
  mCamera::wc_client **prev = &rt.client_p;
  while(client)
  {
    if (!client->active)
    {
      // just in case...
      mSupport::AutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
      *prev = client->p_next;
      client->client.stop();
      mCamera::wc_client *next = client->p_next;
      delete client;
      client = next;
    } else {
      prev = &client->p_next;
      client = client->p_next;
    }
  }
}

void mCamera::Frame_WaitForNext(int maxtime_ms) {
  if (!rt.taskRunning) return;
  // force a wait for a read
  Access_WaitUntilZero(&rt.taskTakePic, -1, maxtime_ms);
}

uint32_t mCamera::Frame_GetWidth(void) {
  if (rt.taskRunning){
    if (!rt.width){
      Frame_WaitForNext(1000);
    }
  }
  return rt.width;
}

uint32_t mCamera::Frame_GetHeight(void) {
  if (rt.taskRunning){
    if (!rt.width){
      Frame_WaitForNext(1000);
    }
  }
  return rt.width;
}

uint32_t mCamera::PictureStore_GetBuffer(int32_t num, uint8_t **buff) {
  if (num<0) { return MAX_PICSTORE; }
  *buff = rt.picstore[num].buff;
  return rt.picstore[num].len;
}

uint32_t mCamera::PictureStore_Get(int32_t num, struct mCamera::PICSTORE **p) {
  if (num < 0) { 
    *p = nullptr;
    return MAX_PICSTORE; 
  }
  if (num >= MAX_PICSTORE) {
    *p = nullptr;
    return MAX_PICSTORE; 
  } 
  *p = &rt.picstore[num];
  return rt.picstore[num].len;
}

uint32_t mCamera::Frame_RequestCaptureToStore(int32_t bnum) {
  if (bnum < 0) {
    if (bnum < -MAX_PICSTORE) { bnum=-1; }
    bnum = -bnum;
    // check before decrement
    if (bnum == rt.lastBnum){
      rt.lastBnum = 0;
    }
    bnum--;
    mSupport::AutoMutex localmutex(&WebcamMutex, "WcGetFrame", 200);
    if (rt.picstore[bnum].buff) { free(rt.picstore[bnum].buff); }
    memset(&rt.picstore[bnum], 0, sizeof(rt.picstore[bnum]));
    rt.picstore[bnum].len = 0;
    return 0;
  }

  if (!bnum) {    
    // used to be used to trigger read a frame to get width/height
    // can still get here via scripts
    return 0;
  }

  if (rt.taskRunning){
    Access_WaitUntilZero(&rt.taskGetFrame, bnum, 1000);
    return rt.picstore[bnum - 1].len;
  }
  return 0;
}

void mCamera::FrameTask_Start()
{

  if (rt.taskRunning == 0)
  {

    ALOG_INF(PSTR(D_LOG_CAMERA "Start operations"));

    xTaskCreatePinnedToCore(
      FrameTask_StaticLoop,    /* Function to implement the task */
      "WCOperationTask",  /* Name of the task */

      /**************
      This stack is exclusive to the task — it's used for:
        Local variables
        Function calls
        ISR contexts
        Deep recursion (bad idea in FreeRTOS)
        Library calls (like printf, String, JSON parsing, etc.)
      Stack Size (Words)	Bytes	When to Use
        2048	8 KB	Very lightweight (GPIO, polling)
        4096	16 KB	Basic parsing, logging
        8192	32 KB	Camera, networking, JSON, RTSP
        16384	64 KB	Heavy memory usage, image processing
      **************/
      8192,               // Stack size in number of words (not bytes) allocated to the task's stack memory. On ESP32: Each "word" is 4 bytes (32 bits).


      this,               // <--- Pass the instance
      0,                  /* Priority of the task */
      &rt.taskHandle,     /* Task handle. */
      #ifdef CONFIG_FREERTOS_UNICORE
        0                 /* Core where the task should run */
      #else
        WEBCAM_CORE       /* Core where the task should run */
      #endif
    );

    // wait for task to start
    int loops = 10;
    while(!rt.taskRunning && loops--){
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
  }

}

void mCamera::FrameTask_StaticLoop(void* pvParameters) {
  // Avoids relying on tkr_camera and is more reusable if there are multiple camera instances.
  static_cast<mCamera*>(pvParameters)->FrameTask_Loop();  // Call actual task logic
}

void mCamera::FrameTask_Loop(){
  unsigned long loopcount = 0;

  ALOG_INF(PSTR(D_LOG_CAMERA "WCOperationTask: Start task"));
  
  int framecount = 0;
  unsigned long laststatmillis = millis();
  bool jpeg_converted = false;
  uint32_t processing_total = 0;
  uint64_t last_camtime = 0;
  rt.taskRunning = 1;
  stats.framesTotal = 0;
  stats.framesLost = 0;

  int32_t skipsWanted = rt.skipFrames;


  // we set to rt.taskRunning 2 to stop the task
  while (rt.taskRunning == 1){
    loopcount++;
    uint32_t thismillis = millis();
    uint32_t statdur = thismillis - laststatmillis;

    // storage and settings disable cam.
    // we must stall until re-enabled
    // it asserts the mutex whilst stopping the cam, so it can't stop it mid-processing
    Access_WaitUntilEnabled();
    // if camera is configured and working
    { // closure for auto mutex
      // note that this mutex can block the loop for a long time - 
      // e.g. if motion detect on a full big frame, up to a second
      mSupport::AutoMutex localmutex(&WebcamMutex, "WebcamMjpeg", 30000);

      // only do anything if cam us up, and cam has not remained stopped
      if (rt.up && !rt.disable_cam) {
        size_t _jpg_buf_len = 0;
        uint8_t * _jpg_buf = NULL;

        // read a frame buffer pointer.  this will block until a frame is available
        camera_fb_t *wc_fb = esp_camera_fb_get();

        if (!wc_fb) {
          // add framecount so we show this right away if we were showing frames.
          if (!(loopcount % 100) || (statdur > 5000) || framecount){
            ALOG_INF(PSTR(D_LOG_CAMERA "Frame fail")); 
            laststatmillis = thismillis;
            framecount = 0;
          }
          stats.camfail++;
          // if camera up, but just can't get a frame right now
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          framecount++;
          uint32_t now = millis();
          uint64_t camtime = wc_fb->timestamp.tv_sec;
          camtime = camtime * 1000000;
          camtime += wc_fb->timestamp.tv_usec;

          if (!skipsWanted) skipsWanted = rt.skipFrames;

          if (last_camtime){
            uint32_t camdiff = camtime - last_camtime;
            // this is an accurate indication of framerate, but also frame skip
            rt.camtimediff = camdiff;
            int framesPassed = round(((float)camdiff)/(float)(rt.frameIntervalsus));
            if (framesPassed){
              stats.framesTotal += framesPassed;
              stats.framesLost += (framesPassed - 1);
              if (skipsWanted > 0){
                skipsWanted -= (framesPassed - 1);
              }
              if (skipsWanted > 0) skipsWanted --;
            } else {
              ALOG_INF(PSTR(D_LOG_CAMERA "Duplicate time in frame? diff %d intv %d"), camdiff, rt.frameIntervalsus);
            }
          }
          last_camtime = camtime;

          // every 100 frames or 5s
          if (!(loopcount % 100) || (statdur > 5000)){
            float framespersec = ((float)framecount)/(((float)(thismillis - laststatmillis))/1000.0);
            ALOG_DBG(PSTR(D_LOG_CAMERA "avFPS %f %s FS:%d(%d) f:%u s:%u"), 
              framespersec, 
              jpeg_converted?"raw":"jpg", 
              wc_fb->len, 
              rt.lenDiff,
              stats.framesTotal,
              stats.framesLost
              );
            stats.avgProcessingPerFrameMS = processing_total/framecount;
            stats.avgFPS = framespersec;
            stats.avgFrameMS = (uint8_t) (1000/framespersec);
            processing_total = 0;
            laststatmillis = thismillis;
            framecount = 0;
          }

          // we got a frame
          rt.width = wc_fb->width;
          rt.height = wc_fb->height;
          stats.camcnt++;

          // if we want to process this frame.
          // skipsWanted is the counter used for rt.skipFrames use
          if (skipsWanted <= 0) skipsWanted = 0;
          if (!skipsWanted){
#ifdef USE_WEBCAM_MOTION
            int detectMotion = 0;
            // if we want simple mootion detect,
            if (wc_motion.motion_detect){
              if ((now - wc_motion.motion_ltime) > wc_motion.motion_detect) {
                wc_motion.motion_ltime = now;
                detectMotion = 1;
              }
            }
#endif
            // if we need a frame for web stream or rtsp
            if(
              rt.client_p // pointer to first 
              #ifdef ENABLE_RTSPSERVER
              || rt.rtsp_client // pointer to first
              #endif
              || rt.taskGetFrame // get one frame - from scripts
              || rt.taskTakePic // get one frame - from scripts
              #ifdef USE_WEBCAM_MOTION
              || detectMotion 
              #endif                
            ){

              // most cameras will supply as jpeg? it's what we ask for...
              jpeg_converted = false;
              bool free_jpeg = false;
              /* 2023-05-28 - AiThinker type cam module marked DM.
                Note: I tried everything to get pixels ina different form, but faiiled.
                So this code has NEVER run for me in anger directly off cam data.
              */

              if (wc_fb->format != PIXFORMAT_JPEG) 
              {
                #ifdef USE_WEBCAM_MOTION
                // note - don't free the jpeg, we re-use it.
                jpeg_converted = WcencodeToJpeg(wc_fb->buf, wc_fb->len, wc_fb->width, wc_fb->height, (int)wc_fb->format, 80, &VideoJpeg);
                _jpg_buf_len = VideoJpeg.len;
                _jpg_buf = VideoJpeg.buff;

                // this function is incredibly expensive - always allocates 128kbytes
                //jpeg_converted = frame2jpg(wc_fb, 80, &_jpg_buf, &_jpg_buf_len);
                // free_jpeg = true; // if using frame2jpg, we must free
                #endif                
                if (!jpeg_converted)
                {
                  ALOG_DBG(PSTR(D_LOG_CAMERA "JPEG compression failed"));
                  stats.jpegfail++;
                }
              } else {
                _jpg_buf_len = wc_fb->len;
                _jpg_buf = wc_fb->buf;
              }

              if (_jpg_buf)
              {
                int jpeg_len_diff = abs((int)rt.last_frame_len - (int)_jpg_buf_len);
                int jpeg_len_diff_percent = 100*jpeg_len_diff/_jpg_buf_len;
                rt.lenDiff = jpeg_len_diff_percent;
                if (rt.lenDiffLimit && (rt.lenDiffLimit < jpeg_len_diff_percent))
                {
                  rt.lenDiffTrigger = 1;
                  rt.lenDiffTriggered = jpeg_len_diff_percent;
                }

                rt.last_frame_len = _jpg_buf_len;
                // we use -1 to just detect that rt.width/height should have been set
                if (rt.taskTakePic)
                {
                  if (rt.taskTakePic > 0)
                  {
                    if (Picture_Alloc(&rt.snapshotStore, 0, 0, _jpg_buf_len, PIXFORMAT_JPEG, 1))
                    {
                      memcpy(rt.snapshotStore.buff, _jpg_buf, _jpg_buf_len);
                    }
                  }
                  rt.taskTakePic = 0;
                }

                // request to store the frame into picstore
                if (rt.taskGetFrame)
                {
                  int bnum = rt.taskGetFrame;
                  if ((bnum < 1) || (bnum > MAX_PICSTORE)) { bnum = 1; }
                  bnum--;
                  if (rt.picstore[bnum].buff) { free(rt.picstore[bnum].buff); }
                  memset(&rt.picstore[bnum], 0, sizeof(rt.picstore[bnum]));
                  rt.picstore[bnum].buff = (uint8_t *)heap_caps_aligned_alloc(4, _jpg_buf_len+4, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
                  if (rt.picstore[bnum].buff) {
                    memcpy(rt.picstore[bnum].buff, _jpg_buf, _jpg_buf_len);
                    rt.picstore[bnum].len = _jpg_buf_len;
                    rt.lastBnum = bnum + 1;
                    rt.picstore[bnum].format = (int8_t)PIXFORMAT_JPEG;
                    rt.picstore[bnum].width = rt.width;
                    rt.picstore[bnum].height = rt.height;

                    ALOG_INF(PSTR(D_LOG_CAMERA "Got frame %d"), rt.lastBnum);
                  } 
                  else 
                  {
                    ALOG_INF(PSTR(D_LOG_CAMERA "Can't allocate picstore"));
                  }
                  rt.taskGetFrame = 0;
                }

                #ifdef USE_WEBCAM_MOTION
                // if motion detect triggered by timer
                if (detectMotion){
                  WcDetectMotionFn(_jpg_buf, _jpg_buf_len);
                }
                #endif

                // if http streaming is active, we will have one or more clients
                mCamera::wc_client *client = rt.client_p;
                // iterate over clients
                uint8_t webclientcount = 0;

                #ifdef USE_WEBCAM_MOTION
                size_t diff_jpg_buf_len = 0;
                uint8_t *diff_jpg_buf = NULL;
                #endif

                while(client)
                {
                  if (client->active)
                  {
                    uint32_t client_start = millis();

                    if (!client->client.connected())
                    {
                      ALOG_INF(PSTR(D_LOG_CAMERA "Client fail"));
                      client->active = 0;
                      stats.clientfail++;
                    }
                    if (1 == client->active) {
                      client->client.flush();
                      client->client.setTimeout(3);

                      ALOG_INF(PSTR(D_LOG_CAMERA "Start stream"));

                      client->client.print("HTTP/1.1 200 OK\r\n"
                        "Content-Type: multipart/x-mixed-replace;boundary=" BOUNDARY "\r\n"
                        "\r\n");
                      client->active = 2;
                    }

                    if (2 == client->active) 
                    {
                      int type = client->type;
                      uint8_t *src = _jpg_buf;
                      int len = _jpg_buf_len;

                      #ifdef USE_WEBCAM_MOTION
                      // if this client wants motion images
                      if (type == 1){
                        // if we already coded to jpeg for another client
                        if (diff_jpg_buf){
                          src = diff_jpg_buf;
                          len = diff_jpg_buf_len;
                        } else {
                          src = nullptr;
                          // only for frames where motion is being detected,
                          // e.g. once per sec
                          if (detectMotion){
                            struct PICSTORE *ps = nullptr;
                            // by default, we send the diff buffer if present
                            if (wc_motion.diff && wc_motion.diff->buff){
                              ps = wc_motion.diff;
                            }
                            if (!ps){
                              // else we send the motion buffer (mono of video, but slower)
                              if (wc_motion.last_motion && wc_motion.last_motion->buff){
                                ps = wc_motion.last_motion;
                              }
                            }
                            if (ps){
                              // use a malloc that we don't free to save memory creep
                              // it is re-mallcoed if the frame does not fit.
                              bool conv = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &OurOneJpeg);

                              diff_jpg_buf_len = OurOneJpeg.len;
                              diff_jpg_buf = OurOneJpeg.buff;

                              if (conv && diff_jpg_buf_len){
                                src = diff_jpg_buf;
                                len = diff_jpg_buf_len;
                              }
                            }
                          }
                        }
                      }
                      #endif                      

                      if (src){
                        client->client.printf(
                          "--" BOUNDARY "\r\n"
                          "Content-Type: image/jpeg\r\n"
                          "Content-Length: %d\r\n"
                          "\r\n", static_cast<int>(len));
                        uint32_t tlen = client->client.write(src, len);
                        client->client.print("\r\n");
                      }
                    }
                    uint32_t client_end = millis();
                    // if it took more than 20s to send to the client, then kill it.
                    // this was observed on wifi rescan
                    if (client_end - client_start > 20000){
                      ALOG_INF(PSTR(D_LOG_CAMERA "Cl timeout on send"));
                      stats.clientfail++;
                      client->client.stop();
                      client->active = 0;
                    }
                  }
                  client = client->p_next;
                  webclientcount++;
                }
                // if we allocates a jppeg image for motion, free now
                // NO!!! This is now a long-lived buffer!!!
                //if (diff_jpg_buf) free(diff_jpg_buf);
                stats.activeClients.web = webclientcount;

                #ifdef ENABLE_RTSPSERVER
                // if rtsp is active, we will have one or more clients
                volatile wc_rtspclient *rtsp_client = rt.rtsp_client;
                uint8_t rtspclientcount = 0;
                while (rtsp_client) {
                  if (rtsp_client->camStreamer && rtsp_client->rtsp_session){
                    rtsp_client->camStreamer->setframe(_jpg_buf, _jpg_buf_len);
                    rtsp_client->rtsp_session->broadcastCurrentFrame(now);
                    rtsp_client->camStreamer->clearframe();
                  }
                  rtsp_client = rtsp_client->p_next;
                  rtspclientcount++;
                }
                stats.activeClients.rtsp = rtspclientcount;
                #endif

                // free jpeg frame if we created it
                if (free_jpeg) { free(_jpg_buf); }

                // could trigger info to berry every fraem, if configured
                rt.frame_processed = 1;

                uint32_t processingend = millis();
                uint32_t processingdur = processingend - now;
                processing_total += processingdur;
              } else {
                rt.last_frame_len = 0;            
              }
            }
          }

          // free resources and return frame buffer for re-use AFTER we have used the data
          esp_camera_fb_return(wc_fb);
        }
      } else {
        if (rt.up){
          // if camera up, but just can't get a frame right now because
          // else we may conflict with files or settings store, delay just a shorter time
          vTaskDelay(10/ portTICK_PERIOD_MS);
        } else {
          // be sure to clear this if camera is not running, else we could wait forever
          // when asking for a frame
          rt.taskGetFrame = 0;
          // come around every 1/10s if not serving pictures
          vTaskDelay(100/ portTICK_PERIOD_MS);
        }
      }
    }
  }

  // this log sometimes causes guru mediation error. Maybe because 
  // temp storage is removed before it is serviced?
  ALOG_INF(PSTR(D_LOG_CAMERA "Left task"));
  rt.taskRunning = 0;

  // wait 1/2 second for log to be done?
  vTaskDelay(500/ portTICK_PERIOD_MS);

  vTaskDelete( NULL );
}

void mCamera::EveryLoop(void) 
{
  rt.loopcounter++;
  { // closure for automutex
    // we don't need one here
    //AutoMutex localmutex(&WebcamMutex, "WcLoop", 200);

#ifdef USE_WEBCAM_MOTION
    // if wc_motion.motion_trigger > wc_motion.motion_triggerlimit
    // set wc_motion.motion_triggerlimit low if you want every time it's processed
    // NOTE: there is no 'retrigger hold off' time.
    if (wc_motion.motion_triggered){
#ifdef USE_BERRY
      char t[80];
      snprintf(t, (size_t)39, "{\"val\":%d,\"bri\":%d,\"pix\":%d}", wc_motion.motion_trigger, wc_motion.motion_brightness, wc_motion.changedPixelPertenthousand);
      callBerryEventDispatcher("webcam", "motion", 0, t, strlen(t));
#endif
      ALOG_INF(PSTR(D_LOG_CAMERA "Motion Triggered"));
      WcMotionLog();
      wc_motion.motion_triggered = 0;
    }
#endif    
    if (rt.lenDiffTrigger){
#ifdef USE_BERRY
      char t[40];
      snprintf(t, (size_t)39, "{\"diff\":%d}", rt.lenDiffTriggered);
      callBerryEventDispatcher("webcam", "framesizechange", 0, t, strlen(t));
#endif
      ALOG_INF(PSTR(D_LOG_CAMERA "Framesize Change > %d = %d"), rt.lenDiffLimit, rt.lenDiffTriggered);
      rt.lenDiffTrigger = 0;
#ifdef USE_WEBCAM_MOTION
      WcMotionLog();
#endif      
    }

    if (rt.frame_processed){
#ifdef USE_BERRY
      if (rt.berryFrames){
        char t[80];
        snprintf(t, (size_t)39, "{\"len\":%d}", rt.last_frame_len);
        callBerryEventDispatcher("webcam", "frame", 0, t, strlen(t));
      }
#endif
      rt.frame_processed = 0;
    }
  } // end closure for automutex

  if (rt.CamServer) {
    // pretty sure this would be safe
    //AutoMutex localmutex(&WebcamMutex, "WcLoop", 200);
    rt.CamServer->handleClient();
    Stream_RemoveDeadClients();
  }

#ifdef ENABLE_RTSPSERVER
// rtsp://192.168.2.39:8554/mjpeg/1
  if (tkr_iDrivers->webcam_config.rtsp){
    if (!tkr_set->runtime.global_state.wifi_down) {
      // pretty sure we don;t need the mutex here
      mSupport::AutoMutex localmutex(&WebcamMutex, "WcLoop2", 30000);
      if (!rt.rtspp) {
        rt.rtspp = new WiFiServer(8554);
        rt.rtspp->begin();
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP init"));
      }

      wc_rtspclient * volatile rtsp_client = rt.rtsp_client;
      wc_rtspclient * volatile *  prev = &rt.rtsp_client;
      uint8_t clients = 0;
      while (rtsp_client) {
        bool removed = false;
        if (rtsp_client->rtsp_session){
          rtsp_client->rtsp_session->handleRequests(0);
          // if a client has stopped, remove it.
          if (rtsp_client->rtsp_session->m_stopped){
            delete rtsp_client->rtsp_session;
            rtsp_client->rtsp_session = nullptr;
            delete rtsp_client->camStreamer;
            rtsp_client->camStreamer = nullptr;
            //rtsp_client->rtsp_client.stop();
            *prev = rtsp_client->p_next;
            wc_rtspclient *next = rtsp_client->p_next;
            delete rtsp_client;
            rtsp_client = next;
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP stop"));
            removed = true;
          }
        }
        if (!removed){
          clients++;
          prev = &rtsp_client->p_next;
          rtsp_client = rtsp_client->p_next;
        }
      }
      stats.activeClients.rtsp = clients;

      // accept new rtsp clients
      WiFiClient rtsp_client2 = rt.rtspp->accept();
      if (rtsp_client2) {
        wc_rtspclient *client = new wc_rtspclient;
        client->p_next = rt.rtsp_client;
        client->rtsp_client = rtsp_client2;
        client->camStreamer = new localOV2640Streamer(&client->rtsp_client, rt.width, rt.height);
        client->rtsp_session = new CRtspSession(&client->rtsp_client, client->camStreamer); // our threads RTSP session and state
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP str"));
        rt.rtsp_client = client;
        stats.activeClients.rtsp++;
      }
    } else {
      // rtsp not enabled
      if (rt.rtspp){
        // if running, stop cleanly.
        // stop all clients
        EndRTSP();
        delete rt.rtspp;
        rt.rtspp = nullptr;
      }
    }
  }
#endif // ENABLE_RTSPSERVER
}

void mCamera::Stats_Update(void) {
  stats.camfps = stats.camcnt;
  stats.camcnt = 0;
  rt.loopspersec = rt.loopcounter;
  rt.loopcounter = 0;
}


/************************************************************************************************
 * SECTION: Pending / Legacy Picture Save Notes
 ************************************************************************************************/


// int WebcamSavePic(int append) {
//   // returns size
//   // use a dummy for buffer ptr
//   uint8_t *buf = nullptr;
//   int len = 0;
//   int bnum = tkr_events->XdrvMailbox.index;
//   if (bnum < 0){
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic bnum %d"), bnum);
//     return 0;
//   }

//   if (0 == tkr_events->XdrvMailbox.data_len){
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Save Pic no fname"));
//     return 0;
//   }

//   if (bnum == 0){
//     // if bnum 0, get a picture now....
//     // use the snapshot frame buffer
//     rt.taskTakePic = 1;
//     int loops = 100;
//     while (rt.taskTakePic && (loops--)){
//       vTaskDelay(10/ portTICK_PERIOD_MS);
//     }
//   }
//   mSupport::AutoMutex localmutex(&WebcamMutex, "SavePic", 2000);
//   if (bnum == 0){
//     buf = rt.snapshotStore.buff;
//     len = rt.snapshotStore.len;
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic snapshotstore %d"), len);
//   } else {
//     len = WcGetPicstore(bnum - 1, &buf);
//     ALOG_DBG(PSTR(D_LOG_CAMERA "savePic PicStore %d -> %d"), bnum, len);
//   }
//   if (len){
// #ifdef USE_UFILESYS
//     // ffsp should recognise /sd/ if sd card present
//     if (ffsp){
//       File f = ffsp->open(tkr_events->XdrvMailbox.data, append?FS_FILE_APPEND:FS_FILE_WRITE); //
//       if (f){
//         f.write(buf, len);
//         f.close();
//         ALOG_DBG(PSTR(D_LOG_CAMERA "Save Pic %s"), tkr_events->XdrvMailbox.data);
//         if (bnum == 0){
//           Picture_Free(&rt.snapshotStore);
//         }
//         return 1;
//       }
//     }
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic %s"), tkr_events->XdrvMailbox.data);
// #else
//     ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic %s - no USE_UFILESYS"), tkr_events->XdrvMailbox.data);
// #endif
//     if (bnum == 0){
//       Picture_Free(&rt.snapshotStore);
//     }
//     return 0;
//   } 
//   ALOG_DBG(PSTR(D_LOG_CAMERA "Failed Save Pic inv index %d"), tkr_events->XdrvMailbox.payload);
//   return 0;
// }
// // "WCSAVEPIC1 /temp.jpg" "WCSAVEPIC2 /temp.jpg"
// void CmdWebcamSavePic(){
//   if (WebcamSavePic(0)) {
//     ResponseCmndDone();
//   }    
// }
// // "WCAPPENDPIC1 /temp.jpg" "WCAPPENDPIC2 /temp.jpg"
// void CmdWebcamAppendPic(){
//   if (WebcamSavePic(1)) {
//     ResponseCmndDone();
//   }
// }

// void CmndWebcamMenuVideoDisable(void) {
//   if ((tkr_events->XdrvMailbox.payload >= 0) && (tkr_events->XdrvMailbox.payload <= 1)) {
//     tkr_iDrivers->webcam_config.spare15 = tkr_events->XdrvMailbox.payload;
//   }
//   ResponseCmndStateText(tkr_iDrivers->webcam_config.spare15);
// }

// void CmndWebcamStream(void) {
//   if ((tkr_events->XdrvMailbox.payload >= 0) && (tkr_events->XdrvMailbox.payload <= 1)) {
//     tkr_iDrivers->webcam_config.stream = tkr_events->XdrvMailbox.payload;
//     WcSetStreamserver(tkr_iDrivers->webcam_config.stream);
//   } else {
//     // we use this from a menu
//     if (tkr_events->XdrvMailbox.index == 2){
//       tkr_iDrivers->webcam_config.stream = 1;
//       WcSetStreamserver(tkr_iDrivers->webcam_config.stream);
//     }
//   }
//   ResponseCmndStateText(tkr_iDrivers->webcam_config.stream);
// }




/************************************************************************************************
 * SECTION: Read-Related Web / Stream Handlers
 ************************************************************************************************/

bool mCamera::Web_Authenticate(void)
{
  // if (strlen(SettingsText(SET_WEBPWD)) && (HTTP_MANAGER_RESET_ONLY != Web.state)) {
  //   return rt.CamServer->authenticate(WEB_USERNAME, SettingsText(SET_WEBPWD));
  // } else {
    return true;
  // }
}

bool mCamera::Web_CheckPrivilegedAccess(bool autorequestauth)
{
  // if(tkr_iDrivers->webcam_config2.auth == 0){
  //   return true;
  // }
  // if (autorequestauth && !WebcamAuthenticate()) {
  //   rt.CamServer->requestAuthentication();
  //   return false;
  // }
  return true;
}

void mCamera::Web_HandleImage(void) 
{

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "HandleImage"));


  uint32_t bnum = rt.CamServer->arg(F("p")).toInt();
  if ((bnum < 0) || (bnum > MAX_PICSTORE)) { bnum= 1; }
  int format = (int)PIXFORMAT_JPEG;

  if (bnum){
    // no picture present at this index
    if (!rt.picstore[bnum-1].buff){
      rt.CamServer->send(404,"",""); 
      return;
    } 
    format = (int)rt.picstore[bnum-1].format;
  }

  String response = "HTTP/1.1 200 OK\r\n";
  response += "Content-disposition: inline; filename=cap.";
  if(format == (int)PIXFORMAT_JPEG){
    response += "jpg\r\nContent-type: image/jpeg\r\n";
  } else {
    response += "bin\r\nContent-type: image/x-tas-binary\r\n";
  }
  response += "Content-Length: ";
  char tmp[12];

  if (!bnum) {
    if (rt.taskRunning == 1){
      Access_WaitUntilZero(&rt.taskTakePic, 1, 1000);
      mSupport::AutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
      if (rt.snapshotStore.len) {
        response += itoa(rt.snapshotStore.len, tmp, 10);
        response += "\r\n\r\n";

        WiFiClient client = rt.CamServer->client();
        rt.CamServer->sendContent(response);
        client.write((char *)rt.snapshotStore.buff, rt.snapshotStore.len);
        client.stop();
        Picture_Free(&rt.snapshotStore);
      } else {
        rt.CamServer->send(404,"",""); 
        ALOG_INF(PSTR(D_LOG_CAMERA "No img #: %d"), bnum);
        return;
      }
    } else {
      rt.CamServer->send(404,"",""); 
      return;
    }
  } else {
    mSupport::AutoMutex localmutex(&WebcamMutex, "HandleImage", 200);
    bnum--;
    if (!rt.picstore[bnum].len) {
      rt.CamServer->send(404,"",""); 
      ALOG_INF(PSTR(D_LOG_CAMERA "No img #: %d"), bnum);
      return;
    }
    response += itoa(rt.picstore[bnum].len, tmp, 10);
    response += "\r\n\r\n";
    WiFiClient client = rt.CamServer->client();
    rt.CamServer->sendContent(response);
    client.write((char *)rt.picstore[bnum].buff, rt.picstore[bnum].len);
    client.stop();
  }

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "Sending image #: %d"), bnum+1);
}

void mCamera::Web_SendPictureStore(struct mCamera::PICSTORE *ps)
{

  if (!ps || !ps->buff) {
    AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_CAMERA "img pres"));
    rt.CamServer->send(404,"",""); 
    return;
  }
  // convert mono to rgb8

  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  // use a malloc that we don't free to save memory creep
  // it is re-mallcoed if the frame does not fit.
  bool conv = false;

  // allocate a new picture every time to avoid holding up task thread by using OurOneJpeg
  struct mCamera::PICSTORE psout = {0};

  if (ps->format != PIXFORMAT_JPEG) {
#ifdef USE_WEBCAM_MOTION
    ALOG_INF(PSTR(D_LOG_CAMERA "image will be encoded from %d"), ps->format);

    mSupport::AutoMutex localmutex(&WebcamMutex, "HandleImagemotion", 2000);
    // use a malloc that we don't free to save memory creep
    // it is re-mallcoed if the frame does not fit.
    ALOG_INF(PSTR(D_LOG_CAMERA "%x %d %d %d"), ps->buff, ps->len, ps->width, ps->height);
    conv = WcencodeToJpeg(ps->buff, ps->len, ps->width, ps->height, (int)ps->format, 80, &psout);
    if (!conv) ALOG_INF(PSTR(D_LOG_CAMERA "image encode failed"));

    _jpg_buf_len = psout.len;
    _jpg_buf = psout.buff;
    ALOG_INF(PSTR(D_LOG_CAMERA "encoded %X %d (%dx%d) -> %X %d (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      _jpg_buf, _jpg_buf_len,
      psout.allocatedLen
    );
#endif    
  } else {
    _jpg_buf_len = ps->len;
    _jpg_buf = ps->buff;
    
    ALOG_INF(PSTR(D_LOG_CAMERA "already jpeg %X %d (%dx%d) (%d)"),
      ps->buff, ps->len, ps->width, ps->height,
      ps->allocatedLen
    );
  }

  if (_jpg_buf_len){
    WiFiClient client = rt.CamServer->client();
    String response = "HTTP/1.1 200 OK\r\n";
    response += "Content-disposition: inline; filename=cap.jpg\r\n";
    response += "Content-type: image/jpeg\r\n\r\n";
    rt.CamServer->sendContent(response);
    client.write((char *)_jpg_buf, _jpg_buf_len);
    client.stop();
    if (conv){
      Picture_Free(&psout);
    }
    return; // don't send 500
  }

  ALOG_INF(PSTR(D_LOG_CAMERA "image could not be encoded"));
  if (conv){
    Picture_Free(&psout);
  }
  rt.CamServer->send(500,"",""); 
  return;
}

void mCamera::Stream_HandleMjpeg(int type) {
  if(!Web_CheckPrivilegedAccess()){
    rt.CamServer->send(403,"","");
    return;
  }
  mSupport::AutoMutex localmutex(&WebcamMutex, "HandleWebcamMjpeg", 200);
  mCamera::wc_client *client = new mCamera::wc_client;
  client->active = 1;
  client->type = type;
  client->p_next = rt.client_p;
  client->client = rt.CamServer->client();
  rt.client_p = client;
  
  ALOG_INF(PSTR(D_LOG_CAMERA "Create client"));
  
}

void mCamera::Stream_HandleMjpegRoot(void) {
  tkr_camera->Stream_HandleMjpeg(0);
}

void mCamera::Stream_HandleMjpegDiff(void) {
  tkr_camera->Stream_HandleMjpeg(1);
}

void mCamera::Web_HandleRoot(void) {
  if(!tkr_camera->Web_CheckPrivilegedAccess()){
    tkr_camera->rt.CamServer->send(403,"","");
    return;
  }
  //CamServer->redirect("http://" + String(ip) + ":81/cam.mjpeg");
  tkr_camera->rt.CamServer->sendHeader("Location", "/stream");
  tkr_camera->rt.CamServer->send(302, "", "");
  ALOG_INF(PSTR(D_LOG_CAMERA "Root called"));
}

void mCamera::Web_HandleJpegCapture(AsyncWebServerRequest *request)
{
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb || fb->len == 0) {
    if (fb) esp_camera_fb_return(fb);
    request->send(503, "text/plain", "Camera capture failed");
    return;
  }

  // Optional: ensure it's JPEG. If not, you need fmt conversion.
  if (fb->format != PIXFORMAT_JPEG) {
    esp_camera_fb_return(fb);
    request->send(500, "text/plain", "Framebuffer not JPEG");
    return;
  }

  // State holder so we can return fb after last chunk
  struct FbState {
    camera_fb_t *fb;
    bool returned;
  };
  auto *st = new FbState{fb, false};

  AsyncWebServerResponse *response =
    request->beginResponse("image/jpeg", fb->len,
      [st](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
        if (!st || !st->fb) return 0;

        const size_t remaining = st->fb->len - index;
        const size_t toCopy = (remaining > maxLen) ? maxLen : remaining;

        if (toCopy) memcpy(buffer, st->fb->buf + index, toCopy);

        // When finished, return fb exactly once and free state
        if ((index + toCopy) >= st->fb->len) {
          if (!st->returned) {
            st->returned = true;
            esp_camera_fb_return(st->fb);
          }
          delete st;
        }

        return toCopy;
      });

  response->addHeader("Cache-Control", "no-store");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Connection", "close"); // helps browsers that keep-alive aggressively
  request->send(response);
}

void mCamera::EndRTSP(){ // kill all rtsp clients
  // we should use a mutext here, in case we are currently sending
  mSupport::AutoMutex localmutex(&WebcamMutex, "EndRTSP", 2000);
  wc_rtspclient * volatile rtsp_client = rt.rtsp_client;
  wc_rtspclient * volatile * prev = &rt.rtsp_client;
  while (rtsp_client) {
    delete rtsp_client->rtsp_session;
    rtsp_client->rtsp_session = nullptr;
    delete rtsp_client->camStreamer;
    rtsp_client->camStreamer = nullptr;
    wc_rtspclient *next = rtsp_client->p_next;
    delete rtsp_client;
    rtsp_client = next;
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CAMERA "RTSP stop"));
  }
  rt.rtsp_client = nullptr;
  stats.activeClients.rtsp = 0;
}

void mCamera::Web_AddStreamButton(void) {
  // if streaming is enabled (1 or 2), start stream server
  if (tkr_iDrivers->webcam_config.stream) {
//    if (!rt.CamServer || !rt.up) {
    if (!rt.CamServer) {
      Stream_SetEnabled(tkr_iDrivers->webcam_config.stream);
    }
  }

  // if (!rt.CamServer){
  //   WSContentSend_P(PSTR("<p></p><center>Cam Server Not Running'</center><p></p>"));
  //   WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcstream2", "Turn On Streaming");
  // } else {
  //   if (!rt.up){
  //     WSContentSend_P(PSTR("<p></p><center>Cam Not Running Err 0x%x</center><p></p>"), rt.lastCamError);
  //     WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcinit", "Try WCINIT");
  //   } else {
  //     if (tkr_iDrivers->webcam_config.spare15) {
  //       WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideodisable%200", "Turn On Video");
  //     }
  //   }
  // }

  // // spare15 is 'hide cam on menu'
  // if (!tkr_iDrivers->webcam_config.spare15 && tkr_iDrivers->webcam_config.stream && rt.CamServer && rt.up!=0) {
  //   // Give the webcam webserver some time to prepare the stream - catch error in JS
  //   WSContentSend_P(PSTR("<p></p><center><img onerror='setTimeout(()=>{this.src=this.src;},1000)' src='http://%_I:81/stream' alt='Webcam stream' style='width:99%%;'></center><p></p>"),(uint32_t)WiFi.localIP());
  //   WSContentSend_P(HTTP_WEBCAM_MENUVIDEOCONTROL, "wcmenuvideodisable%201", "Turn Off Video");
  // }
}

void mCamera::RTSP_SetFrame(camera_fb_t* fb) {
  if (rtsp_streamer && fb) {
    rtsp_streamer->setframe(fb->buf, fb->len);
  }
}

void mCamera::RTSP_ClearFrame() {
  if (rtsp_streamer) {
    rtsp_streamer->clearframe();
  }
}

void mCamera::RTSP_StreamTick(uint32_t now) {
  if (rtsp_streamer) {
    rtsp_streamer->streamImage(now);
  }
}

void mCamera::CommandSet_GetFrame(int bnum) {
  // returns size
  // int bnum = 0;
  // if (tkr_events->XdrvMailbox.index){
  //   bnum = tkr_events->XdrvMailbox.index;
  // } else {
  //   bnum = tkr_events->XdrvMailbox.payload;
  // }
  if (bnum == 0) bnum = 1;
  uint32_t res = Frame_RequestCaptureToStore(bnum);
  struct PICSTORE *p = nullptr;
  res = PictureStore_Get(bnum-1, &p);
  char resp[100] = "0";
  if (p) {
    snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  } else {
    snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
      res);
  }
  // Response_P(S_JSON_COMMAND_XVALUE, tkr_events->XdrvMailbox.command, resp);

  ALOG_DBG(PSTR(D_LOG_CAMERA "Getframe %d -> %d"), bnum, res);
  // ResponseCmndNumber((int)res);
}

void mCamera::CommandSet_GetPicStore(int bnum) {
  // returns size
  // use a dummy for buffer ptr
  uint8_t *t = nullptr;
  // int bnum = tkr_events->XdrvMailbox.payload;
  // if (bnum == -99){
  //   bnum = tkr_events->XdrvMailbox.index;
  // }
  if (bnum < 0 || bnum > MAX_PICSTORE) {
    return;  // Command Error
  }

  // if given 0, then get frame 1 first, and use frame 1 (the first frame, index 0).
  if (bnum == 0){
    bnum = 1;
    uint32_t res = Frame_RequestCaptureToStore(bnum);
  }
  //uint32_t res = WcGetPicstore(bnum-1, &t);
  struct PICSTORE *p = nullptr;
  uint32_t res = PictureStore_Get(bnum-1, &p);
  ALOG_DBG(PSTR(D_LOG_CAMERA "PicStore %d at 0x%x"), bnum, p);
  char resp[100] = "0";
  if (p) {
    snprintf_P(resp, sizeof(resp), PSTR("{\"buff\":%d,\"addr\":%d,\"len\":%d,\"w\":%d,\"h\":%d,\"format\":%d}"), 
      bnum, p->buff, p->len, p->width, p->height, p->format+1);
  } else {
    snprintf_P(resp, sizeof(resp), PSTR("{\"maxstore\":%d}"), 
      res);
  }
  // Response_P(S_JSON_COMMAND_XVALUE, tkr_events->XdrvMailbox.command, resp);
}



#ifdef ENABLE_RTSPSERVER

mCamera::localOV2640Streamer::localOV2640Streamer(SOCKET aClient, int width, int height)
  : CStreamer(aClient, width, height) {
  clearframe();
}

void mCamera::localOV2640Streamer::setframe(BufPtr ptr, int len) {
  f_ptr = ptr;
  f_len = len;
}

void mCamera::localOV2640Streamer::clearframe() {
  f_ptr = nullptr;
  f_len = 0;
}

void mCamera::localOV2640Streamer::streamImage(uint32_t curMsec) {
  if (!f_ptr) return;
  streamFrame(f_ptr, f_len, curMsec);
}

// void mCamera::RTSP_SetFrame(camera_fb_t* fb) {
//   if (rtsp_streamer && fb) {
//     rtsp_streamer->setframe(fb->buf, fb->len);
//   }
// }

// void mCamera::RTSP_ClearFrame() {
//   if (rtsp_streamer) {
//     rtsp_streamer->clearframe();
//   }
// }

// void mCamera::RTSP_StreamTick(uint32_t now) {
//   if (rtsp_streamer) {
//     rtsp_streamer->streamImage(now);
//   }
// }

#endif

#endif // ENABLE_DRIVERS_CAMERA