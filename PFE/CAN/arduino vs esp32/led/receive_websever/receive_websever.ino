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

int ledState = LOW; // Variable to track LED state (initially OFF)

// Web page content
const char webPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 LED Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            background-color: #f0f0f0;
            text-align: center;
            margin: 0;
            padding: 0;
        }
        h1 {
            color: #333;
            margin-top: 50px;
        }
        .container {
            width: 80%;
            max-width: 600px;
            margin: 30px auto;
            background-color: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 20px rgba(0, 0, 0, 0.1);
        }
        .status {
            font-size: 24px;
            font-weight: bold;
            margin: 20px 0;
        }
        .on {
            color: #28a745;
        }
        .off {
            color: #dc3545;
        }
        .btn {
            padding: 15px 25px;
            font-size: 20px;
            background-color: #007bff;
            color: white;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        .btn:hover {
            background-color: #0056b3;
        }
    </style>
    <script>
        function updateLEDStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    const ledStatus = document.getElementById("ledStatus");
                    const btn = document.getElementById("toggleButton");

                    if (data.ledState == 1) {
                        ledStatus.textContent = 'LED is ON';
                        ledStatus.classList.add('on');
                        ledStatus.classList.remove('off');
                        btn.textContent = 'Turn OFF';
                    } else {
                        ledStatus.textContent = 'LED is OFF';
                        ledStatus.classList.add('off');
                        ledStatus.classList.remove('on');
                        btn.textContent = 'Turn ON';
                    }
                });
        }

        function toggleLED() {
            fetch('/toggle')
                .then(response => response.json())
                .then(data => {
                    updateLEDStatus();
                });
        }

        window.onload = function() {
            updateLEDStatus();
        }
    </script>
</head>
<body>
    <h1>ESP32 LED Control</h1>
    <div class="container">
        <p class="status" id="ledStatus">Loading...</p>
        <button class="btn" id="toggleButton" onclick="toggleLED()">Loading...</button>
    </div>
</body>
</html>
)rawliteral";

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
    // Handle CAN messages
    if (!digitalRead(CAN0_INT)) { // Check if data is available
        long unsigned int rxId;
        unsigned char len;
        unsigned char rxBuf[8];

        CAN0.readMsgBuf(&rxId, &len, rxBuf);

        if (rxId == 0x200) { // Ensure it's the correct CAN message
            byte buttonState = rxBuf[0];  // Button state (1 = ON, 0 = OFF)

            if (buttonState == 1) {
                digitalWrite(LED_PIN, HIGH); // Turn LED ON
                ledState = HIGH;
                Serial.println("LED ON");
            } else {
                digitalWrite(LED_PIN, LOW); // Turn LED OFF
                ledState = LOW;
                Serial.println("LED OFF");
            }
        }
    }

    // Handle Web Clients
    WiFiClient client = server.available();
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        // Serve the Web Page
        if (request.indexOf("/status") != -1) {
            // Return JSON data for the LED state
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.printf("{\"ledState\": %d}", ledState);
        } 
        else if (request.indexOf("/toggle") != -1) {
            // Toggle LED state
            ledState = (ledState == LOW) ? HIGH : LOW;
            digitalWrite(LED_PIN, ledState);

            // Return JSON response after toggling
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.printf("{\"ledState\": %d}", ledState);
        } 
        else {
            // Serve the HTML page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println(webPage); // Send the HTML content
        }

        client.stop();
    }
}
