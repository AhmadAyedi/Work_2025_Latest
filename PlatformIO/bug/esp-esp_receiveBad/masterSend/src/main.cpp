#include <Arduino.h>
#include <LinFrameTransfer.hpp>

// Define pins
#define LIN_TX_PIN 17  // Serial1 TX
#define LIN_RX_PIN 16  // Serial1 RX
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

  Serial.println("LIN Master Initialized");
}

void sendLinFrame()
{
  std::vector<uint8_t> data = {0x01, 0x02, 0x03, 0x04}; // 4 bytes of data
  if (lin.writeFrame(0x10, data))
  {
    Serial.println("Frame sent successfully AYEDI");
  }
  else
  {
    Serial.println("Failed to send frame AYEDI");
  }
}

void loop()
{
  sendLinFrame();
  delay(1000); // Send every 1 second
}