#include <Arduino.h>
#include "LinFrameTransfer.hpp"

#define LIN_TX_PIN 17 // UART2 TX (unused)
#define LIN_RX_PIN 16 // UART2 RX

HardwareSerial linSerial(2);
LinFrameTransfer lin(linSerial, Serial, 0); // Verbose off

void setup()
{
  Serial.begin(115200);
  linSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, -1); // RX only
  linSerial.flush();
  delay(800);
  Serial.println("Slave ready, listening for frame ID 0x10");
}

void loop()
{
  auto result = lin.readFrame(0x10, 3);
  if (result.has_value())
  {
    Serial.print("Received data: ");
    for (uint8_t byte : result.value())
    {
      Serial.print("0x");
      Serial.print(byte, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  delay(100); // Sync with master
}