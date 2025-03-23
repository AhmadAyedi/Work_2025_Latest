
import serial
import RPi.GPIO as GPIO

# GPIO setup for controlling the LED
LED_PIN = 27  # Assuming you're using GPIO 17 for the yellow LED
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

            # Check if the received data contains the LED control message for yellow LED
            if received_data == f"{SLAVE_ID}:LED_YELLOW_ON":
                print("Button pressed, turning YELLOW LED ON")
                GPIO.output(LED_PIN, GPIO.HIGH)  # Turn yellow LED on
            elif received_data == f"{SLAVE_ID}:LED_YELLOW_OFF":
                print("Button not pressed, turning YELLOW LED OFF")
                GPIO.output(LED_PIN, GPIO.LOW)  # Turn yellow LED off

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
