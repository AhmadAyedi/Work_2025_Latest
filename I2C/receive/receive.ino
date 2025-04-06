#include <Wire.h>

void setup() {
  Wire.begin(8); // Initialize I2C as Slave with address 8
  Wire.onReceive(receiveEvent); // Register event handler for receiving data
  Serial.begin(9600); // Start Serial for output
  Serial.println("Slave ready");
}

void loop() {
  delay(100); // Small delay to keep loop running
}

// Function to handle received data
void receiveEvent(int byteCount) {
  while (Wire.available()) { // While there’s data to read
    char c = Wire.read();    // Read a character
    Serial.print(c);         // Print it to Serial Monitor
  }
  Serial.println(); // New line after message
}