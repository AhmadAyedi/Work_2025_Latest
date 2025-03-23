import RPi.GPIO as GPIO
import serial

LED_PIN = 2  # GPIO 2 (D02)
TOGGLE_ID = "2"  # Slave ID for Raspberry Pi 2

GPIO.setmode(GPIO.BCM)  # Use BCM pin numbering
GPIO.setup(LED_PIN, GPIO.OUT)  # Set GPIO 2 as an output

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

led_state = False  # Start with LED off

try:
    print("Master Ready")
    while True:
        if linSerial.in_waiting > 0:
            receivedData = linSerial.readline().decode('utf-8').strip()
            print(f"Received RAW: {receivedData}")  # Debugging print

            delimiterIndex = receivedData.find(':')
            if delimiterIndex > 0:
                msg_id = receivedData[:delimiterIndex]
                message = receivedData[delimiterIndex + 1:]

                if msg_id == TOGGLE_ID and message == "TOGGLE":
                    led_state = not led_state  # Toggle LED state
                    GPIO.output(LED_PIN, GPIO.HIGH if led_state else GPIO.LOW)
                    print(f"LED {'ON' if led_state else 'OFF'}")
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
