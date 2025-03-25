#include <WiFi.h>

const char* ssid = "Nacho WiFi";
const char* password = "zach5312";

void setup_wifi() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() {
  delay(5000);  // Adjust for your desired frequency
}
