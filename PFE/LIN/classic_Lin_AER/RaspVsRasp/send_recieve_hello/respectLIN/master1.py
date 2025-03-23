import serial
import time
import RPi.GPIO as GPIO

# GPIO setup
SLP_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(SLP_PIN, GPIO.OUT)
GPIO.output(SLP_PIN, GPIO.HIGH)  # Wake up LIN module

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Define the message to request temperature data
request_message = "get_temperature"

def send_request():
    """Send a request message to the slave."""
    formatted_message = f"1:{request_message}\n"  # Message ID 1 for temperature request
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Master Sent: {formatted_message.strip()}")

def receive_response():
    """Wait for and receive the slave's response."""
    if linSerial.in_waiting > 0:
        response = linSerial.readline().decode('utf-8').strip()
        print(f"Master Received: {response}")
        return response

try:
    print("Master Ready")
    while True:
        send_request()  # Send request for temperature data
        time.sleep(1)   # Give the slave time to respond
        temperature_data = receive_response()  # Get the temperature data from slave
        if temperature_data:
            print(f"Received Temperature Data: {temperature_data} C")
        time.sleep(2)  # Wait before sending the next request
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
