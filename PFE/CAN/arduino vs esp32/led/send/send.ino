#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS_PIN 10   // Chip Select pin for MCP2515
#define BUTTON_PIN 4    // Push button pin

MCP_CAN CAN0(CAN_CS_PIN);

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up resistor

    // Initialize CAN bus
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
        Serial.println("MCP2515 Initialized Successfully!");
    } else {
        Serial.println("Error Initializing MCP2515...");
        while (1);
    }

    CAN0.setMode(MCP_NORMAL);
}

void loop() {
    // Read the button state (pressed = LOW, not pressed = HIGH)
    bool buttonState = digitalRead(BUTTON_PIN) == LOW;

    byte data[1];
    data[0] = buttonState ? 1 : 0;  // 1 for ON, 0 for OFF

    // Send the button state over CAN bus
    byte sndStat = CAN0.sendMsgBuf(0x200, 0, 1, data); // Message ID 0x200
    if (sndStat == CAN_OK) {
        Serial.println("Button State Sent Successfully!");
    } else {
        Serial.println("Error Sending Data...");
    }

    delay(200); // Send data every 200ms
}
