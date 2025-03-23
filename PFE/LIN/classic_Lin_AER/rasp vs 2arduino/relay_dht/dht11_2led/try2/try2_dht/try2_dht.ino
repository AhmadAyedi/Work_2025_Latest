#include <SoftwareSerial.h>
#include <DHT.h>

// LIN communication pins
#define TX_PIN 2  
#define RX_PIN 3  
#define SLP_PIN 4 

// DHT11 sensor configuration
#define DHT_PIN 5
#define DHTTYPE DHT11
DHT dht(DHT_PIN, DHTTYPE);

SoftwareSerial linSerial(RX_PIN, TX_PIN);

void setup() {
    pinMode(SLP_PIN, OUTPUT);
    digitalWrite(SLP_PIN, HIGH); 

    linSerial.begin(9600);
    Serial.begin(9600);

    dht.begin();

    Serial.println("Slave 1 (DHT11) Ready");
}

void loop() {
    if (linSerial.available()) {
        String frame = linSerial.readStringUntil('\n');
        frame.trim();
        Serial.println("Received Frame: " + frame);

        // Parse LIN Frame ID
        if (frame.startsWith("0x10")) { // 0x10 is the ID for requesting temp data
            float temperature = dht.readTemperature();

            if (isnan(temperature)) {
                Serial.println("Failed to read from DHT sensor!");
                return;
            }

            // Send temperature data back to the Master immediately
            String response = "temp:" + String(temperature);
            linSerial.println(response);
            Serial.println("Sent: " + response);
        }
    }
}
