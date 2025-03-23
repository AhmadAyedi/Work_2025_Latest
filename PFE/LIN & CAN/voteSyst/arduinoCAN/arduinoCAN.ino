#include <SPI.h>
#include <mcp_can.h>
#include <SoftwareSerial.h>

#define CAN_CS 10
MCP_CAN CAN(CAN_CS);

// Button Pins
#define BUTTON_JACK 2  // Vote for Jack (Green)
#define BUTTON_ALEX 3  // Vote for Alex (Red)

bool lastJackState = HIGH;
bool lastAlexState = HIGH;

// Vote Counters
int jackVotes = 0;
int alexVotes = 0;

// LIN Serial (Connects to Raspberry Pis)
SoftwareSerial linSerial(7, 8); // TX: 7, RX: 8

void setup() {
    Serial.begin(9600);
    linSerial.begin(9600);

    pinMode(BUTTON_JACK, INPUT_PULLUP);
    pinMode(BUTTON_ALEX, INPUT_PULLUP);

    if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
        Serial.println("CAN Module Initialized!");
    } else {
        Serial.println("CAN Module Initialization Failed!");
        while (1);
    }
    CAN.setMode(MCP_NORMAL);
}

void loop() {
    bool jackState = digitalRead(BUTTON_JACK);
    bool alexState = digitalRead(BUTTON_ALEX);

    // Detect Jack Vote
    if (jackState == LOW && lastJackState == HIGH) {
        jackVotes++;
        sendCANMessage(1);
        Serial.println("Vote for Jack Sent");
        updateWinner();
        delay(300);
    }
    lastJackState = jackState;

    // Detect Alex Vote
    if (alexState == LOW && lastAlexState == HIGH) {
        alexVotes++;
        sendCANMessage(2);
        Serial.println("Vote for Alex Sent");
        updateWinner();
        delay(300);
    }
    lastAlexState = alexState;
}

// 📨 **Send CAN Message**
void sendCANMessage(int voteId) {
    byte data[2] = {voteId, 1};
    CAN.sendMsgBuf(0x100, 0, 2, data);
}

// 🔥 **Determine Winner & Send LIN Command**
void updateWinner() {
    if (jackVotes > alexVotes) {
        sendLINMessage("1,LED_ON");  // Jack is winning -> Green LED ON
        sendLINMessage("2,LED_OFF"); // Alex is losing -> Red LED OFF
        Serial.println("Jack is Winning! Green LED ON");
    } else if (alexVotes > jackVotes) {
        sendLINMessage("1,LED_OFF"); // Jack is losing -> Green LED OFF
        sendLINMessage("2,LED_ON");  // Alex is winning -> Red LED ON
        Serial.println("Alex is Winning! Red LED ON");
    } else {
        sendLINMessage("1,LED_OFF"); // Tie -> Both LEDs OFF
        sendLINMessage("2,LED_OFF");
        Serial.println("TIE! No LED is ON");
    }
}

// 📡 **Send LIN Message**
void sendLINMessage(String command) {
    linSerial.println(command);
    delay(100);
}
