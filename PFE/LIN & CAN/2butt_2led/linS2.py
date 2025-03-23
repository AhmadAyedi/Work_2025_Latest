import serial
import RPi.GPIO as GPIO

LED_PIN = 27  # Yellow LED
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

SLAVE_ID = "2"

print("Raspberry Pi 2 (LIN Slave 2) Ready")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {received_data}")

            try:
                data_id, command = received_data.split(',')
                if data_id == SLAVE_ID:
                    if command == "LED_ON":
                        print("Turning YELLOW LED ON")
                        GPIO.output(LED_PIN, GPIO.HIGH)
                    elif command == "LED_OFF":
                        print("Turning YELLOW LED OFF")
                        GPIO.output(LED_PIN, GPIO.LOW)
            except ValueError:
                print("Invalid Data Format")

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
