#include <SPI.h>

volatile char receivedChar;
char buffer[32];
volatile int index = 0;
volatile bool messageReady = false;

void setup() {
  Serial.begin(9600);
  pinMode(MISO, OUTPUT);      // Must set MISO as output
  SPCR |= _BV(SPE);           // Enable SPI (Slave)
  SPI.attachInterrupt();      // Attach SPI interrupt
}

ISR(SPI_STC_vect) {
  receivedChar = SPDR;
  
  if (receivedChar == '\n') {
    buffer[index] = '\0';     // Null-terminate string
    messageReady = true;
    index = 0;
  } else {
    buffer[index++] = receivedChar;
    if (index >= sizeof(buffer)) {
      index = 0; // Prevent overflow
    }
  }
}

void loop() {
  if (messageReady) {
    Serial.print("Received: ");
    Serial.println(buffer);
    messageReady = false;
  }
}
