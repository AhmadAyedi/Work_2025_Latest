#include <Arduino.h>

// Define UART communication pins
#define UART_TX 1
#define UART_RX 3

// Initialize Serial object for UART communication
HardwareSerial mySerial(0);  // Using UART0

void setup() {
    Serial.begin(115200);  // USB Serial Monitor
    mySerial.begin(115200, SERIAL_8N1, UART_RX, UART_TX); // UART communication
    //Serial.println("Waiting for UART data...");
}

void loop() {
    if (mySerial.available()) {  // Check if data is received
        String receivedData = mySerial.readString();  // Read the received string
        Serial.print("Received: ");
        Serial.println(receivedData);  // Print to Serial Monitor
    }
    delay(1000);  // Wait 1 second before checking again
}
