#include <Arduino.h>

void setup()
{
  Serial2.begin(115200); // UART2 RX on GPIO 16
  delay(1000);
  Serial.begin(115200);
  Serial.println("Wemos Lolin32 Receiver Started");
}

void loop()
{
  if (Serial2.available())
  {
    String received = Serial2.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(received);
  }
}