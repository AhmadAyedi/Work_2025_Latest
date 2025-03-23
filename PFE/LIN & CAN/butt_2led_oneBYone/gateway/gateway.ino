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

        // Send corresponding LIN frame for green LED (ID=1) to Raspberry Pi 1
        if (buttonState == LOW) {  // Button pressed
            linSerial.println("1:LED_GREEN_ON");  // Send to Raspberry Pi 1 (Green LED)
            linSerial.println("2:LED_YELLOW_OFF");  // Send to Raspberry Pi 2 (Yellow LED OFF)
            Serial.println("Sent to LIN: LED_GREEN_ON for Slave 1, LED_YELLOW_OFF for Slave 2");
        } else {  // Button not pressed
            linSerial.println("1:LED_GREEN_OFF");  // Send to Raspberry Pi 1 (Green LED OFF)
            linSerial.println("2:LED_YELLOW_ON");  // Send to Raspberry Pi 2 (Yellow LED ON)
            Serial.println("Sent to LIN: LED_GREEN_OFF for Slave 1, LED_YELLOW_ON for Slave 2");
        }

        delay(500); // Wait for a bit before checking again
    }
}
