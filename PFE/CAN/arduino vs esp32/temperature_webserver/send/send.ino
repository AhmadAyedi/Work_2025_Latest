#include <mcp_can.h>
#include <SPI.h>
#include <DHT.h>

#define DHTPIN 3        // Pin where the DHT11 is connected
#define DHTTYPE DHT11   // Define sensor type
#define CAN_CS_PIN 10   // Chip Select pin for MCP2515

DHT dht(DHTPIN, DHTTYPE);
MCP_CAN CAN0(CAN_CS_PIN);

void setup() {
    Serial.begin(115200);
    dht.begin();

    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_16MHZ) == CAN_OK) {
        Serial.println("MCP2515 Initialized Successfully!");
    } else {
        Serial.println("Error Initializing MCP2515...");
    }

    CAN0.setMode(MCP_NORMAL);
}

void loop() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Convert float values to integers for transmission (scaled by 100 for precision)
    int tempInt = temperature * 100;
    int humInt = humidity * 100;

    // Prepare data packet (4 bytes for temp, 4 bytes for humidity)
    byte data[8];
    data[0] = (tempInt >> 8) & 0xFF;
    data[1] = tempInt & 0xFF;
    data[2] = (humInt >> 8) & 0xFF;
    data[3] = humInt & 0xFF;

    // Send data via CAN bus
    byte sndStat = CAN0.sendMsgBuf(0x200, 0, 4, data);
    if (sndStat == CAN_OK) {
        Serial.println("Temperature & Humidity Sent Successfully!");
    } else {
        Serial.println("Error Sending Data...");
    }

    delay(2000); // Send data every 2 seconds
}
