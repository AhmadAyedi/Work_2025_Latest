#include <Arduino.h>
#include "LIN33.h"

LIN33::LIN33(uint8_t txPin, uint8_t rxPin, uint8_t slpPin)
    : _linSerial(rxPin, txPin), _slpPin(slpPin) {}

void LIN33::begin(long baudRate)
{
    pinMode(_slpPin, OUTPUT);
    digitalWrite(_slpPin, HIGH); // Wake up the LIN module
    _linSerial.begin(baudRate);
    Serial.begin(9600); // For debugging
    Serial.println("LIN Communication Started");
}

void LIN33::sendFrame(uint8_t id, uint8_t *data, uint8_t dataLength)
{
    byte checksum = calculateChecksum(data, dataLength);

    // Construct the frame: Sync byte + ID byte + Data bytes + Checksum byte
    byte frame[11];  // 1 byte for sync, 1 byte for ID, 8 bytes for data, 1 byte for checksum
    frame[0] = 0x55; // Sync byte
    frame[1] = id;   // ID byte

    // Add data to frame
    for (byte i = 0; i < dataLength; i++)
    {
        frame[2 + i] = data[i];
    }
    frame[2 + dataLength] = checksum; // Add checksum

    // Send the frame
    for (byte i = 0; i < sizeof(frame); i++)
    {
        _linSerial.write(frame[i]);
    }

    // Debugging: Print sent frame
    Serial.print("Sent frame: ");
    for (byte i = 0; i < sizeof(frame); i++)
    {
        Serial.print(frame[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void LIN33::receiveFrame()
{
    if (_linSerial.available())
    {
        String receivedData = "";

        // Read all available bytes
        while (_linSerial.available())
        {
            byte dataByte = _linSerial.read();     // Read one byte
            receivedData += String(dataByte, HEX); // Convert to HEX and append
            receivedData += " ";                   // Add a space between bytes
            delay(10);                             // Small delay to ensure all bytes are read correctly
        }

        // Print the entire frame in one line
        Serial.print("Received: ");
        Serial.println(receivedData);
    }
}

byte LIN33::calculateChecksum(uint8_t *data, uint8_t length)
{
    byte checksum = 0;
    for (byte i = 0; i < length; i++)
    {
        checksum += data[i];
    }
    return (checksum ^ 0xFF) + 1; // LIN checksum formula (negate and add 1)
}
