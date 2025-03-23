#include <LIN44.h>

LIN44 linMaster(3, 2, 4); // RX_PIN, TX_PIN, SLP_PIN

void setup()
{
    linMaster.begin();
}

void loop()
{
    uint8_t id = 0x17;                             // Example LIN ID
    uint8_t message[] = {'h', 'e', 'l', 'l', 'o'}; // "hello"
    uint8_t length = sizeof(message);

    Serial.println("Sending LIN Frame...");
    linMaster.sendFrame(id, message, length);

    delay(2000); // Send every 2 seconds
}
