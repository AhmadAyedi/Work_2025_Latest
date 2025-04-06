#include <SPI.h>

volatile char receivedChar;
volatile bool newData = false;

void setup() {
  Serial.begin(115200); // Start Serial for output
  pinMode(MISO, OUTPUT); // Set MISO as output for SPI
  SPCR |= _BV(SPE); // Enable SPI in Slave mode
  SPCR |= _BV(SPIE); // Enable SPI interrupts
  Serial.println("Slave ready");
}

// SPI interrupt routine
ISR(SPI_STC_vect) {
  receivedChar = SPDR; // Read received byte from SPI Data Register
  newData = true; // Flag new data received
}

void loop() {
  if (newData) {
    Serial.print(receivedChar); // Print received character
    //Serial.println();
    newData = false;
  }
}