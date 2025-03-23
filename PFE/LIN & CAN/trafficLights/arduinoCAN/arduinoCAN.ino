#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

#define BUTTON_ACTIVATE 2   // Button to start traffic light cycle
#define BUTTON_DEACTIVATE 3 // Button to stop traffic light cycle

bool lastActivateState = HIGH;
bool lastDeactivateState = HIGH;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_ACTIVATE, INPUT_PULLUP);
    pinMode(BUTTON_DEACTIVATE, INPUT_PULLUP);

    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);
    }
    CAN.setMode(MCP_NORMAL);
}

void loop() {
    bool activateState = digitalRead(BUTTON_ACTIVATE);
    bool deactivateState = digitalRead(BUTTON_DEACTIVATE);

    // Send activation signal only when button is pressed
    if (activateState == LOW && lastActivateState == HIGH) {
        byte data[2] = {1, 1};  // Button ID = 1 (Activation), Value = 1 (Pressed)
        CAN.sendMsgBuf(0x100, 0, 2, data);
        Serial.println("Sent Activation Command");
        delay(300);  // Small debounce delay
    }
    lastActivateState = activateState;

    // Send deactivation signal only when button is pressed
    if (deactivateState == LOW && lastDeactivateState == HIGH) {
        byte data[2] = {2, 1};  // Button ID = 2 (Deactivation), Value = 1 (Pressed)
        CAN.sendMsgBuf(0x100, 0, 2, data);
        Serial.println("Sent Deactivation Command");
        delay(300);  // Small debounce delay
    }
    lastDeactivateState = deactivateState;
}
