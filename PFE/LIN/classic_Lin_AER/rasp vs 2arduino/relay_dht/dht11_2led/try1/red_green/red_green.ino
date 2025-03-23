#include <SoftwareSerial.h>

// LIN communication pins
#define TX_PIN 2  
#define RX_PIN 3  
#define SLP_PIN 4 

// LED pins
#define RED_LED_PIN 7
#define GREEN_LED_PIN 6

SoftwareSerial linSerial(RX_PIN, TX_PIN);

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); 

    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);

    linSerial.begin(9600);
    Serial.begin(9600);

    Serial.println("Slave 2 (LED Control) Ready");
}

void loop() {
    if (linSerial.available()) {
        String frame = linSerial.readStringUntil('\n');
        frame.trim();
        Serial.println("Received Frame: " + frame);

        // Parse LIN Frame ID and Command
        if (frame.startsWith("0x20:")) { // 0x20 is the ID for LED control
            String command = frame.substring(5);

            if (command == "red") {
                digitalWrite(RED_LED_PIN, HIGH);
                digitalWrite(GREEN_LED_PIN, LOW);
                Serial.println("RED LED ON, GREEN LED OFF");
            } 
            else if (command == "green") {
                digitalWrite(RED_LED_PIN, LOW);
                digitalWrite(GREEN_LED_PIN, HIGH);
                Serial.println("GREEN LED ON, RED LED OFF");
            }
        }
    }
}
