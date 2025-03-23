#include <Arduino.h>
#include <LinFrameTransfer.hpp>

// Define pins
#define LIN_TX_PIN 9   // Serial1 TX
#define LIN_RX_PIN 10  // Serial1 RX
#define LIN_NSLP_PIN 5 // Transceiver enable pin

HardwareSerial &linSerial = Serial1; // LIN bus communication
Stream &debugSerial = Serial;        // Debugging output

LinFrameTransfer lin(linSerial, debugSerial, 1); // Verbose level 1 for debug

void setup()
{
  Serial.begin(115200); // Debug output
  while (!Serial)
  {
  }

  // Initialize LIN transceiver
  pinMode(LIN_NSLP_PIN, OUTPUT);
  digitalWrite(LIN_NSLP_PIN, HIGH); // Normal mode

  // Initialize LIN serial
  linSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, LIN_TX_PIN);
  lin.baud = 19200; // Ensure library uses correct baud rate

  Serial.println("LIN Slave Initialized");
}

void receiveLinFrame()
{
  unsigned long start = millis();
  auto response = lin.readFrame(0x10, 4); // Expect 4 bytes
  if (response.has_value())
  {
    Serial.printf("Received data in %lu ms: ", millis() - start);
    for (uint8_t byte : response.value())
    {
      Serial.print(byte, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
  else
  {
    Serial.printf("No valid frame received in %lu ms\n", millis() - start);
  }
}

void loop()
{
  receiveLinFrame();
  delay(10); // Poll every 10ms (was 100ms)
}