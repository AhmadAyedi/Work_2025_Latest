#include <SoftwareSerial.h>

#define TX_PIN 2  // Connect to UART to LIN module TX
#define RX_PIN 3  // Connect to UART to LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module
#define BUTTON_PIN 5 // Button connected to pin 5

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    pinMode(BUTTON_PIN, INPUT_PULLUP); // Enable internal pull-up resistor

    linSerial.begin(9600); // LIN communication
    Serial.begin(9600); // Debugging

    Serial.println("Slave 1 Ready");
}

void loop() {
    if (digitalRead(BUTTON_PIN) == LOW) { // Button is pressed (LOW due to pull-up)
        linSerial.println("hello from arduino1");
        Serial.println("Sent: hello from arduino1");
        delay(500); // Debounce delay
    }
}
