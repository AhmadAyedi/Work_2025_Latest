#include <SoftwareSerial.h>

// Initialize serial communication with Raspberry Pi (Master)
SoftwareSerial linSerial(3, 2);  // RX, TX pins

// Define relay pins for relays 1 to 4
int relayPins[] = {4, 5, 6, 7};  // Relay 1 to 4 connected to digital pins 2-5


void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  linSerial.begin(9600);

  // Initialize relay pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);  // Ensure relays are off initially
  }

  Serial.println("Slave 1 Ready");
}

void loop() {
  if (linSerial.available() > 0) {
    String message = linSerial.readStringUntil('\n');  // Read the incoming message

    // Process relay control messages
    if (message.startsWith("relay_")) {
      int relayNumber = message.charAt(6) - '0';  // Extract relay number (1-4)
      
      // Check if it's a toggle command (ON or OFF)
      if (message.indexOf("on") != -1) {
        // Turn on the relay
        digitalWrite(relayPins[relayNumber - 1], HIGH);
        Serial.println("Relay " + String(relayNumber) + " ON");
        delay(1000);  // Wait for 1 second
        digitalWrite(relayPins[relayNumber - 1], LOW);  // Turn off relay
        Serial.println("Relay " + String(relayNumber) + " OFF");
      }
    }
  }
}
