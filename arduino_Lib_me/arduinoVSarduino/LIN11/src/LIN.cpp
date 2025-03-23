#include <Arduino.h>

#include "LIN.h"

LIN::LIN(int rxPin, int txPin, int slpPin)
    : _linSerial(rxPin, txPin), _slpPin(slpPin) {}

void LIN::begin()
{
    pinMode(_slpPin, OUTPUT);
    digitalWrite(_slpPin, HIGH); // Wake up the LIN module

    _linSerial.begin(9600); // Start LIN communication at 9600 baud
    Serial.begin(9600);     // Start serial communication for debugging

    Serial.println("LIN Communication Started");
}

void LIN::sendFrame(const String &frame)
{
    _linSerial.println(frame);        // Send the frame over the LIN bus
    Serial.println("Sent: " + frame); // Debugging output via Serial Monitor
}

void LIN::receiveFrame()
{
    if (_linSerial.available())
    {
        String receivedData = _linSerial.readStringUntil('\n'); // Read the incoming data until newline
        Serial.println("Received: " + receivedData);            // Debugging output via Serial Monitor
    }
}
