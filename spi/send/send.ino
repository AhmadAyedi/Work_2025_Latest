#include <SPI.h>

const int SS_PIN = 10; // Slave Select pin

void setup() {
  Serial.begin(115200); // Start Serial for debugging
  pinMode(SS_PIN, OUTPUT); // Set SS pin as output
  digitalWrite(SS_PIN, HIGH); // Deselect Slave initially
  SPI.begin(); // Initialize SPI as Master
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Set SPI clock to 1 MHz (16 MHz / 16)
  Serial.println("Master ready");
}

void loop() {
  digitalWrite(SS_PIN, LOW); // Select Slave
  Serial.println("Sending: Hello");
  
  // Send "Hello" one byte at a time
  const char* message = "Hello";
  for (int i = 0; i < 5; i++) {
    SPI.transfer(message[i]); // Send each character
    delayMicroseconds(100); // Small delay for stability
  }
  
  digitalWrite(SS_PIN, HIGH); // Deselect Slave
  delay(1000); // Wait 1 second before sending again
}