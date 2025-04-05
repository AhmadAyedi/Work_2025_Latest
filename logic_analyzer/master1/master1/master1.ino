// Master Arduino Code for LIN Communication with TJA1021
#include <SoftwareSerial.h>

// Define pins for SoftwareSerial (if using Uno, which has only one hardware Serial)
#define LIN_RX_PIN 10  // Connect to TJA1021 TXD (pin 1)
#define LIN_TX_PIN 11  // Connect to TJA1021 RXD (pin 4)
SoftwareSerial linSerial(LIN_RX_PIN, LIN_TX_PIN);

// Alternatively, use hardware Serial (uncomment if using hardware Serial)
// #define linSerial Serial

#define SLP_PIN 9  // Connect to TJA1021 SLP_N (pin 5)

uint8_t lin_frame[20];  // Buffer for LIN frame

// Function prototypes
uint8_t Compute_Parity(uint8_t id);
uint8_t Compute_Checksum(uint8_t *data, uint8_t len, uint8_t pid);
void Send_LIN_Frame(void);

void setup() {
  // Initialize Serial for debugging (via USB)
  Serial.begin(9600);
  while (!Serial);

  // Initialize LIN serial communication
  linSerial.begin(19200);  // LIN baud rate (match with slave and logic analyzer)

  // Set SLP_N pin to HIGH to enable TJA1021 (normal mode)
  pinMode(SLP_PIN, OUTPUT);
  digitalWrite(SLP_PIN, HIGH);

  Serial.println("LIN Master Starting...");
}

void loop() {
  Send_LIN_Frame();
  delay(1000);  // Send every 1 second
}

void Send_LIN_Frame(void) {
  uint8_t sync_field = 0x55;  // Sync
  uint8_t id = 0x01;          // Example ID (1)
  uint8_t pid = id | (Compute_Parity(id) << 6); // PID with parity
  uint8_t data[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F}; // "Hello" in ASCII
  uint8_t data_len = sizeof(data);
  uint8_t checksum = Compute_Checksum(data, data_len, pid);

  // Prepare the LIN frame (excluding break)
  lin_frame[0] = sync_field;  // Sync field
  lin_frame[1] = pid;         // PID
  for (uint8_t i = 0; i < data_len; i++) {
    lin_frame[i + 2] = data[i];  // Data bytes
  }
  lin_frame[data_len + 2] = checksum;  // Checksum

  // Send Break (13 bits low)
  // TJA1021 doesn't have a direct break command, so we emulate it
  linSerial.end();  // Stop UART
  pinMode(LIN_TX_PIN, OUTPUT);
  digitalWrite(LIN_TX_PIN, LOW);  // Send break (low for 13 bits at 19200 baud)
  delayMicroseconds(677);  // 13 bits at 19200 baud = 677 µs
  digitalWrite(LIN_TX_PIN, HIGH);  // End break
  delayMicroseconds(52);  // 1 bit time at 19200 baud = 52 µs (inter-frame space)
  linSerial.begin(19200);  // Restart UART

  // Send the rest of the frame
  linSerial.write(lin_frame, data_len + 3);

  // Debug output
  Serial.print("Sent LIN Frame - Sync: 0x");
  Serial.print(sync_field, HEX);
  Serial.print(", PID: 0x");
  Serial.print(pid, HEX);
  Serial.print(", Data: ");
  for (uint8_t i = 0; i < data_len; i++) {
    Serial.print("0x");
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.print(", Checksum: 0x");
  Serial.println(checksum, HEX);
}

uint8_t Compute_Parity(uint8_t id) {
  if (id > 0x3F) {  // ID must be 6 bits (0 to 63)
    while (1);  // Error handling
  }
  uint8_t p0 = (id & 0x01) ^ ((id >> 1) & 0x01) ^ ((id >> 2) & 0x01) ^ ((id >> 4) & 0x01);
  uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^ ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01)) & 0x01;
  return (p0 & 0x01) | ((p1 & 0x01) << 1);
}

uint8_t Compute_Checksum(uint8_t *data, uint8_t len, uint8_t pid) {
  uint16_t sum = pid;
  for (uint8_t i = 0; i < len; i++) {
    sum += data[i];
    if (sum >= 256) sum -= 255;
  }
  return (uint8_t)(~sum);
}