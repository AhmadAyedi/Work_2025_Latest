import RPi.GPIO as GPIO
import serial
import time
import re

# Fonction pour analyser le fichier LDF
def parse_ldf(ldf_file):
    with open(ldf_file, 'r') as file:
        lines = file.readlines()

    nodes = {}
    signals = {}
    frames = {}

    current_section = None

    for line in lines:
        line = line.strip()

        # Analyse de la section Nodes
        if line.startswith("Nodes"):
            current_section = 'Nodes'
        elif current_section == 'Nodes' and line.startswith("Master:"):
            master_info = line.split(":")[1].strip()
            nodes['Master'] = master_info
        elif current_section == 'Nodes' and line.startswith("Slaves:"):
            slave_info = line.split(":")[1].strip()
            nodes['Slaves'] = slave_info

        # Analyse de la section Signals
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

        # Analyse de la section Frames
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
LED_PIN = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

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
ldf_file = 'led_ldf.ldf'  # Remplacer avec votre chemin de fichier LDF
nodes, signals, frames = parse_ldf(ldf_file)

# Extract frame ID from the LDF
request_frame_id = int(frames['Request_Frame'][0])  # Assumer le format décimal

# LED state
led_state = False

def toggle_led(state):
    GPIO.output(LED_PIN, state)

# Main loop
try:
    while True:
        if ser.in_waiting > 0:
            frame = ser.read(2)
            frame_id, signal = frame[0], frame[1]
            print(f"Received frame: {frame_id}, {signal}")  # Debugging received frame

            if frame_id == request_frame_id and signal == 1:
                # Toggle LED
                led_state = not led_state
                toggle_led(led_state)
                print(f"LED state changed: {led_state}")

except KeyboardInterrupt:
    print("Exiting slave...")
finally:
    GPIO.cleanup()
    ser.close()
