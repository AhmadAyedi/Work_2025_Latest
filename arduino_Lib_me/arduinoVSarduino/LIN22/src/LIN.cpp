#include <Arduino.h>
#include "LIN.h"

LIN::LIN(uint8_t txPin, uint8_t rxPin, uint8_t slpPin)
    : _linSerial(rxPin, txPin), _slpPin(slpPin) {}

void LIN::begin(long baudRate)
{
    pinMode(_slpPin, OUTPUT);
    digitalWrite(_slpPin, HIGH); // Wake up LIN module

    _linSerial.begin(baudRate);
    Serial.begin(9600); // For debugging
    Serial.println("LIN Communication Started");
}

void LIN::sendFrame(uint8_t id, uint8_t data)
{
    uint8_t checksum = ~(id + data); // Classic LIN checksum

    _linSerial.write((uint8_t)0x00); // Break field
    Serial.print("Break field: 0x00\n");

    _linSerial.write((uint8_t)0x55); // Sync byte
    Serial.print("Sync byte: 0x55\n");

    _linSerial.write(id); // Identifier
    Serial.print("ID: ");
    Serial.println(id);

    _linSerial.write(data); // Data byte
    Serial.print("Data: ");
    Serial.println(data);

    _linSerial.write(checksum); // Checksum byte
    Serial.print("Checksum: ");
    Serial.println(checksum);
}

void LIN::receiveFrame()
{
    if (_linSerial.available() >= 5)
    {                                                 // Check if at least 5 bytes are available for the frame
        uint8_t breakField = _linSerial.read();       // Read break (ignored)
        uint8_t sync = _linSerial.read();             // Read sync byte
        uint8_t id = _linSerial.read();               // Read identifier
        uint8_t data = _linSerial.read();             // Read data byte
        uint8_t receivedChecksum = _linSerial.read(); // Read checksum byte

        if (isFrameValid(sync, id, data, receivedChecksum))
        {
            Serial.print("Valid LIN Frame:\n");
            Serial.print("ID: ");
            Serial.println(id);
            Serial.print("Data: ");
            Serial.println(data);
            Serial.print("Received Checksum: ");
            Serial.println(receivedChecksum);
        }
        else
        {
            Serial.println("Invalid LIN Frame");
        }
    }
}

bool LIN::isFrameValid(uint8_t sync, uint8_t id, uint8_t data, uint8_t receivedChecksum)
{
    uint8_t calculatedChecksum = ~(id + data); // Compute checksum

    // Check if sync byte is valid and if checksum matches
    return (sync == 0x55 && receivedChecksum == calculatedChecksum);
}
