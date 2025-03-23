#include <SoftwareSerial.h>

#define LIN_BAUDRATE 19200   // LIN baud rate
#define HEADER 0x11          // Predefined header (from LDF file)
#define SLAVE_ID 1           // Slave ID (from LDF file)
#define REQUEST_FRAME_ID 0x10  // Request frame ID (from LDF file)
#define RESPONSE_FRAME_ID 0x20 // Response frame ID (from LDF file)

SoftwareSerial slaveSerial(3, 2); // RX, TX (or use Serial if directly connected)

void setup() {
  slaveSerial.begin(LIN_BAUDRATE);
  Serial.begin(LIN_BAUDRATE);
  Serial.println("Slave ready");
}

void loop() {
  // Check if data is available to read
  if (slaveSerial.available() > 0) {
    byte receivedByte = slaveSerial.read();  // Read the incoming byte
    
    // Print the received byte for debugging
    Serial.print("Reçu (byte): ");
    Serial.println(receivedByte, HEX);

    // If the received byte is the expected request frame ID
    if (receivedByte == REQUEST_FRAME_ID) {
      byte id = slaveSerial.read();  // Read the slave ID

      // Print received ID for debugging
      Serial.print("ID reçu : ");
      Serial.println(id, HEX);

      // If the received slave ID matches the defined SLAVE_ID
      if (id == SLAVE_ID) {
        Serial.println("Slave ready to send response...");
        send_response();
      } else {
        Serial.println("ID mismatch. Slave ID is not correct.");
      }
    } else {
      Serial.println("Frame ID incorrect!");
    }
  }
}
// Function to send a response with the message "Hello"
void send_response() {
  // Le message à envoyer
  String responseMessage = "Hello";

  // Envoyer le message via le port série
  slaveSerial.print(responseMessage);

  // Imprimer le message pour le débogage
  Serial.print("Message envoyé : ");
  Serial.println(responseMessage);
}

// Function to send a response based on the frame structure from LDF
// void send_response() {
//   byte responseMessage[2] = {RESPONSE_FRAME_ID, SLAVE_ID};  // Frame ID and Slave ID

//   // Send the response message
//   slaveSerial.write(responseMessage, sizeof(responseMessage));

//   // Print the response for debugging
//   Serial.print("Message envoyé : ");
//   for (int i = 0; i < sizeof(responseMessage); i++) {
//     Serial.print(responseMessage[i], HEX);
//     Serial.print(" ");
//   }
//   Serial.println();
// }