#include <LIN55.h>

LIN55 linMaster(3, 2, 4); // RX, TX, SLP pin
uint8_t buttonPin = 5;

void setup()
{
    linMaster.begin();
    linMaster.setButtonPin(buttonPin); // Set button pin
}

void loop()
{
    linMaster.handleButtonPress(); // Handle button press and send the LIN frame
    delay(10);                     // Small delay for processing
}
