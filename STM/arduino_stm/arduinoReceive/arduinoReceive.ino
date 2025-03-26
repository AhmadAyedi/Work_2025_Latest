#include <SoftwareSerial.h>

// Define SoftwareSerial pins: RX = 10, TX = 11 (TX not used)
SoftwareSerial mySerial(10, 11); // RX, TX //wire it with D8 of stm PA9 serialTx1

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // Wait for Serial Monitor to connect
  }
  Serial.println("Arduino Ready");

  mySerial.begin(9600);
}

void loop() {
  if (mySerial.available() > 0) {
    String message = mySerial.readStringUntil('\n');
    Serial.println("Received: " + message);
  }
  delay(100);
}