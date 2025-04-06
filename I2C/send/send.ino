#include <Wire.h>

void setup() {
  Wire.begin(); // Initialize I2C as Master (no address needed)
  Serial.begin(9600); // Start Serial for debugging
  Serial.println("Master ready");
}

void loop() {
  Wire.beginTransmission(8); // Begin transmission to Slave at address 8
  Wire.write("Hello");       // Send the message "Hello"
  Wire.endTransmission();    // End transmission
  Serial.println("Sent: Hello"); // Debug message
  delay(1000); // Wait 1 second before sending again
}