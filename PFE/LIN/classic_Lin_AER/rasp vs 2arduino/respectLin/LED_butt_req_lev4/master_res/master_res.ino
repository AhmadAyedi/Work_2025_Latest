#include <SoftwareSerial.h>

#define TX_PIN 2  
#define RX_PIN 3  
#define SLP_PIN 4 
#define LED_PIN 6  

SoftwareSerial linSerial(RX_PIN, TX_PIN);

bool ledState = LOW;

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); 

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, ledState); 

    linSerial.begin(9600);
    Serial.begin(9600); 

    Serial.println("Slave 2 Ready");
}

void loop() {
    if (linSerial.available()) {
        String message = linSerial.readStringUntil('\n');
        message.trim();
        Serial.println("Received: " + message);

        if (message == "toggle_led") {
            // Toggle the LED state
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
            Serial.println("LED Toggled");
        }
    }
}
