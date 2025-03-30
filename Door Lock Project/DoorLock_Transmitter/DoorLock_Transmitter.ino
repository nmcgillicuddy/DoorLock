/**
Transmitter side of the door lock program using ESP-NOW communication to send user ID to
the designated receiver ESP32.
*/

#include <esp_now.h>
#include <WiFi.h>

//receiver MAC address
uint8_t receiverMAC[] = {0x68, 0xB6, 0xB3, 0x3D, 0x86, 0xE8};

//struct for data, update to contain more necessary variables
typedef struct struct_message {
  char userID[20]; //change to actual UID size later
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Send Success" : "Send Fail");
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  WiFi.mode(WIFI_STA);
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  Serial.println("ESP-NOW ready");

  //add callback function
  esp_now_register_send_cb(onDataSent);

  // Register peer
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  strcpy(myData.userID, "123456");

  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }

  delay(5000);
}
