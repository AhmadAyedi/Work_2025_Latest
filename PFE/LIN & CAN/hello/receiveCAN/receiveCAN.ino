#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

void setup() {
    Serial.begin(9600); // Start serial communication for debugging

    // Initialize CAN Module
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);  // Stay here if initialization fails
    }
    CAN.setMode(MCP_NORMAL); // Set to normal mode for receiving messages

    Serial.println("CAN Receiver Ready");
}

void loop() {
    // Check if CAN message is available
    if (CAN.checkReceive() == CAN_MSGAVAIL) {
        long unsigned int canId;
        byte len = 0;
        byte buf[8];

        // Read the received CAN message
        CAN.readMsgBuf(&canId, &len, buf);

        Serial.print("Received CAN Message: ");
        for (int i = 0; i < len; i++) {
            Serial.write(buf[i]);
        }
        Serial.println();
    }
}
