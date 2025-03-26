#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Wemos UART Rx Started");

  Serial2.begin(115200, SERIAL_8N1, 26, -1); // RX=GPIO26, TX unused
  Serial.println("UART2 Initialized (GPIO26)");
}

void loop()
{
  if (Serial2.available())
  {
    char rxChar = Serial2.read();
    Serial.print("Rx: ");
    Serial.println(rxChar);
  }
  else
  {
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint >= 2000)
    {
      Serial.println("No data available on UART2");
      lastPrint = millis();
    }
  }
}