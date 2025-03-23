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

# Define commands with IDs
commands = [
    {"id": 1, "command": "stop"},         # Stop all patterns
    {"id": 2, "command": "blink"},        # Start blinking on Slave 1
    {"id": 3, "command": "fade"}          # Start fading on Slave 2
]

def send_command(command, command_id):
    formatted_message = f"{command_id}:{command}\n"
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Sent: {formatted_message.strip()}")
    time.sleep(1)

try:
    print("Master Ready")
    while True:
        for cmd in commands:
            send_command(cmd["command"], cmd["id"])
            time.sleep(3)  # Wait before sending the next command
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
