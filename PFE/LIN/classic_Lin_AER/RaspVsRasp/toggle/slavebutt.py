import RPi.GPIO as GPIO
import serial
import time

BUTTON_PIN = 2  # GPIO 2 (D02)
SLAVE_ID = "2"  # Slave ID for this Raspberry Pi

# Set up GPIO and Serial
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # Button with pull-down resistor
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

last_state = False  # To remember the last state of the button

try:
    print("Slave Ready, waiting for button press...")
    while True:
        button_state = GPIO.input(BUTTON_PIN)

        # Check for button press (falling edge detection)
        if button_state == GPIO.HIGH and not last_state:
            print("Button Pressed!")
            linSerial.write(f"{SLAVE_ID}:TOGGLE\n".encode('utf-8'))  # Send toggle command to master
            last_state = True
        elif button_state == GPIO.LOW and last_state:
            last_state = False

        time.sleep(0.1)  # Debounce delay
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
