void setup() {
  // Initialize hardware serial at 9600 baud
  Serial.begin(9600);
}

void loop() {
  // Send "Hello" message
  Serial.println("Hello");
  delay(1000); // Wait 1 second between messages
}

// STM will reecive on PA10 = D2 usart1 Rx