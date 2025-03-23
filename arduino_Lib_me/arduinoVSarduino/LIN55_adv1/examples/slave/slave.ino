#include <LIN55.h>

LIN55 linSlave(3, 2, 4, 6); // RX, TX, SLP pin, LED pin

void setup()
{
    linSlave.begin();
}

void loop()
{
    linSlave.processLINFrame();
    delay(10);
}
