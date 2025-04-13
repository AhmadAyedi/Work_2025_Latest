void setup() {
  Serial.begin(19200, SERIAL_8N1);
  while (!Serial) {
    ; // Wait for Serial to initialize
  }
  Serial.println("LIN Slave Started");
}

uint8_t calculatePid(uint8_t frameId) {
  uint8_t p0 = (bitRead(frameId, 0) ^ bitRead(frameId, 1) ^ bitRead(frameId, 2) ^ bitRead(frameId, 4));
  uint8_t p1 = !(bitRead(frameId, 1) ^ bitRead(frameId, 3) ^ bitRead(frameId, 4) ^ bitRead(frameId, 5));
  return (frameId & 0x3F) | (p0 << 6) | (p1 << 7);
}

uint8_t calculateChecksum(uint8_t pid, uint8_t* data, uint8_t dataLen) {
  uint16_t sum = pid; // LIN 2.x includes PID
  for (uint8_t i = 0; i < dataLen; i++) {
    sum += data[i];
    if (sum >= 256) sum -= 255;
  }
  return ~sum;
}

void sendResponse() {
  uint8_t pid = calculatePid(0x20); // PID = 0xA0
  uint8_t data[2] = {0xDE, 0xAD};
  uint8_t checksum = calculateChecksum(pid, data, 2);

  Serial.write(0x00); // Break approximation
  Serial.write(0x55); // Sync
  Serial.write(pid);  // PID
  Serial.write(data[0]);
  Serial.write(data[1]);
  Serial.write(checksum);
  Serial.flush();

  // Debug response
  Serial.print("Sent response: 0x");
  Serial.print(data[0], HEX);
  Serial.print(" 0x");
  Serial.print(data[1], HEX);
  Serial.print(" (Checksum: 0x");
  Serial.print(checksum, HEX);
  Serial.println(")");
}

void loop() {
  if (Serial.available() > 0 && Serial.read() == 0x55) {
    unsigned long start = millis();
    while (Serial.available() == 0 && millis() - start < 100) delay(1);
    if (Serial.available() == 0) {
      Serial.println("Timeout waiting for PID");
      return;
    }
    uint8_t pid = Serial.read();

    // Data frame (ID 0x10, PID 0x50)
    if (pid == calculatePid(0x10)) {
      uint8_t data[3];
      for (int i = 0; i < 3; i++) {
        while (Serial.available() == 0 && millis() - start < 100) delay(1);
        if (Serial.available() == 0) {
          Serial.println("Timeout waiting for data");
          return;
        }
        data[i] = Serial.read();
      }
      while (Serial.available() == 0 && millis() - start < 100) delay(1);
      if (Serial.available() == 0) {
        Serial.println("Timeout waiting for checksum");
        return;
      }
      uint8_t checksum = Serial.read();
      uint8_t expectedChecksum = calculateChecksum(pid, data, 3);

      if (checksum == expectedChecksum) {
        Serial.print("Received data: ");
        for (int i = 0; i < 3; i++) {
          Serial.print("0x");
          if (data[i] < 0x10) Serial.print("0");
          Serial.print(data[i], HEX);
          Serial.print(" ");
        }
        Serial.println();
      } else {
        Serial.print("Data checksum error: Expected 0x");
        Serial.print(expectedChecksum, HEX);
        Serial.print(", Got 0x");
        Serial.println(checksum, HEX);
      }
    }
    // Poll frame (ID 0x20, PID 0xA0)
    else if (pid == calculatePid(0x20)) {
      while (Serial.available() == 0 && millis() - start < 100) delay(1);
      if (Serial.available() == 0) {
        Serial.println("Timeout waiting for poll checksum");
        return;
      }
      uint8_t checksum = Serial.read();
      uint8_t data[0] = {};
      uint8_t expectedChecksum = calculateChecksum(pid, data, 0);

      Serial.print("Poll PID: 0x");
      Serial.print(pid, HEX);
      Serial.print(", Checksum: 0x");
      Serial.print(checksum, HEX);
      Serial.print(", Expected: 0x");
      Serial.println(expectedChecksum, HEX);

      if (checksum == expectedChecksum) {
        Serial.println("Poll received, sending response");
        sendResponse();
      } else {
        Serial.println("Poll checksum error");
      }
    } else {
      Serial.print("Unknown PID: 0x");
      Serial.println(pid, HEX);
    }
  }
}