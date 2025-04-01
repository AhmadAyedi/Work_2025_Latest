// Slave Arduino Code for LIN Communication with TJA1021
#include <SoftwareSerial.h>

// Define pins for SoftwareSerial
#define LIN_RX_PIN 10  // Connect to TJA1021 TXD (pin 1)
#define LIN_TX_PIN 11  // Connect to TJA1021 RXD (pin 4)
SoftwareSerial linSerial(LIN_RX_PIN, LIN_TX_PIN);

// Alternatively, use hardware Serial (uncomment if using hardware Serial)
// #define linSerial Serial

#define SLP_PIN 9  // Connect to TJA1021 SLP_N (pin 5)

uint8_t received_data[8];
uint8_t data_len = 0;
bool frame_received = false;

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);
  while (!Serial);

  // Initialize LIN serial communication
  linSerial.begin(19200);  // Match master's baud rate

  // Set SLP_N pin to HIGH to enable TJA1021
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH);

  Serial.println("LIN Slave Starting...");
}

void loop() {
  // Simple LIN frame reception (break detection not handled by TJA1021)
  if (linSerial.available() >= 2) {  // At least sync + PID
    // Look for sync field (0x55)
    if (linSerial.read() == 0x55) {
      // Read PID
      uint8_t pid = linSerial.read();
      uint8_t id = pid & 0x3F;  // Extract ID from PID
      if (id == 0x01) {  // This slave responds to ID 0x01
        data_len = 5;  // Expected data length ("Hello")
        for (uint8_t i = 0; i < data_len; i++) {
          while (!linSerial.available());  // Wait for data
          received_data[i] = linSerial.read();
        }
        // Read checksum
        while (!linSerial.available());
        uint8_t checksum = linSerial.read();

        // Verify checksum
        uint8_t calc_checksum = Compute_Checksum(received_data, data_len, pid);
        if (calc_checksum == checksum) {
          Serial.print("Received LIN Frame - PID: 0x");
          Serial.print(pid, HEX);
          Serial.print(", Data: ");
          for (uint8_t i = 0; i < data_len; i++) {
            Serial.print("0x");
            Serial.print(received_data[i], HEX);
            Serial.print(" ");
          }
          Serial.print(", Checksum: 0x");
          Serial.println(checksum, HEX);
        } else {
          Serial.println("Checksum Error!");
        }
      }
    }
  }
}

uint8_t Compute_Checksum(uint8_t *data, uint8_t len, uint8_t pid) {
  uint16_t sum = pid;
  for (uint8_t i = 0; i < len; i++) {
    sum += data[i];
    if (sum >= 256) sum -= 255;
  }
  return (uint8_t)(~sum);
}