import serial
import RPi.GPIO as GPIO

LED_PIN = 27  # Red LED
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "1"

print("Raspberry Pi 1 (LIN Slave 1) Ready")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {received_data}")

            try:
                data_id, command = received_data.split(',')
                if data_id == SLAVE_ID:
                    GPIO.output(LED_PIN, GPIO.HIGH if command == "LED_ON" else GPIO.LOW)
            except ValueError:
                print("Invalid Data Format")

except KeyboardInterrupt:
    GPIO.cleanup()
    linSerial.close()
