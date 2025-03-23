#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

#define BUTTON_PIN 2  // Button connected to pin 2

void setup() {
    Serial.begin(9600); // Start serial communication for debugging
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Configure button input

    // Initialize CAN Module
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);  // Stay here if initialization fails
    }
    CAN.setMode(MCP_NORMAL); // Set to normal mode for receiving messages
    Serial.println("CAN Sender Ready");
}

void loop() {
    int buttonState = digitalRead(BUTTON_PIN); // 0 when not pressed, 1 when pressed

    // Print button state to Serial Monitor for debugging
    Serial.print("Button State: ");
    Serial.println(buttonState);

    // Send the button state (LED status) to CAN bus
    byte data[1] = {buttonState};  // Sending 1 byte (button state)
    CAN.sendMsgBuf(0x100, 0, 1, data);  // CAN ID 0x100, 1 byte of data

    delay(500);  // Update every 500 ms
}
