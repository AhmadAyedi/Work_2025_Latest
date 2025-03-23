#ifndef LIN33_H
#define LIN33_H

#include <SoftwareSerial.h>

class LIN33
{
public:
    LIN33(uint8_t txPin, uint8_t rxPin, uint8_t slpPin);
    void begin(long baudRate);
    void sendFrame(uint8_t id, uint8_t *data, uint8_t dataLength);
    void receiveFrame();
    byte calculateChecksum(uint8_t *data, uint8_t length);

private:
    SoftwareSerial _linSerial;
    uint8_t _slpPin;
};

#endif
