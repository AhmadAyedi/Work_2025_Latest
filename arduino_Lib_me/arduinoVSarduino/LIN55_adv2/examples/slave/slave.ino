#include <LIN55.h>

LIN55 linSlave(3, 2, 4); // RX, TX, SLP pin

void setup()
{
    linSlave.begin();
    linSlave.setLEDPin(6); // Set LED pin for slave after initialization
}

void loop()
{
    linSlave.processLINFrame();
    delay(10);
}
