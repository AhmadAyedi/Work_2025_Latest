import serial
import RPi.GPIO as GPIO

# GPIO setup for controlling the Green LED
LED_PIN = 27  # Green LED on GPIO 27
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# Serial setup for LIN communication
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Define the Slave ID
SLAVE_ID = "1"

print("Raspberry Pi 1 (LIN Slave 1) Ready")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {received_data}")

            # Extract ID and command
            try:
                data_id, command = received_data.split(',')
                if data_id == SLAVE_ID:  # Process only if ID matches
                    if command == "LED_ON":
                        print("Turning GREEN LED ON")
                        GPIO.output(LED_PIN, GPIO.HIGH)
                    elif command == "LED_OFF":
                        print("Turning GREEN LED OFF")
                        GPIO.output(LED_PIN, GPIO.LOW)
            except ValueError:
                print("Invalid Data Format")

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
