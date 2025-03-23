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

// Button IDs (Voters)
#define JACK_VOTE_ID 1
#define ALEX_VOTE_ID 2

// LED IDs for Raspberry Pis
#define JACK_LED_ID 1  // Green LED on Raspberry Pi 1 (Jack)
#define ALEX_LED_ID 2  // Red LED on Raspberry Pi 2 (Alex)

// Vote Counters
int jackVotes = 0;
int alexVotes = 0;

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
    checkCANMessages();  // Always check for new votes
    updateWinningLed();  // Update the winner LED
}

// 📡 **Check for Incoming Votes (CAN Messages)**
void checkCANMessages() {
    byte len = 0;
    byte buf[2];
    long unsigned int canId;

    if (CAN.checkReceive() == CAN_MSGAVAIL) {  // Message received
        CAN.readMsgBuf(&canId, &len, buf);

        int voteId = buf[0];

        if (voteId == JACK_VOTE_ID) {
            jackVotes++;
            Serial.print("Vote for Jack! Total: ");
            Serial.println(jackVotes);
            sendLinCommand(JACK_LED_ID, "LED_ON");  // Flash Jack's LED (Green)
            delay(500);
            sendLinCommand(JACK_LED_ID, "LED_OFF");
        } else if (voteId == ALEX_VOTE_ID) {
            alexVotes++;
            Serial.print("Vote for Alex! Total: ");
            Serial.println(alexVotes);
            sendLinCommand(ALEX_LED_ID, "LED_ON");  // Flash Alex's LED (Red)
            delay(500);
            sendLinCommand(ALEX_LED_ID, "LED_OFF");
        }
    }
}

// 🚦 **Update Winner LED**
void updateWinningLed() {
    if (jackVotes > alexVotes) {
        sendLinCommand(JACK_LED_ID, "LED_ON");
        sendLinCommand(ALEX_LED_ID, "LED_OFF");
        Serial.println("Jack is Winning! (Green LED ON)");
    } else if (alexVotes > jackVotes) {
        sendLinCommand(JACK_LED_ID, "LED_OFF");
        sendLinCommand(ALEX_LED_ID, "LED_ON");
        Serial.println("Alex is Winning! (Red LED ON)");
    } else {
        sendLinCommand(JACK_LED_ID, "LED_OFF");
        sendLinCommand(ALEX_LED_ID, "LED_OFF");
        Serial.println("Tie - No winner yet.");
    }
}

// 📨 **Send LIN Command**
void sendLinCommand(int ledId, const char *command) {
    linSerial.print(ledId);
    linSerial.print(",");
    linSerial.println(command);
}
