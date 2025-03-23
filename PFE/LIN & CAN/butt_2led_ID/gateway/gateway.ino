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

// Define Slave IDs
#define SLAVE1_ID 1  // Raspberry Pi 1 (Green LED)
#define SLAVE2_ID 2  // Raspberry Pi 2 (Yellow LED)

void setup() {
    Serial.begin(9600); // Debugging
    linSerial.begin(9600); // LIN Communication

    // Initialize CAN Module
    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1); // Stay here if initialization fails
    }
    CAN.setMode(MCP_NORMAL); // Set to normal mode for receiving messages
    Serial.println("LIN Master & CAN Gateway Ready");
}

void loop() {
    byte len = 0;
    byte buf[8];
    long unsigned int canId;

    // Check for CAN messages
    if (CAN.readMsgBuf(&canId, &len, buf)) {
        int buttonState = buf[0];  // Assuming button state is stored in the first byte

        // Print button state for debugging
        Serial.print("Button State: ");
        Serial.println(buttonState);

        // Send separate frames with IDs
        if (buttonState == LOW) {  // Button pressed
            linSerial.print(SLAVE1_ID);
            linSerial.println(",LED_ON");  // Send to Slave 1 (Green LED)
            delay(10);
            linSerial.print(SLAVE2_ID);
            linSerial.println(",LED_ON");  // Send to Slave 2 (Yellow LED)
            Serial.println("Sent: ID=1,LED_ON | ID=2,LED_ON");
        } else {  // Button not pressed
            linSerial.print(SLAVE1_ID);
            linSerial.println(",LED_OFF"); // Send to Slave 1 (Green LED)
            delay(10);
            linSerial.print(SLAVE2_ID);
            linSerial.println(",LED_OFF"); // Send to Slave 2 (Yellow LED)
            Serial.println("Sent: ID=1,LED_OFF | ID=2,LED_OFF");
        }

        delay(500); // Wait before checking again
    }
}
