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
    uint8_t id = 0x01;                                                 // Example ID (1)
    uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x00}; // Example data
    uint8_t dataLength = sizeof(data) / sizeof(data[0]);

    Serial.print("Payload length : ");
    Serial.println(dataLength);

    lin.sendFrame(id, data, dataLength); // Send a LIN frame
    delay(2000);                         // Send frame every 2 seconds
}
