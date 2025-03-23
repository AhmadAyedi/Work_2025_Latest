#ifndef LIN44_H
#define LIN44_H

#include <SoftwareSerial.h>

class LIN44
{
public:
    LIN44(uint8_t rxPin, uint8_t txPin, uint8_t slpPin);
    void begin();
    void sendFrame(uint8_t id, uint8_t *data, uint8_t length);
    void receiveFrame();

private:
    uint8_t calculateParity(uint8_t id);
    uint8_t calculateChecksum(uint8_t id, uint8_t *data, uint8_t length);
    SoftwareSerial linSerial;
    uint8_t slpPin;
};

#endif
