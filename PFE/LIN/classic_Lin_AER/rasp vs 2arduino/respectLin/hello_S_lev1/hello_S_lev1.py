import serial
import time
import RPi.GPIO as GPIO

# GPIO setup
SLP_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(SLP_PIN, GPIO.OUT)
GPIO.output(SLP_PIN, GPIO.HIGH)  # Wake up LIN module

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Define schedule table with message IDs and corresponding data
schedule_table = [
    {"id": 3, "message": "hello arduino1"},
    {"id": 2, "message": "hello arduino2"}
]

def send_message(message, message_id):
    formatted_message = f"{message_id}:{message}\n"
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Sent: {formatted_message.strip()}")
    time.sleep(1)

try:
    print("Master Ready")
    while True:
        for entry in schedule_table:
            send_message(entry["message"], entry["id"])
            time.sleep(2)  # Schedule table period
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
