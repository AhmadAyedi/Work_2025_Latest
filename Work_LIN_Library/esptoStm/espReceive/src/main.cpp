#include <Arduino.h>
#include "LinFrameTransfer.hpp"

#define LIN_RX_PIN 16
#define LIN_TX_PIN 17

HardwareSerial linSerial(2);
LinFrameTransfer lin(linSerial, Serial, 1); // Verbose on for debugging

void setup()
{
  Serial.begin(115200);
  linSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);
  linSerial.flush();
  delay(2000); // Extended initial delay to sync with STM32 startup
  Serial.println("ESP32 LIN Receiver ready, listening for frame ID 0x10");
}

void loop()
{
  static int timeout_count = 0; // Track consecutive timeouts

  // Clear UART buffer
  linSerial.flush();

  // Listen for frame ID 0x10, expecting 3 bytes
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
    timeout_count = 0; // Reset timeout counter on success
  }
  else
  {
    timeout_count++;
    if (timeout_count >= 3) // Only print after 3 consecutive timeouts
    {
      Serial.println("Listening for frame ID 0x10... No frame received or invalid");
    }
  }
  delay(500); // Keep 500ms loop for responsiveness
}