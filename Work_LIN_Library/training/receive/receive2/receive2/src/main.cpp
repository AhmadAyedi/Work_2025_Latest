#include <Arduino.h>
#include "LinFrameTransfer.hpp"
#include "LinPDU.hpp"

#define LIN_TX_PIN 17  // UART2 TX (unused)
#define LIN_RX_PIN 16  // UART2 RX

HardwareSerial linSerial(2);
LinFrameTransfer lin(linSerial, Serial, 0);  // Verbose off

void setup() {
  Serial.begin(115200);
  linSerial.begin(19200, SERIAL_8N1, LIN_RX_PIN, -1);  // RX only
  linSerial.flush();  // Harmless
  delay(1000);        // Stabilize startup
  Serial.println("Slave ready, listening for all frames");
}

void loop() {
  // Rotate through IDs to avoid locking on one
  static uint8_t nextId = 0;
  uint8_t idToCheck;
  size_t dataLength;

  switch (nextId) {
    case 0:
      idToCheck = 0x10;
      dataLength = 2;
      break;
    case 1:
      idToCheck = 0x11;
      dataLength = 4;
      break;
    case 2:
      idToCheck = 0x3C;
      dataLength = 8;
      break;
    default:
      nextId = 0;
      idToCheck = 0x10;
      dataLength = 2;
  }

  // Check one ID per loop iteration
  auto result = lin.readFrame(idToCheck, dataLength);
  if (result.has_value()) {
    std::vector<uint8_t> frameData = result.value();
    if (idToCheck == 0x10) {
      Serial.print("Received ID 0x10: ");
      for (uint8_t byte : frameData) {
        Serial.print("0x");
        Serial.print(byte, HEX);
        Serial.print(" ");
      }
      Serial.println();
    } else if (idToCheck == 0x11) {
      Serial.print("Received ID 0x11: ");
      for (uint8_t byte : frameData) {
        Serial.print("0x");
        Serial.print(byte, HEX);
        Serial.print(" ");
      }
      Serial.println();
    } else if (idToCheck == 0x3C) {
      PDU pdu;
      std::copy(frameData.begin(), frameData.end(), reinterpret_cast<uint8_t*>(&pdu.common));
      if (pdu.getNAD() == PDU::NAD_Type::SLEEP) {
        Serial.println("Sleep command received (ID 0x3C)");
      } else {
        Serial.print("Received ID 0x3C (unknown): ");
        for (uint8_t byte : frameData) {
          Serial.print("0x");
          Serial.print(byte, HEX);
          Serial.print(" ");
        }
        Serial.println();
      }
    }
  }

  // Cycle to next ID
  nextId = (nextId + 1) % 3;
  delay(50);  // Poll frequently, but check one ID at a time
}