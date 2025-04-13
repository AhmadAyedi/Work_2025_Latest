#include <Arduino.h>
#include "LinFrameTransfer.hpp"

#define LIN_TX_PIN 17
#define LIN_RX_PIN 16

HardwareSerial linSerial(2);
LinFrameTransfer lin(linSerial, Serial, 0); // Verbose off

void setup()
{
  Serial.begin(115200);
  linSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);
  linSerial.flush();
  delay(1000);
  Serial.println("Master ready, sending data on frame ID 0x10");
}

void loop()
{
  std::vector<uint8_t> data = {0x01, 0x02, 0x03};
  Serial.print("Sending: ");
  for (uint8_t byte : data)
  {
    Serial.print("0x");
    Serial.print(byte, HEX);
    Serial.print(" ");
  }
  Serial.println();
  lin.writeFrame(0x10, data);
  Serial.println("Frame sent");
  delay(100); // instead of 1000 in esp esp
}