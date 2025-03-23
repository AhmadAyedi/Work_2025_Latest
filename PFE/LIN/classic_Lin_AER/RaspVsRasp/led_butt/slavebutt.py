import serial
import RPi.GPIO as GPIO
import time

# GPIO setup
BUTTON_PIN = 2
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "3"  # Identifier for messages

try:
    print("Slave Ready")
    while True:
        button_state = GPIO.input(BUTTON_PIN)
        message = "ON" if button_state else "OFF"

        formatted_message = f"{SLAVE_ID}:{message}\n"
        linSerial.write(formatted_message.encode('utf-8'))
        print(f"Sent: {formatted_message.strip()}")
        time.sleep(0.5)  # Adjust debounce timing
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()

