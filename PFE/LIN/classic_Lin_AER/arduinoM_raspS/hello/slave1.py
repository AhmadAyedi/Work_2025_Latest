import serial
import RPi.GPIO as GPIO

# GPIO setup
SLP_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(SLP_PIN, GPIO.OUT)
GPIO.output(SLP_PIN, GPIO.HIGH)  # Wake up LIN module

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "1"  # This Raspberry Pi listens for ID 1

print("Raspberry Pi Slave 1 Ready")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            if ":" in received_data:
                message_id, message = received_data.split(":", 1)
                if message_id == SLAVE_ID:
                    print(f"Received: {message}")
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
