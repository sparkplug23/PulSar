#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>


/**************************************************************************************************
 * Recovery WiFi Configuration
 **************************************************************************************************/

const char* ssid = "HACS2400";
const char* password = "af4d8bc9ab";

const char* hostname = "pulsar-recovery";


/**************************************************************************************************
 * Web Server
 **************************************************************************************************/

WebServer server(80);


/**************************************************************************************************
 * Recovery Web Page
 **************************************************************************************************/

const char recoveryPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>PulSar Recovery</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            background: #111;
            color: #eee;
            text-align: center;
            margin: 0;
            padding: 40px 15px;
        }

        .card {
            max-width: 500px;
            margin: auto;
            background: #222;
            padding: 24px;
            border-radius: 12px;
        }

        h2 {
            margin-top: 0;
        }

        input[type=file] {
            width: 100%;
            margin: 20px 0;
        }

        button {
            padding: 12px 24px;
            font-size: 16px;
            cursor: pointer;
        }

        #progressOuter {
            width: 100%;
            height: 24px;
            margin-top: 20px;
            background: #333;
            border-radius: 12px;
            overflow: hidden;
        }

        #progressInner {
            width: 0%;
            height: 100%;
            background: #4a8;
        }

        #status {
            margin-top: 12px;
        }
    </style>
</head>

<body>

<div class="card">

    <h2>PulSar Recovery</h2>

    <p>Select a compiled firmware .bin file.</p>

    <form id="uploadForm">
        <input id="firmware" type="file" name="update" accept=".bin" required>
        <br>
        <button type="submit">Upload Firmware</button>
    </form>

    <div id="progressOuter">
        <div id="progressInner"></div>
    </div>

    <div id="status">Ready</div>

</div>

<script>

const form = document.getElementById("uploadForm");
const fileInput = document.getElementById("firmware");
const progress = document.getElementById("progressInner");
const statusText = document.getElementById("status");

form.addEventListener("submit", function(event)
{
    event.preventDefault();

    if (!fileInput.files.length)
        return;

    const data = new FormData();
    data.append("update", fileInput.files[0]);

    const xhr = new XMLHttpRequest();

    xhr.open("POST", "/update", true);

    xhr.upload.onprogress = function(event)
    {
        if (event.lengthComputable)
        {
            const percent = Math.round((event.loaded / event.total) * 100);

            progress.style.width = percent + "%";
            statusText.textContent = "Uploading: " + percent + "%";
        }
    };

    xhr.onload = function()
    {
        if (xhr.status === 200)
        {
            statusText.textContent = xhr.responseText;
        }
        else
        {
            statusText.textContent = "Upload failed";
        }
    };

    xhr.onerror = function()
    {
        statusText.textContent = "Network error";
    };

    statusText.textContent = "Starting upload...";

    xhr.send(data);
});

</script>

</body>
</html>
)rawliteral";


/**************************************************************************************************
 * Web Update Handler
 **************************************************************************************************/

