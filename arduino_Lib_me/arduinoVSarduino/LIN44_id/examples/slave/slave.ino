#include <LIN44.h>

LIN44 linSlave(3, 2, 4); // RX_PIN, TX_PIN, SLP_PIN

void setup()
{
    linSlave.begin();
}

void loop()
{
    linSlave.receiveFrame();
}
