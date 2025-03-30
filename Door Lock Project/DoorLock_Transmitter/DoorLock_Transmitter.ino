/**
Transmitter side of the door lock program using ESP-NOW communication to send user ID to
the designated receiver ESP32.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SCK_PIN 18
#define MISO_PIN 16
#define MOSI_PIN 17
#define SS_PIN 14

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
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

  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to unlock..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  char uidString[20] = {0};
  int index = 0;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    index += sprintf(&uidString[index], "%02X", mfrc522.uid.uidByte[i]);
  }

  strcpy(myData.userID, uidString);

  esp_err_t result = esp_now_send(receiverMAC, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent UID: ");
    Serial.println(myData.userID);
  } else {
    Serial.println("Error sending the data");
  }

  mfrc522.PICC_HaltA();

  delay(5000);
}
