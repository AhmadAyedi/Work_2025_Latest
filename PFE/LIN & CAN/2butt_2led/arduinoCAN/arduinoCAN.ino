#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

#define BUTTON1 2  // First Button (Triggers Green LED)
#define BUTTON2 3  // Second Button (Triggers Yellow LED)

bool lastButton1State = HIGH;
bool lastButton2State = HIGH;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);

    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);
    }
    CAN.setMode(MCP_NORMAL);
}

void loop() {
    bool button1State = digitalRead(BUTTON1);
    bool button2State = digitalRead(BUTTON2);

    // Check Button 1 state change
    if (button1State != lastButton1State) {
        lastButton1State = button1State;
        byte data[2] = {1, button1State};  // Button ID = 1
        CAN.sendMsgBuf(0x100, 0, 2, data);
        Serial.print("Sent Button 1 State: ");
        Serial.println(button1State);
    }

    // Check Button 2 state change
    if (button2State != lastButton2State) {
        lastButton2State = button2State;
        byte data[2] = {2, button2State};  // Button ID = 2
        CAN.sendMsgBuf(0x100, 0, 2, data);
        Serial.print("Sent Button 2 State: ");
        Serial.println(button2State);
    }

    delay(100);
}
