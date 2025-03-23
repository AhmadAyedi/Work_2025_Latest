#include <WiFi.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN0_INT 0
#define CAN_CS_PIN 4 // Chip Select pin for MCP2515
#define LED_PIN 2    // LED pin

const char* ssid = "TOPNET_21F8";   // Change this to your WiFi SSID
const char* password = "yp5qac98w8";  // Change this to your WiFi password

WiFiServer server(80);
MCP_CAN CAN0(CAN_CS_PIN);

void setup() {
    Serial.begin(115200);
    
    // Initialize CAN bus
    while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) != CAN_OK) {
        Serial.println("Error Initializing MCP2515...");
        delay(100);
    }
    Serial.println("MCP2515 Initialized Successfully!");
    CAN0.setMode(MCP_NORMAL);

    // Initialize LED pin
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);  // Ensure LED is OFF initially

    // Connect to WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi Connected!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
    if (!digitalRead(CAN0_INT)) { // Check if data is available
        long unsigned int rxId;
        unsigned char len;
        unsigned char rxBuf[8];

        CAN0.readMsgBuf(&rxId, &len, rxBuf);

        if (rxId == 0x200) { // Ensure it's the correct CAN message
            byte buttonState = rxBuf[0];  // Button state (1 = ON, 0 = OFF)

            if (buttonState == 1) {
                digitalWrite(LED_PIN, HIGH); // Turn LED ON
                Serial.println("LED ON");
            } else {
                digitalWrite(LED_PIN, LOW); // Turn LED OFF
                Serial.println("LED OFF");
            }
        }
    }

    // Handle Web Clients (optional, can be added for monitoring via a web interface)
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        // Serve the Web Page
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<html><body><h1>ESP32 LED Control</h1><p>LED is ");
        client.println(digitalRead(LED_PIN) == HIGH ? "ON" : "OFF");
        client.println("</p></body></html>");
        client.stop();
    }
}
