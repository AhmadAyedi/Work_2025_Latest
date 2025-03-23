#ifndef LIN_H
#define LIN_H

#include <SoftwareSerial.h>

class LIN
{
private:
    SoftwareSerial _linSerial;
    int _slpPin;

public:
    LIN(int rxPin, int txPin, int slpPin);
    void begin();
    void sendFrame(const String &frame);
    void receiveFrame();
};

#endif
