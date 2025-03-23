#include <SoftwareSerial.h>

#define TX_PIN 2  // LIN TX
#define RX_PIN 3  // LIN RX
#define SLP_PIN 4 // Sleep pin
#define LED_PIN 5 // LED for blinking

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX
bool blinkPattern = false;

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN bus communication
    Serial.begin(9600); // Debugging

    Serial.println("Slave 1 Ready");
}

void loop() {
    if (linSerial.available()) {
        String receivedData = linSerial.readStringUntil('\n');
        int delimiterIndex = receivedData.indexOf(':');
        
        if (delimiterIndex > 0) {
            String id = receivedData.substring(0, delimiterIndex);
            String command = receivedData.substring(delimiterIndex + 1);

            if (id == "1") {
                blinkPattern = false;
                digitalWrite(LED_PIN, LOW);
                Serial.println("Stop all patterns");
            } else if (id == "2" && command == "blink") {
                blinkPattern = true;
                Serial.println("Blink pattern activated");
            }
        }
    }

    if (blinkPattern) {
        digitalWrite(LED_PIN, HIGH);
        delay(500);
        digitalWrite(LED_PIN, LOW);
        delay(500);
    }
}
