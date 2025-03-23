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
    String frame = "hello"; // Data to send
    lin.sendFrame(frame);   // Send the frame over LIN
    delay(500);             // Send every 500ms
}
