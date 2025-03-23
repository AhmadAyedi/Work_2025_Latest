#include <Arduino.h>

#define TX_PIN 10 // UART1 TX on GPIO 10
#define RX_PIN 9  // UART1 RX on GPIO 9 (not used, but specified)

void setup()
{
  // Initialize UART1 instead of UART2
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  delay(1000);
  Serial.begin(115200);
  Serial.println("ESP32-Pico Sender Started");
}

void loop()
{
  Serial1.println("hello");
  Serial.println("Sent: hello");
  delay(2000);
}