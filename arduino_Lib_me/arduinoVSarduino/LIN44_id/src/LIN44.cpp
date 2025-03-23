#include <Arduino.h>
#include "LIN44.h"

LIN44::LIN44(uint8_t rxPin, uint8_t txPin, uint8_t slpPin)
    : linSerial(rxPin, txPin), slpPin(slpPin) {}

void LIN44::begin()
{
    pinMode(slpPin, OUTPUT);
    digitalWrite(slpPin, HIGH); // Wake up LIN module

    linSerial.begin(9600); // LIN baud rate
    Serial.begin(9600);    // Debugging

    Serial.println("LIN44 Initialized");
}

uint8_t LIN44::calculateParity(uint8_t id)
{
    uint8_t p0 = (id & 0x01) ^ ((id & 0x02) >> 1) ^ ((id & 0x04) >> 2) ^ ((id & 0x10) >> 4);
    uint8_t p1 = ~(((id & 0x02) >> 1) ^ ((id & 0x08) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
    p1 = (p1 & 0x01) << 1;
    return id | p0 | p1;
}

uint8_t LIN44::calculateChecksum(uint8_t id, uint8_t *data, uint8_t length)
{
    uint16_t sum = id; // Start with the ID

    // Add each data byte
    for (int i = 0; i < length; i++)
    {
        sum += data[i];
    }

    // Add carry using bitwise operations
    sum = (sum & 0xFF) + (sum >> 8); // This simulates the "carry" by adding the upper byte to the lower byte.

    // Invert all bits (bitwise NOT)
    return ~sum;
}

void LIN44::sendFrame(uint8_t id, uint8_t *data, uint8_t length)
{
    id = calculateParity(id);

    // Send Break field (simulate with three zeros)
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);
    linSerial.write((uint8_t)0x00);
    Serial.println("Break field: 0x00");

    linSerial.write((uint8_t)0x55); // Sync byte
    Serial.println("Sync field: 0x55");

    linSerial.write(id); // Identifier with parity bits
    Serial.print("ID (with parity): ");
    Serial.println(id, HEX);

    // Send Data bytes
    for (int i = 0; i < length; i++)
    {
        linSerial.write(data[i]);
        Serial.print("Data: ");
        Serial.println(data[i], HEX);
    }

    // Calculate and send checksum
    uint8_t checksum = calculateChecksum(id, data, length);
    linSerial.write(checksum);
    Serial.print("Checksum: ");
    Serial.println(checksum, HEX);

    Serial.println("------------------------------");
}

void LIN44::receiveFrame()
{
    if (linSerial.available() > 8)
    { // Check if frame size is at least 8 bytes
        // Read Break field (ignore)
        linSerial.read();
        linSerial.read();
        linSerial.read();

        // Read remaining LIN frame
        uint8_t sync = linSerial.read();
        uint8_t id = linSerial.read();
        uint8_t data[5];

        for (int i = 0; i < 5; i++)
        {
            data[i] = linSerial.read();
            Serial.print("Data Received: ");
            Serial.println(data[i], HEX);
        }

        uint8_t receivedChecksum = linSerial.read();
        Serial.print("Received Checksum: ");
        Serial.println(receivedChecksum, HEX);

        // Calculate checksum on receiver side
        uint8_t calculatedChecksum = calculateChecksum(id, data, 5);
        Serial.print("Calculated Checksum: ");
        Serial.println(calculatedChecksum, HEX);

        // Compare checksums
        if (sync == 0x55 && receivedChecksum == calculatedChecksum)
        {
            Serial.print("LIN Frame Successfully Received: ");
            for (int i = 0; i < 5; i++)
            {
                Serial.print((char)data[i]);
            }
            Serial.println();
        }
        else
        {
            Serial.println("Invalid LIN Frame");
        }

        Serial.println("------------------------------");
    }
}
