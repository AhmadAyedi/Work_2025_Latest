import RPi.GPIO as GPIO
import time
import serial
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
BUTTON_PIN = 24
GPIO.setmode(GPIO.BCM)
GPIO.setup(BUTTON_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # Résistance pull-up

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

# Analyser le fichier LDF
ldf_file = 'led_ldf.ldf'  # Remplacer par le chemin correct
nodes, signals, frames = parse_ldf(ldf_file)

# Extraire l'ID du frame à partir du LDF
request_frame_id = int(frames['Request_Frame'][0])  # Supposer que c'est en format décimal

# Main loop
last_button_state = 0

try:
    while True:
        button_state = GPIO.input(BUTTON_PIN)
        print(f"Current button state: {button_state}")  # Afficher l'état du bouton

        if button_state == GPIO.HIGH and last_button_state == GPIO.LOW:
            print("Button pressed.")
            frame = bytearray([request_frame_id, 0x01])  # Utilisation de l'ID du frame du LDF
            ser.write(frame)
            print(f"Sent frame: {frame}")

        last_button_state = button_state
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting master...")
finally:
    GPIO.cleanup()
    ser.close()
