#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Nacho WiFi";
const char* password = "zach5312";

AsyncWebServer server(80);

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
    request->send(200, "text/plain", "Hello, World");
  });

  //start server
  server.begin();
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  setup_server();
}

void loop() {
  //delay(5000);  // Adjust for your desired frequency

}
