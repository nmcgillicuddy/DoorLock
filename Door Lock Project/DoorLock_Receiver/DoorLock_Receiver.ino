/**
Receiver side of the door lock program using ESP-NOW communication to receive when a user ID is
send and turns on an LED
*/

#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

#define LED_PIN 3
#define SERVO_PIN 8

//mimicking struct from transmitter
typedef struct struct_message {
  char userID[20];
} struct_message;

struct_message receivedData;

Servo doorLock;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&receivedData, incomingData, sizeof(receivedData));
  
  Serial.print("Received from: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  
  Serial.print("  | UserID: ");
  Serial.println(receivedData.userID);

  //check for the lone valid user (temporary)
  if (strcmp(receivedData.userID, "B3AC43DA") == 0) {
    doorLock.write(90);
  }

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);

  delay(1000);
  doorLock.write(0);
}

void setup() {
  Serial.begin(115200); 
  //setup ESP-NOW communication
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  Serial.println("ESP-NOW ready");

  //register receive callback
  esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));

  //setup LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  //initialize servo
  doorLock.setPeriodHertz(50);
  doorLock.attach(SERVO_PIN, 500, 2400);
}

void loop() {

}
