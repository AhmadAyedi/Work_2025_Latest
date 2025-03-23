#include <Arduino.h>
#include "LIN55.h"

LIN55::LIN55(uint8_t rxPin, uint8_t txPin, uint8_t slpPin)
{
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->slpPin = slpPin;
    this->linSerial = new SoftwareSerial(rxPin, txPin);
    this->buttonState = HIGH;
    this->lastButtonState = HIGH;
}

void LIN55::begin()
{
    pinMode(slpPin, OUTPUT);
    digitalWrite(slpPin, HIGH); // Wake up LIN module
    linSerial->begin(9600);
    Serial.begin(9600);

    if (ledPin != 255)
    {
        pinMode(ledPin, OUTPUT); // Set LED pin as output if it's valid
    }
}

void LIN55::setLEDPin(uint8_t ledPin)
{
    this->ledPin = ledPin;   // Set the LED pin
    pinMode(ledPin, OUTPUT); // Set the pin as output
}

uint8_t LIN55::calculateParity(uint8_t id)
{
    uint8_t p0 = (id & 0x01) ^ ((id & 0x02) >> 1) ^ ((id & 0x04) >> 2) ^ ((id & 0x10) >> 4);
    uint8_t p1 = ~(((id & 0x02) >> 1) ^ ((id & 0x08) >> 3) ^ ((id & 0x10) >> 4) ^ ((id & 0x20) >> 5));
    p1 = (p1 & 0x01) << 1;
    return id | p0 | p1;
}

uint8_t LIN55::calculateChecksum(uint8_t id, uint8_t *data, uint8_t length)
{
    uint16_t sum = id; // Start with the ID
    for (int i = 0; i < length; i++)
    {
        sum += data[i];
    }
    sum = (sum & 0xFF) + (sum >> 8);
    return ~sum;
}

void LIN55::sendLINFrame(uint8_t id, uint8_t *data, uint8_t length)
{
    id = calculateParity(id); // Calculate parity and update the id

    linSerial->write((uint8_t)0x00); // Break field
    linSerial->write((uint8_t)0x00);
    linSerial->write((uint8_t)0x00);

    linSerial->write(0x55); // Sync byte
    linSerial->write(id);   // Identifier with parity
    for (int i = 0; i < length; i++)
    {
        linSerial->write(data[i]); // Data
    }

    uint8_t checksum = calculateChecksum(id, data, length);
    linSerial->write(checksum); // Checksum
}

void LIN55::processLINFrame()
{
    if (linSerial->available() > 8) // Check if enough data is available
    {
        linSerial->read(); // Read Break field (ignore)
        linSerial->read();
        linSerial->read();

        uint8_t sync = linSerial->read();
        uint8_t id = linSerial->read();
        uint8_t data[1]; // Assuming one byte for ON/OFF
        data[0] = linSerial->read();

        uint8_t receivedChecksum = linSerial->read();

        uint8_t calculatedChecksum = calculateChecksum(id, data, 1);

        if (sync == 0x55 && receivedChecksum == calculatedChecksum)
        {
            // Debugging: Print the received data
            Serial.print("Received data: ");
            Serial.println(data[0], HEX);

            // Check command and control LED
            if (ledPin != 255)
            {
                if (data[0] == 0x01)
                {
                    digitalWrite(ledPin, HIGH); // Turn ON LED
                    Serial.println("LED ON");
                }
                else if (data[0] == 0x00)
                {
                    digitalWrite(ledPin, LOW); // Turn OFF LED
                    Serial.println("LED OFF");
                }
            }
        }
        else
        {
            Serial.println("Invalid LIN Frame");
        }
    }
}

void LIN55::setButtonPin(uint8_t buttonPin)
{
    this->buttonPin = buttonPin;
    pinMode(buttonPin, INPUT_PULLUP);
}

void LIN55::setLED(uint8_t ledState)
{
    if (ledPin != 255)
    {
        digitalWrite(ledPin, ledState);
    }
}

void LIN55::handleButtonPress()
{
    static bool ledState = true;

    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW && lastButtonState == HIGH)
    {
        ledState = !ledState;                          // Toggle the state
        uint8_t message[1] = {ledState ? 0x01 : 0x00}; // Send 0x01 for ON, 0x00 for OFF

        if (ledState)
        {
            Serial.println("Sending OFF Command");
        }
        else
        {
            Serial.println("Sending ON Command");
        }

        sendLINFrame(0x12, message, sizeof(message)); // Send the command
        delay(200);                                   // Debounce delay
    }

    lastButtonState = buttonState; // Update the button state
}
