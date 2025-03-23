#include <LIN.h>

// Define pins
#define TX_PIN 2
#define RX_PIN 3
#define SLP_PIN 4

LIN lin(RX_PIN, TX_PIN, SLP_PIN); // Create LIN object

void setup()
{
    lin.begin(); // Initialize LIN communication
}

void loop()
{
    lin.receiveFrame(); // Receive the frame from master and display it
    delay(100);         // Check for data every 100ms
}
