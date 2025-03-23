import serial
import RPi.GPIO as GPIO

YELLOW_LED = 17
GREEN_LED = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(YELLOW_LED, GPIO.OUT)
GPIO.setup(GREEN_LED, GPIO.OUT)

linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

print("Raspberry Pi 2 (LIN Slave 2) Ready")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {received_data}")

            try:
                data_id, command = received_data.split(',')

                if data_id == "2":  # Yellow LED
                    GPIO.output(YELLOW_LED, GPIO.HIGH if command == "LED_ON" else GPIO.LOW)
                elif data_id == "3":  # Green LED
                    GPIO.output(GREEN_LED, GPIO.HIGH if command == "LED_ON" else GPIO.LOW)

            except ValueError:
                print("Invalid Data Format")

except KeyboardInterrupt:
    GPIO.cleanup()
    linSerial.close()
