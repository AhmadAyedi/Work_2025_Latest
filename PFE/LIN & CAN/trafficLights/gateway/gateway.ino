#include <SoftwareSerial.h>
#include <SPI.h>
#include <mcp_can.h>

// LIN Communication
#define LIN_TX 2
#define LIN_RX 3
SoftwareSerial linSerial(LIN_RX, LIN_TX);

// CAN Communication
#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

// Button IDs
#define ACTIVATE_ID 1
#define DEACTIVATE_ID 2

// LED IDs for LIN Slaves
#define RED_LED_ID 1       // Raspberry Pi 1 (Red LED)
#define YELLOW_LED_ID 2    // Raspberry Pi 2 (Yellow LED)
#define GREEN_LED_ID 3     // Raspberry Pi 2 (Green LED)

bool trafficLightActive = false;
int currentStep = 0;
unsigned long lastStepTime = 0;

// Traffic Light Timings
const unsigned long stepDurations[] = {5000, 2000, 5000};  // Red, Yellow, Green

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
    checkCANMessages();  // Always check for new CAN messages

    if (trafficLightActive) {
        unsigned long currentTime = millis();
        if (currentTime - lastStepTime >= stepDurations[currentStep]) {
            lastStepTime = currentTime;
            updateTrafficLight();
        }
    }
}

// 📡 **Check for Incoming CAN Messages**
void checkCANMessages() {
    byte len = 0;
    byte buf[2];
    long unsigned int canId;

    if (CAN.checkReceive() == CAN_MSGAVAIL) {  // Message received
        CAN.readMsgBuf(&canId, &len, buf);

        int buttonId = buf[0];
        int buttonState = buf[1];

        Serial.print("Received CAN Message - Button ID: ");
        Serial.print(buttonId);
        Serial.print(", State: ");
        Serial.println(buttonState);

        if (buttonState == 1) {  // Ensure button was pressed
            if (buttonId == ACTIVATE_ID) {
                Serial.println("Activation Button Pressed: Starting Traffic Light Cycle");
                trafficLightActive = true;
                currentStep = 0;
                lastStepTime = millis();
                updateTrafficLight();
            } else if (buttonId == DEACTIVATE_ID) {
                Serial.println("Deactivation Button Pressed: Stopping All LEDs");
                trafficLightActive = false;
                turnOffAllLeds();
            }
        }
    }
}

// 🚦 **Update Traffic Light Based on Current Step**
void updateTrafficLight() {
    if (!trafficLightActive) return;

    switch (currentStep) {
        case 0:
            sendLinCommand(RED_LED_ID, "LED_ON");
            sendLinCommand(YELLOW_LED_ID, "LED_OFF");
            sendLinCommand(GREEN_LED_ID, "LED_OFF");
            Serial.println("RED LED ON");
            break;
        case 1:
            sendLinCommand(RED_LED_ID, "LED_OFF");
            sendLinCommand(YELLOW_LED_ID, "LED_ON");
            sendLinCommand(GREEN_LED_ID, "LED_OFF");
            Serial.println("YELLOW LED ON");
            break;
        case 2:
            sendLinCommand(RED_LED_ID, "LED_OFF");
            sendLinCommand(YELLOW_LED_ID, "LED_OFF");
            sendLinCommand(GREEN_LED_ID, "LED_ON");
            Serial.println("GREEN LED ON");
            break;
    }

    // Move to the next step or loop back
    currentStep = (currentStep + 1) % 3;
}

// ❌ **Turn Off All LEDs**
void turnOffAllLeds() {
    sendLinCommand(RED_LED_ID, "LED_OFF");
    sendLinCommand(YELLOW_LED_ID, "LED_OFF");
    sendLinCommand(GREEN_LED_ID, "LED_OFF");
}

// 📨 **Send LIN Command**
void sendLinCommand(int ledId, const char *command) {
    linSerial.print(ledId);
    linSerial.print(",");
    linSerial.println(command);
}
