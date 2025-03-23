#include <LIN.h>

#define TX_PIN 2  // TX to LIN module
#define RX_PIN 3  // RX from LIN module
#define SLP_PIN 4 // Sleep pin

LIN lin(TX_PIN, RX_PIN, SLP_PIN);

void setup()
{
    lin.begin(9600); // Initialize LIN communication with 9600 baud rate
}

void loop()
{
    uint8_t id = 0x12;   // Example LIN ID
    uint8_t data = 0x45; // Example data

    lin.sendFrame(id, data); // Send a LIN frame
    delay(1000);             // Send every 1 second
}
