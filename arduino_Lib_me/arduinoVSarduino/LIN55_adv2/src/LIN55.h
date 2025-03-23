#ifndef LIN55_H
#define LIN55_H

#include <SoftwareSerial.h>

class LIN55
{
public:
    // Constructor for both master and slave
    LIN55(uint8_t rxPin, uint8_t txPin, uint8_t slpPin);

    void begin();
    void sendLINFrame(uint8_t id, uint8_t *data, uint8_t length);
    void processLINFrame();
    void handleButtonPress(); // New method for button press logic

    void setButtonPin(uint8_t buttonPin);
    void setLED(uint8_t ledState);
    void setLEDPin(uint8_t ledPin); // New method for setting the LED pin

    uint8_t calculateChecksum(uint8_t id, uint8_t *data, uint8_t length);
    uint8_t calculateParity(uint8_t id);

private:
    SoftwareSerial *linSerial;
    uint8_t rxPin;
    uint8_t txPin;
    uint8_t slpPin;
    uint8_t ledPin = 255; // Default to 255 (invalid pin)
    uint8_t buttonPin;
    uint8_t buttonState;     // Track button state
    uint8_t lastButtonState; // Track previous button state for debouncing
};

#endif // LIN55_H
