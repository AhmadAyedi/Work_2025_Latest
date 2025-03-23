import serial
import RPi.GPIO as GPIO

# GPIO setup
LED_PIN = 2
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

try:
    print("Master Ready")
    while True:
        if linSerial.in_waiting > 0:
            receivedData = linSerial.readline().decode('utf-8').strip()
            delimiterIndex = receivedData.find(':')
            
            if delimiterIndex > 0:
                msg_id = receivedData[:delimiterIndex]
                message = receivedData[delimiterIndex + 1:]

                if msg_id == "3":  # Check if the message is from the button slave
                    if message == "ON":
                        GPIO.output(LED_PIN, GPIO.HIGH)
                        print("LED ON")
                    else:
                        GPIO.output(LED_PIN, GPIO.LOW)
                        print("LED OFF")

except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
    