void setupWebUpdate()
{
    server.on("/", HTTP_GET, []()
    {
        server.send(200, "text/html", recoveryPage);
    });


    server.on("/update", HTTP_GET, []()
    {
        server.send(200, "text/html", recoveryPage);
    });


    server.on(
        "/update",
        HTTP_POST,

        []()
        {
            server.sendHeader("Connection", "close");

            if (Update.hasError())
            {
                server.send(500, "text/plain", "Update FAILED");
            }
            else
            {
                server.send(200, "text/plain", "Update successful. Rebooting...");
            }

            delay(500);

            if (!Update.hasError())
            {
                ESP.restart();
            }
        },

        []()
        {
            HTTPUpload& upload = server.upload();


            if (upload.status == UPLOAD_FILE_START)
            {
                Serial.println();
                Serial.println("================================");
                Serial.printf("HTTP OTA Start: %s\n", upload.filename.c_str());
                Serial.println("================================");

                Serial.printf("Free sketch space: %u bytes\n", ESP.getFreeSketchSpace());

                if (!Update.begin(UPDATE_SIZE_UNKNOWN, U_FLASH))
                {
                    Serial.println("Update.begin() FAILED");
                    Update.printError(Serial);
                }
            }


            else if (upload.status == UPLOAD_FILE_WRITE)
            {
                if (!Update.hasError())
                {
                    size_t written = Update.write(upload.buf, upload.currentSize);

                    if (written != upload.currentSize)
                    {
                        Serial.println("Update.write() FAILED");
                        Update.printError(Serial);
                    }
                }
            }


            else if (upload.status == UPLOAD_FILE_END)
            {
                if (Update.end(true))
                {
                    Serial.printf(
                        "HTTP OTA Success: %u bytes\n",
                        upload.totalSize
                    );
                }
                else
                {
                    Serial.println("Update.end() FAILED");
                    Update.printError(Serial);
                }
            }


            else if (upload.status == UPLOAD_FILE_ABORTED)
            {
                Serial.println("HTTP OTA Aborted");

                Update.abort();
            }
        }
    );


    server.begin();

    Serial.println("HTTP recovery server started");
}


/**************************************************************************************************
 * Arduino OTA
 **************************************************************************************************/

void setupArduinoOTA()
{
    ArduinoOTA.setHostname(hostname);

    // Default ESP32 ArduinoOTA port is 3232
    ArduinoOTA.setPort(3232);


    ArduinoOTA.onStart([]()
    {
        Serial.println();
        Serial.println("================================");

        if (ArduinoOTA.getCommand() == U_FLASH)
        {
            Serial.println("ArduinoOTA Start: firmware");
        }
        else
        {
            Serial.println("ArduinoOTA Start: filesystem");
        }

        Serial.printf("Free sketch space: %u bytes\n", ESP.getFreeSketchSpace());

        Serial.println("================================");
    });


    ArduinoOTA.onEnd([]()
    {
        Serial.println();
        Serial.println("ArduinoOTA complete");
    });


    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
    {
        if (total)
        {
            Serial.printf(
                "ArduinoOTA Progress: %u%%\r",
                (progress * 100U) / total
            );
        }
    });


    ArduinoOTA.onError([](ota_error_t error)
    {
        Serial.printf("\nArduinoOTA Error[%u]: ", error);

        if (error == OTA_AUTH_ERROR)
            Serial.println("Authentication failed");

        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin failed");

        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connection failed");

        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive failed");

        else if (error == OTA_END_ERROR)
            Serial.println("End failed");

        else
            Serial.println("Unknown error");
    });


    ArduinoOTA.begin();

    Serial.println("ArduinoOTA started");
}


/**************************************************************************************************
 * Setup
 **************************************************************************************************/

void setup()
{
    Serial.begin(115200);

    delay(500);

    Serial.println();
    Serial.println();
    Serial.println("================================");
    Serial.println("PulSar Recovery Firmware");
    Serial.println("================================");


    WiFi.mode(WIFI_STA);

    WiFi.begin(ssid, password);


    Serial.printf("Connecting to %s", ssid);


    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");
    }


    Serial.println();
    Serial.println("WiFi connected");

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());


    if (MDNS.begin(hostname))
    {
        Serial.printf(
            "mDNS: http://%s.local/\n",
            hostname
        );
    }
    else
    {
        Serial.println("mDNS failed");
    }


    setupArduinoOTA();

    setupWebUpdate();


    Serial.println();
    Serial.println("Recovery interfaces ready");

    Serial.print("Web: http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");

    Serial.print("OTA: ");
    Serial.print(WiFi.localIP());
    Serial.println(":3232");

    Serial.println("================================");
}


/**************************************************************************************************
 * Main Loop
 **************************************************************************************************/

void loop()
{
    ArduinoOTA.handle();

    server.handleClient();

    delay(1);
}