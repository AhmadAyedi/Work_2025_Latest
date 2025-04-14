#include <SPI.h>

char message[] = "HELLO\n"; // Add newline terminator

void setup() {
  Serial.begin(9600);
  SPI.begin();              // Master init
  pinMode(10, OUTPUT);      // SS must be output
  digitalWrite(10, LOW);    // Pull SS low to select slave
}

void loop() {
  for (int i = 0; i < sizeof(message) - 1; i++) {
    SPI.transfer(message[i]);
    delay(10);
  }
  delay(2000); // Send every 2 seconds
}
