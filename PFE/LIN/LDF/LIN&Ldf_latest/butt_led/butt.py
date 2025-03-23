import RPi.GPIO as GPIO
import time
import serial
import re

# Function to parse the LDF file
def parse_ldf(ldf_file):
    with open(ldf_file, 'r') as file:
        lines = file.readlines()

    nodes = {}
    signals = {}
    frames = {}

    current_section = None

    for line in lines:
        line = line.strip()

        # Parse the Nodes section
        if line.startswith("Nodes"):
            current_section = 'Nodes'
        elif current_section == 'Nodes' and line.startswith("Master:"):
            master_info = line.split(":")[1].strip()
            nodes['Master'] = master_info
        elif current_section == 'Nodes' and line.startswith("Slaves:"):
            slave_info = line.split(":")[1].strip()
            nodes['Slaves'] = slave_info

        # Parse the Signals section
        elif line.startswith("Signals"):
            current_section = 'Signals'
        elif current_section == 'Signals' and 'Request_Signal' in line:
            request_signal_info = re.findall(r"Request_Signal:\s*(\d+),\s*{([\d,]+)}", line)
            if request_signal_info:
                signals['Request_Signal'] = request_signal_info[0]
        elif current_section == 'Signals' and 'Response_Signal' in line:
            response_signal_info = re.findall(r"Response_Signal:\s*(\d+),\s*([\d,]+)", line)
            if response_signal_info:
                signals['Response_Signal'] = response_signal_info[0]

        # Parse the Frames section
        elif line.startswith("Frames"):
            current_section = 'Frames'
        elif current_section == 'Frames' and 'Request_Frame' in line:
            request_frame_info = re.findall(r"Request_Frame:\s*(\d+),\s*(\w+),\s*(\d+)", line)
            if request_frame_info:
                frames['Request_Frame'] = request_frame_info[0]
        elif current_section == 'Frames' and 'Response_Frame' in line:
            response_frame_info = re.findall(r"Response_Frame:\s*(\d+),\s*(\w+),\s*(\d+)", line)
            if response_frame_info:
                frames['Response_Frame'] = response_frame_info[0]

    return nodes, signals, frames

# GPIO setup
BUTTON_PIN = 24
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # Pull-up resistor

# Serial setup
try:
    ser = serial.Serial(
        '/dev/serial0',
        baudrate=19200,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=1
    )
    print("Serial port initialized.")
except Exception as e:
    print(f"Serial port initialization error: {e}")
    exit()

# Parse the LDF file
ldf_file = 'led_ldf.ldf'  # Replace with the correct path
nodes, signals, frames = parse_ldf(ldf_file)

# Print parsed LDF for debugging
print("Parsed LDF:")
print(f"Nodes: {nodes}")
print(f"Signals: {signals}")
print(f"Frames: {frames}")

# Extract the frame ID from the LDF
request_frame_id = int(frames['Request_Frame'][0])  # Assume it's in decimal

# Main loop
last_button_state = 0

try:
    while True:
        button_state = GPIO.input(BUTTON_PIN)
        print(f"Current button state: {button_state}")  # Print button state

        if button_state == GPIO.LOW and last_button_state == GPIO.HIGH:
            print("Button pressed.")
            frame = bytearray([request_frame_id])  # Send only the frame ID
            ser.write(frame)
            print(f"Sent frame (raw): {[hex(b) for b in frame]}")  # Print frame in hex
            print(f"Sent frame (decimal): {[b for b in frame]}")  # Print frame in decimal

        last_button_state = button_state
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting master...")
finally:
    GPIO.cleanup()
    ser.close()
