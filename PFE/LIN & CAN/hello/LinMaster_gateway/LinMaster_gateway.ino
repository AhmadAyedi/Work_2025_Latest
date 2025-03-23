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

void setup() {
    Serial.begin(9600); // Debugging
    linSerial.begin(9600); // LIN Communication

    // Initialize CAN
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);
    }
    CAN.setMode(MCP_NORMAL); // Set CAN mode

    Serial.println("LIN Master & CAN Gateway Ready");
}

void loop() {
    // Send message to LIN Slave
    linSerial.println("1:hello");
    Serial.println("Sent to LIN: hello");

    // Send message to CAN Bus
    char msg[] = "hello";
    CAN.sendMsgBuf(0x100, 0, sizeof(msg), (byte *)msg);
    Serial.println("Sent to CAN: hello");

    delay(2000); // Repeat every 2 seconds
}
