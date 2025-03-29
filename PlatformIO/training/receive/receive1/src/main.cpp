#include <Arduino.h>
#include "LinFrameTransfer.hpp"

#define LIN_TX_PIN 17
#define LIN_RX_PIN 16

HardwareSerial LinSerial(2);
LinFrameTransfer lin(LinSerial, Serial, 1);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  LinSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);
  Serial.println("Receiver ready");
}

void loop()
{
  auto response = lin.readFrame(0x10, 4);
  if (response.has_value())
  {
    Serial.print("Received frame: ");
    for (uint8_t byte : response.value())
    {
      Serial.print(byte, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  delay(1100); // Slightly longer than sender’s 1000ms to ensure overlap
}