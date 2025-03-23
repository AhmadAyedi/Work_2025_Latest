#include <SoftwareSerial.h>

#define TX_PIN 2  // Connect to UART-to-LIN module TX
#define RX_PIN 3  // Connect to UART-to-LIN module RX
#define SLP_PIN 4 // Sleep pin to wake up the LIN module

SoftwareSerial linSerial(RX_PIN, TX_PIN); // RX, TX

struct LinMessage {
    int id;
    String message;
};

// Define a schedule table
LinMessage scheduleTable[] = {
    {1, "Hello Raspberry 1"},
    {2, "Hello Raspberry 2"}
};

const int scheduleSize = sizeof(scheduleTable) / sizeof(scheduleTable[0]);

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN bus communication
    Serial.begin(9600); // Debugging

    Serial.println("Master Ready");
}

void send_message(int message_id, String message) {
    String formattedMessage = String(message_id) + ":" + message + "\n";
    linSerial.print(formattedMessage);
    Serial.println("Sent: " + formattedMessage);
}

void loop() {
    for (int i = 0; i < scheduleSize; i++) {
        send_message(scheduleTable[i].id, scheduleTable[i].message);
        delay(2000); // Scheduling interval
    }
}
