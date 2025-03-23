#include <SPI.h>
#include <mcp_can.h>

#define CAN0_INT 2  // Interrupt pin
MCP_CAN CAN0(4);    // Set CS to pin 5

void setup() {
    Serial.begin(115200);
    
    // Initialize MCP2515 at 16MHz with 500kbps baud rate
    while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) != CAN_OK) {
        Serial.println("Error Initializing MCP2515...");
        delay(100);
    }
    Serial.println("MCP2515 Initialized Successfully!");
    
    CAN0.setMode(MCP_NORMAL); // Set to normal mode to receive messages
    pinMode(CAN0_INT, INPUT); // Set INT pin as input
}

void loop() {
    if (!digitalRead(CAN0_INT)) { // Check if data is available
        long unsigned int rxId;
        unsigned char len;
        unsigned char rxBuf[8];

        CAN0.readMsgBuf(&rxId, &len, rxBuf); // Read message

        Serial.print("Received data from ID: 0x");
        Serial.println(rxId, HEX);
        
        Serial.print("Data: ");
        for (int i = 0; i < len; i++) {
            Serial.print(rxBuf[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
}
