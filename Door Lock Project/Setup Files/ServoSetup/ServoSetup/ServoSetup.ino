#include <ESP32Servo.h>

#define servoPin 8

Servo doorLock;

void setup() {
  // put your setup code here, to run once:
  doorLock.setPeriodHertz(50);
  doorLock.attach(servoPin, 500, 2400);
}

void loop() {
  // put your main code here, to run repeatedly:
  doorLock.write(90);
  delay(3000);
  doorLock.write(0);
  delay(3000);
}

void openDoor() {
  
}