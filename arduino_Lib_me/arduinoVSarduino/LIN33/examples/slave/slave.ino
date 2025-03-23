#include <LIN33.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

LIN33 lin(TX_PIN, RX_PIN, SLP_PIN);

void setup()
{
    lin.begin(9600); // Initialize LIN communication with 9600 baud rate
}

void loop()
{
    lin.receiveFrame(); // Continuously listen for LIN frames
}
