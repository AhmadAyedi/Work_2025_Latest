import time
import board
import adafruit_dht

# Définir le pin GPIO où est connecté le DHT11
dht_device = adafruit_dht.DHT11(board.D4)

try:
    while True:
        # Lire la température et l'humidité
        temperature = dht_device.temperature
        humidity = dht_device.humidity

        if temperature is not None and humidity is not None:
            print(f"Température: {temperature:.1f}°C  Humidité: {humidity:.1f}%")
        else:
            print("Erreur de lecture des données, réessayez...")

        time.sleep(2)

except KeyboardInterrupt:
    print("Programme arrêté.")
finally:
    dht_device.exit()
