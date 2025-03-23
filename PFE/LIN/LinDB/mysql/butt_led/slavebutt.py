import serial
import RPi.GPIO as GPIO
import time

# GPIO setup
BUTTON_PIN = 2
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # Button with pull-up resistor

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Function to check button press
def check_button():
    if GPIO.input(BUTTON_PIN) == GPIO.LOW:
        linSerial.write(b"TOGGLE\n")  # Send "TOGGLE" command to Master
        print("Button Pressed - TOGGLE Sent")
        time.sleep(0.5)  # Debounce delay

try:
    print("Slave Ready")
    while True:
        check_button()
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
