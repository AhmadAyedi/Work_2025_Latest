#include <SoftwareSerial.h>

#define TX_PIN 2  
#define RX_PIN 3  
#define SLP_PIN 4 
#define BUTTON_PIN 5 

SoftwareSerial linSerial(RX_PIN, TX_PIN);

bool lastButtonState = HIGH;

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); 

    pinMode(BUTTON_PIN, INPUT_PULLUP); 

    linSerial.begin(9600);
    Serial.begin(9600); 

    Serial.println("Slave 1 Ready");
}

void loop() {
    bool buttonState = digitalRead(BUTTON_PIN);

    // Check for button press (LOW) and change from previous state
    if (buttonState == LOW && lastButtonState == HIGH) {
        linSerial.println("button_pressed");
        Serial.println("Sent: button_pressed");
        delay(300);  // Debounce delay
    }

    lastButtonState = buttonState;
}
