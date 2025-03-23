#ifndef LIN_H
#define LIN_H

#include <SoftwareSerial.h>

class LIN {
public:
    LIN(uint8_t txPin, uint8_t rxPin, uint8_t slpPin);

    void begin(long baudRate);
    void sendFrame(uint8_t id, uint8_t data);
    void receiveFrame();
    bool isFrameValid(uint8_t sync, uint8_t id, uint8_t data, uint8_t receivedChecksum);

private:
    SoftwareSerial _linSerial;
    uint8_t _slpPin;
};

#endif
