import RPi.GPIO as GPIO
import serial
import time
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
        elif current_section == 'Signals' and 'MasterReqFrame_NewSignal' in line:
            request_signal_info = re.findall(r"MasterReqFrame_NewSignal:\s*(\d+),\s*{([\d,]+)}", line)
            if request_signal_info:
                signals['MasterReqFrame_NewSignal'] = request_signal_info[0]
        elif current_section == 'Signals' and 'SlaveRespFrame_NewSignal' in line:
            response_signal_info = re.findall(r"SlaveRespFrame_NewSignal:\s*(\d+),\s*([\d,]+)", line)
            if response_signal_info:
                signals['SlaveRespFrame_NewSignal'] = response_signal_info[0]

        # Parse the Frames section
        elif line.startswith("Frames"):
            current_section = 'Frames'
        elif current_section == 'Frames' and 'MasterReqFrame' in line:
            request_frame_info = re.findall(r"MasterReqFrame:\s*(\d+),\s*(\w+),\s*(\d+)", line)
            if request_frame_info:
                frames['MasterReqFrame'] = request_frame_info[0]
        elif current_section == 'Frames' and 'SlaveRespFrame' in line:
            response_frame_info = re.findall(r"SlaveRespFrame:\s*(\d+),\s*(\w+),\s*(\d+)", line)
            if response_frame_info:
                frames['SlaveRespFrame'] = response_frame_info[0]

    return nodes, signals, frames

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
ldf_file = 'lin.ldf'  # Replace with your LDF file path
nodes, signals, frames = parse_ldf(ldf_file)

# Extract frame ID from the LDF
request_frame_id = int(frames['MasterReqFrame'][0])  # Assume it's in decimal
response_frame_id = int(frames['SlaveRespFrame'][0])  # Assume it's in decimal

# Main loop to listen for master request and respond
try:
    while True:
        if ser.in_waiting > 0:
            frame = ser.read(2)  # Read request frame
            frame_id, signal = frame[0], frame[1]
            if frame_id == request_frame_id:
                print(f"Received request frame: {frame}")
                # Respond with the "hello master i am right here" message, truncated to 8 bytes
                response_message = "hello m"
                response = bytearray([response_frame_id] + list(response_message.encode('utf-8'))[:7])
                response = response.ljust(8, b'\x00')  # Ensure the response is 8 bytes long
                ser.write(response)
                print(f"Sent response: {response}")

        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting slave...")
finally:
    ser.close()
