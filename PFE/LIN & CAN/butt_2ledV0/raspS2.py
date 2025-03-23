import serial
import RPi.GPIO as GPIO

# GPIO setup for controlling the LED
LED_PIN = 27  # Assuming you're using GPIO 17 for the LED
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# Serial setup for LIN communication
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "2"  # ID assigned to this Raspberry Pi
print("Raspberry Pi 2 (LIN Slave) Ready")

try:
    while True:
        # Check if there's data to read from the LIN bus
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received from LIN Master: {received_data}")

            # Check if the received data contains the LED control message
            if received_data == f"{SLAVE_ID}:LED_ON":
                print("Button pressed, turning LED ON")
                GPIO.output(LED_PIN, GPIO.HIGH)  # Turn LED on
            elif received_data == f"{SLAVE_ID}:LED_OFF":
                print("Button not pressed, turning LED OFF")
                GPIO.output(LED_PIN, GPIO.LOW)  # Turn LED off

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
