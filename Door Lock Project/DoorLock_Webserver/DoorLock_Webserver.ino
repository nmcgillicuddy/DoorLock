/***
  This program sets up a webserver to open and close a "lock" in the form of a servo motor. Controls are
  operational fully from the webserver.
***/

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ESP32Servo.h>

const char* ssid = "Nacho WiFi";
const char* password = "zach5312";
#define servoPin 8

Servo doorLock;
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Door Lock System</title>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
                background-color: #f4f4f4;
            }

            header {
                background-image: linear-gradient(rgb(31, 49, 129), rgb(94, 109, 240));
                text-align: center;
                padding: 5px ;
                font-family: 'Brush Script MT';
                font-size: large
            }

            .btn-container {
                display: flex;
                flex-direction: column;
                align-items: center;
                gap: 15px;
                margin: 10px;
            }

            .btn {
                color: white;
                width: 400px;
                height: 75px;
                font-size: 16px;
                cursor: pointer;
                text-align: center;
                border: none;
                border-radius: 10px;
                font-weight: bold;
                /*box-shadow: 0px 0px 4px 4px rgba(0, 0, 0, 0.3);*/
            }

            #btn-open {
                background-color: rgb(4, 161, 4);
            }

            #btn-open:hover {
                background-color: rgb(4, 116, 63);
            }

            #btn-close {
                background-color: red;
            }

            #btn-close:hover {
                background-color: rgb(124, 3, 3);
            }

            #btn-forceLock {
                background-color: rgb(131, 131, 131);
            }

            #btn-forceLock:hover {
                background-color: rgb(63, 63, 63);
            }

            footer {
                text-align: center;
                position:absolute;
                bottom:0;
                width:100%;
                height:60px;
                background: linear-gradient(rgb(31, 49, 129), rgb(94, 109, 240));
                font-weight: bold;
            }

        </style>
    </head>
    <body>
        <header>
            <h1>Door Lock System</h1>
        </header>
        <div class="btn-container">
            <button class="btn" id="btn-open"> Open</button>
            <button class="btn" id="btn-close"> Close</button>
            <button class="btn" id="btn-forceLock"> Force Lock</button>
        </div>
        <footer>
            <p>By Nate McGillicuddy</p>
        </footer>
        <script>
            document.addEventListener("DOMContentLoaded", function() {
                const openBtn = document.getElementById('btn-open');
                const closeBtn = document.getElementById('btn-close');
                const forceLockBtn = document.getElementById('btn-forceLock');

                openBtn.addEventListener('click', function() {
                    fetch('/open', { method: 'POST'})
                        .then(response => response.text())
                        .then(data => alert(data));
                });

                closeBtn.addEventListener('click', function() {
                    fetch('/close', { method: 'POST'})
                        .then(response => response.text())
                        .then(data => alert(data));
                });

                forceLockBtn.addEventListener('click', function() {
                    fetch('/forceLock', { method: 'POST'})
                        .then(response => response.text())
                        .then(data => alert(data));
                });
            });
        </script>
    </body>
</html>
)rawliteral";

void setup_wifi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
}

void setup_server() {
  //turn server on
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  server.on("/open", HTTP_POST, [](AsyncWebServerRequest *request) {
    openDoor();
    request->send(200, "text/plain", "Successfully opened the door!");
  });

  server.on("/close", HTTP_POST, [](AsyncWebServerRequest *request) {
    closeDoor();
    request->send(200, "text/plain", "Successfully closeed the door!");
  });

  server.on("/forceLock", HTTP_POST, [](AsyncWebServerRequest *request) {
    lockDoor();
    request->send(200, "text/plain", "The door is now locked for any entry.");
  });

  //start server
  server.begin();
}

void setup_servo() {
  doorLock.setPeriodHertz(50);
  doorLock.attach(servoPin, 500, 2400);
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_server();
  setup_servo();
}

void loop() {

}

void openDoor() {
  doorLock.write(90);
  delay(15);
}

void closeDoor() {
  doorLock.write(0);
  delay(15);
}

void lockDoor() {
  //unfinished
  //ultimate goal is to use a transistor as a switch so open and close won't work
  //if the door is locked
}