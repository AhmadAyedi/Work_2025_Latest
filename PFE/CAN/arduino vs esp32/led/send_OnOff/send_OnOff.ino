#include <mcp_can.h>
#include <SPI.h>

#define CAN_CS_PIN 10   // Chip Select pin for MCP2515
#define BUTTON_PIN 4    // Push button pin

MCP_CAN CAN0(CAN_CS_PIN);

bool ledState = false; // Keep track of LED state (false = OFF, true = ON)
bool lastButtonState = HIGH; // Previous button state
bool currentButtonState = HIGH; // Current button state

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
    // Read the current button state
    currentButtonState = digitalRead(BUTTON_PIN);

    // Check if button was pressed (transition from HIGH to LOW)
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        // Toggle LED state
        ledState = !ledState;
        
        // Prepare the data to send over CAN bus (1 for ON, 0 for OFF)
        byte data[1];
        data[0] = ledState ? 1 : 0;  // Send 1 for ON, 0 for OFF

        // Send the button state over CAN bus (Message ID 0x200)
        byte sndStat = CAN0.sendMsgBuf(0x200, 0, 1, data);
        if (sndStat == CAN_OK) {
            Serial.println("Button State Sent Successfully!");
        } else {
            Serial.println("Error Sending Data...");
        }

        delay(200); // Debounce delay (prevent multiple toggles from a single press)
    }

    // Update the lastButtonState to the current button state for the next loop
    lastButtonState = currentButtonState;

    delay(50); // Small delay to avoid excessive checking of button state
}
