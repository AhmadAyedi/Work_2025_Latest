#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

// LIN Configuration
#define LIN_TX 2
#define LIN_RX 3
SoftwareSerial linSerial(LIN_RX, LIN_TX);

// CAN Configuration
#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

// Define IDs
#define BUTTON1_ID 1  // Button 1 → Green LED
#define BUTTON2_ID 2  // Button 2 → Yellow LED
#define SLAVE1_ID 1   // Raspberry Pi 1 (Green LED)
#define SLAVE2_ID 2   // Raspberry Pi 2 (Yellow LED)

void setup() {
    Serial.begin(9600);
    linSerial.begin(9600);

    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);
    }
    CAN.setMode(MCP_NORMAL);
}

void loop() {
    byte len = 0;
    byte buf[2];
    long unsigned int canId;

    // Check for CAN messages
    if (CAN.readMsgBuf(&canId, &len, buf)) {
        int buttonId = buf[0];  // First byte = Button ID
        int buttonState = buf[1];  // Second byte = Button State

        Serial.print("Received Button ");
        Serial.print(buttonId);
        Serial.print(" State: ");
        Serial.println(buttonState);

        // Route button state to corresponding LED
        if (buttonId == BUTTON1_ID) {
            linSerial.print(SLAVE1_ID);
            linSerial.print(",");
            linSerial.println(buttonState == LOW ? "LED_ON" : "LED_OFF");
        } else if (buttonId == BUTTON2_ID) {
            linSerial.print(SLAVE2_ID);
            linSerial.print(",");
            linSerial.println(buttonState == LOW ? "LED_ON" : "LED_OFF");
        }

        delay(100);
    }
}
