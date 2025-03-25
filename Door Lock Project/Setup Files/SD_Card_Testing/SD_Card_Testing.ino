#include <SPI.h>
#include <SD.h>

#define SCK_PIN 18
#define MISO_PIN 16
#define MOSI_PIN 17
#define SS_PIN 14


void setup() {
  Serial.begin(115200);
  while (!Serial) {
    // Wait for Serial to initialize
  }

  // Initialize SPI bus
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SS_PIN);

  // Initialize SD card
  //Serial.println(SD.begin(SS_PIN));
  if (!SD.begin(SS_PIN)) {
    Serial.println("Card Mount no worky 2 2!!!");
    testSPI();
    return;
  }
  Serial.println("Card Mount worked 1 1!!!");
}

void testSPI() {
  Serial.println("Testing SPI communication...");

  // Set SS_PIN low to select the SPI device
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, LOW);

  // Send a byte and receive a byte
  byte sentData = 0xAB; // Example data to send
  byte receivedData = SPI.transfer(sentData);

  // Set SS_PIN high to deselect the SPI device
  digitalWrite(SS_PIN, HIGH);

  // Print received data
  Serial.print("Sent data: 0x");
  Serial.print(sentData, HEX);
  Serial.print(" Received data: 0x");
  Serial.println(receivedData, HEX);
}

void loop() {
  // Nothing to do here
}
