#include <SoftwareSerial.h>

#define TX_PIN 2  // Connect to UART-to-LIN module TX
#define RX_PIN 3  // Connect to UART-to-LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600);
    Serial.begin(9600);

    Serial.println("Master Ready");
}

void loop() {
    // Poll Raspberry Pi 1 for button stater
    linSerial.print("poll:1\n");
    Serial.println("Polling Raspberry Pi 1...");

    delay(500); // Wait for response

    if (linSerial.available()) {
        String response = linSerial.readStringUntil('\n');
        Serial.println("Received: " + response);

        if (response.indexOf(":pressed") > 0) {
            Serial.println("Button pressed! Notifying Raspberry Pi 2...");
            linSerial.print("2:toggle_led\n"); // Send command to Raspberry Pi 2
        }
    }

    delay(2000); // Poll every 2 seconds
}
