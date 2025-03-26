#include <SoftwareSerial.h>

// Define SoftwareSerial pins: RX = 11, TX = 10
SoftwareSerial mySerial(11, 10); // RX, TX

void setup() {
  // Initialize hardware serial for debugging at 9600 baud
  Serial.begin(9600);
  Serial.println("Arduino initialized");

  // Initialize SoftwareSerial for STM32 communication at 9600 baud
  mySerial.begin(9600);
}

void loop() {
  // Buffer to store incoming data from STM32
  char rx_buffer[20];

  // Check if data is available from STM32 via SoftwareSerial
  if (mySerial.available() > 0) {
    // Read the incoming string until newline
    String received = mySerial.readStringUntil('\n');
    received.trim(); // Remove trailing whitespace/newlines

    // Display received message on Serial Monitor
    Serial.print("Received from STM32: ");
    Serial.println(received);

    // Check if the received message is "REQUEST_DATA"
    if (received == "REQUEST_DATA") {
      // Prepare and send response via SoftwareSerial
      String response = "RESPONSE: Hello from Arduino";
      mySerial.println(response);

      // Display sent message on Serial Monitor
      Serial.print("Sent to STM32: ");
      Serial.println(response);
    }
  }
  delay(100); // Small delay to avoid overwhelming the loop
}