// #include <SPI.h>
// #include <mcp_can.h>

// #define CAN0_INT 2  // Interrupt pin
// #define CAN_CS_PIN 4 // Chip Select pin for MCP2515

// MCP_CAN CAN0(CAN_CS_PIN);

// void setup() {
//     Serial.begin(115200);

//     while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) != CAN_OK) {
//         Serial.println("Error Initializing MCP2515...");
//         delay(100);
//     }

//     Serial.println("MCP2515 Initialized Successfully!");
//     CAN0.setMode(MCP_NORMAL);
//     pinMode(CAN0_INT, INPUT);
// }

// void loop() {
//     if (!digitalRead(CAN0_INT)) { // Check if data is available
//         long unsigned int rxId;
//         unsigned char len;
//         unsigned char rxBuf[8];

//         CAN0.readMsgBuf(&rxId, &len, rxBuf);

//         if (rxId == 0x200) { // Ensure we are receiving the correct message
//             int tempInt = (rxBuf[0] << 8) | rxBuf[1];
//             int humInt = (rxBuf[2] << 8) | rxBuf[3];

//             float temperature = tempInt / 100.0;
//             float humidity = humInt / 100.0;

//             Serial.print("Received Temperature: ");
//             Serial.print(temperature);
//             Serial.println("°C");

//             Serial.print("Received Humidity: ");
//             Serial.print(humidity);
//             Serial.println("%");
//         }
//     }
// }



#include <WiFi.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN0_INT 2  // Interrupt pin
#define CAN_CS_PIN 4 // Chip Select pin for MCP2515

const char* ssid = "TOPNET_21F8";   // Change this to your WiFi SSID
const char* password = "yp5qac98w8";  // Change this to your WiFi password

WiFiServer server(80);
MCP_CAN CAN0(CAN_CS_PIN);

float temperature = 0.0;
float humidity = 0.0;

void setup() {
    Serial.begin(115200);

    // Initialize MCP2515 CAN Bus Module
    while (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) != CAN_OK) {
        Serial.println("Error Initializing MCP2515...");
        delay(100);
    }
    Serial.println("MCP2515 Initialized Successfully!");
    CAN0.setMode(MCP_NORMAL);
    pinMode(CAN0_INT, INPUT);

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
            int tempInt = (rxBuf[0] << 8) | rxBuf[1];
            int humInt = (rxBuf[2] << 8) | rxBuf[3];

            temperature = tempInt / 100.0;
            humidity = humInt / 100.0;

            Serial.print("Received Temperature: ");
            Serial.print(temperature);
            Serial.println("°C");

            Serial.print("Received Humidity: ");
            Serial.print(humidity);
            Serial.println("%");
        }
    }
    // Web Page Content
const char webPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Temperature & Humidity</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f4; }
        h1 { color: #333; }
        .container { width: 50%; margin: auto; padding: 20px; background: white; border-radius: 10px; box-shadow: 0px 0px 10px #ccc; }
        .data-box { font-size: 24px; font-weight: bold; margin-top: 20px; }
        .temp { color: #FF5733; }
        .hum { color: #3498db; }
        .refresh { margin-top: 20px; padding: 10px 20px; font-size: 18px; background: #28a745; color: white; border: none; cursor: pointer; }
        .refresh:hover { background: #218838; }
    </style>
    <script>
        function fetchData() {
            fetch("/data")
                .then(response => response.json())
                .then(data => {
                    document.getElementById("temperature").innerText = data.temperature.toFixed(2) + " °C";
                    document.getElementById("humidity").innerText = data.humidity.toFixed(2) + " %";
                })
                .catch(error => console.log("Error fetching data:", error));
        }
        setInterval(fetchData, 2000); // Auto-refresh every 2 seconds
    </script>
</head>
<body>
    <h1>ESP32 Temperature & Humidity Monitor</h1>
    <div class="container">
        <div class="data-box temp">Temperature: <span id="temperature">-- °C</span></div>
        <div class="data-box hum">Humidity: <span id="humidity">-- %</span></div>
        <button class="refresh" onclick="fetchData()">Refresh Now</button>
    </div>
</body>
</html>
)rawliteral";

    // Handle Web Clients
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client Connected!");
        String request = client.readStringUntil('\r');
        client.flush();

        // Serve the Web Page
        if (request.indexOf("/data") != -1) {
            // Send JSON Response for AJAX
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: application/json");
            client.println("Connection: close");
            client.println();
            client.printf("{\"temperature\": %.2f, \"humidity\": %.2f}", temperature, humidity);
        } else {
            // Serve the HTML Page
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println(webPage); // Send the HTML, CSS, JS content
        }

        client.stop();
        Serial.println("Client Disconnected.");
    }
}


