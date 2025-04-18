#include <SoftwareSerial.h>

#define TX_PIN 2    // LIN TX
#define RX_PIN 3    // LIN RX
#define SLP_PIN 4   // Sleep pin
#define BUTTON_PIN 5 // Button on pin 5

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

const byte SLAVE_ID = 0x01; // LIN ID for Slave 1

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    pinMode(BUTTON_PIN, INPUT_PULLUP); // Button with pull-up

    linSerial.begin(9600); // LIN Communication
    Serial.begin(9600);     // Debugging

    Serial.println("Slave 1 Ready");
}

void loop() {
    if (linSerial.available()) {
        byte received_id = linSerial.read(); // Read the ID from master
        Serial.print("Received ID: ");
        Serial.println(received_id, HEX);

        if (received_id == SLAVE_ID && digitalRead(BUTTON_PIN) == LOW) {
            linSerial.println("Arduino1: Button pressed successfully");
            Serial.println("Sent: Arduino1: Button pressed successfully");
            delay(500); // Debounce delay
        }
    }
}
