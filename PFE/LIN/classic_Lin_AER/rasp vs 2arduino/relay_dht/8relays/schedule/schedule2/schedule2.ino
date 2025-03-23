#include <SoftwareSerial.h>

// Initialize serial communication with Raspberry Pi (Master)
SoftwareSerial linSerial(3, 2);  // RX, TX pins

// Define relay pins for relays 5 to 8
int relayPins[] = {8, 9, 10, 11};  // Relay 5 to 8 connected to digital pins 6-9

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  linSerial.begin(9600);

  // Initialize relay pins as outputs
  for (int i = 0; i < 4; i++) {
    pinMode(relayPins[i], OUTPUT);
    digitalWrite(relayPins[i], LOW);  // Ensure relays are off initially
  }

  Serial.println("Slave 2 Ready");
}

void loop() {
  if (linSerial.available() > 0) {
    String message = linSerial.readStringUntil('\n');  // Read the incoming message

    // Process relay control messages
    if (message.startsWith("slave2_relay_")) {
      int relayNumber = message.charAt(13) - '0';  // Extract relay number (5-8)
      String action = message.substring(15);  // Get action ("on" or "off")
      
      if (action == "on") {
        // Turn on the relay
        digitalWrite(relayPins[relayNumber - 5], HIGH);
        Serial.println("Relay " + String(relayNumber) + " ON");
      } else if (action == "off") {
        // Turn off the relay
        digitalWrite(relayPins[relayNumber - 5], LOW);
        Serial.println("Relay " + String(relayNumber) + " OFF");
      }
    }
  }
}
