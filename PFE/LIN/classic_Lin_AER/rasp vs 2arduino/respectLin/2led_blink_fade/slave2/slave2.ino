#include <SoftwareSerial.h>

#define TX_PIN 2  // LIN TX
#define RX_PIN 3  // LIN RX
#define SLP_PIN 4 // Sleep pin
#define LED_PIN 6 // LED for fading

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX
bool fadePattern = false;
int brightness = 0;
int fadeAmount = 5;

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN bus communication
    Serial.begin(9600); // Debugging

    Serial.println("Slave 2 Ready");
}

void loop() {
    if (linSerial.available()) {
        String receivedData = linSerial.readStringUntil('\n');
        int delimiterIndex = receivedData.indexOf(':');
        
        if (delimiterIndex > 0) {
            String id = receivedData.substring(0, delimiterIndex);
            String command = receivedData.substring(delimiterIndex + 1);

            if (id == "1") {
                fadePattern = false;
                analogWrite(LED_PIN, 0);
                Serial.println("Stop all patterns");
            } else if (id == "3" && command == "fade") {
                fadePattern = true;
                Serial.println("Fade pattern activated");
            }
        }
    }

    if (fadePattern) {
        analogWrite(LED_PIN, brightness);
        brightness += fadeAmount;
        if (brightness <= 0 || brightness >= 255) {
            fadeAmount = -fadeAmount;
        }
        delay(30);
    }
}
