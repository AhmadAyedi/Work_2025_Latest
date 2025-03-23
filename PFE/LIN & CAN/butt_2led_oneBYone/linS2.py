import serial
import RPi.GPIO as GPIO

# GPIO setup for controlling the LED
LED_PIN = 27  # Assuming you're using GPIO 27 for the green LED
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# Serial setup for LIN communication
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "1"  # ID assigned to this Raspberry Pi
print("Raspberry Pi 1 (LIN Slave) Ready")

try:
    while True:
        # Check if there's data to read from the LIN bus
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received from LIN Master: {received_data}")

            # Check if the received data contains the LED control message for green LED
            if received_data == f"{SLAVE_ID}:LED_GREEN_ON":
                print("Button pressed, turning GREEN LED ON")
                GPIO.output(LED_PIN, GPIO.HIGH)  # Turn green LED on
            elif received_data == f"{SLAVE_ID}:LED_GREEN_OFF":
                print("Button not pressed, turning GREEN LED OFF")
                GPIO.output(LED_PIN, GPIO.LOW)  # Turn green LED off

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()