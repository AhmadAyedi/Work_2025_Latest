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

  std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04};
  lin.writeFrame(0x10, data); // Ignore return value
  Serial.println("Initial frame sent (assumed success)");
}

void loop()
{
  std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC, 0xDD};
  lin.writeFrame(0x10, data); // Ignore return value
  Serial.println("Frame sent in loop (assumed success)");
  delay(1000);
}