#include <Arduino.h>
#include <LinFrameTransfer.hpp>

#define RX_PIN 16 // RX pin for Serial2
#define TX_PIN 17 // TX pin for Serial2

HardwareSerial SerialLin(2);                // Use Serial2 on ESP32
LinFrameTransfer lin(SerialLin, Serial, 1); // Verbose level 1 for debugging

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // Wait for Serial to initialize

  SerialLin.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN); // LIN baud rate: 19.2k
  lin.baud = 19200;
  lin.rxPin = RX_PIN;
  lin.txPin = TX_PIN;

  Serial.println("LIN Master Test Started");
}

void loop()
{
  std::vector<uint8_t> data = {0xAA, 0xBB, 0xCC};
  if (lin.writeFrame(0x10, data))
  {
    Serial.println("Frame sent successfully");
  }
  else
  {
    Serial.println("Failed to send frame");
  }
  delay(1000); // Wait 1 second
}