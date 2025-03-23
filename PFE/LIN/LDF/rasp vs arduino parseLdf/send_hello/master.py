import serial
import time
from ldfparser import parseLDF
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

        # Parse Nodes Section
        if line.startswith("Nodes"):
            current_section = 'Nodes'
        elif current_section == 'Nodes' and line.startswith("Master:"):
            master_info = line.split(":")[1].strip()
            nodes['Master'] = master_info
        elif current_section == 'Nodes' and line.startswith("Slaves:"):
            slave_info = line.split(":")[1].strip()
            nodes['Slaves'] = slave_info

        # Parse Signals Section
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

        # Parse Frames Section
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

# Initialisation de la communication série
try:
    master = serial.Serial(
        '/dev/serial0',  # Remplacez par le port série correct
        baudrate=19200,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=1  # Timeout pour éviter les blocages
    )
    print("Port série initialisé avec succès.")
except Exception as e:
    print(f"Erreur lors de l'initialisation du port série : {e}")
    exit()

time.sleep(2)  # Attente pour stabiliser la connexion série

# Parsing the LDF file to get node and frame data
ldf_file = 'ldfile.ldf'  # Replace with your LDF file path
nodes, signals, frames = parse_ldf(ldf_file)

# Extract the Request_Frame ID and Response_Frame ID from parsed frames
request_frame_id = int(frames['Request_Frame'][0], 16)
response_frame_id = int(frames['Response_Frame'][0], 16)

# ID du slave cible
slave_id = 1

# Fonction pour envoyer une requête
def send_request(slave_id):
    try:
        # Format du message : [HEADER][ID]
        header = bytes([request_frame_id])  # Frame ID from LDF
        message = header + bytes([slave_id])  # Sending the slave ID as part of the message
        master.write(message)
        print(f"Requête envoyée au slave ID {slave_id}")
    except Exception as e:
        print(f"Erreur lors de l'envoi de la requête : {e}")

# Fonction pour recevoir une réponse
def receive_response():
    try:
        response = master.readline()  # Read raw data
        
        if response:
            print(f"Réponse brute reçue: {response}")  # Print raw response for debugging
            
            # Iterate over the response bytes
            for byte in response:
                print(f"Byte reçu : {hex(byte)}")
            
            # Decode the response manually if necessary
            # Check if response matches the expected length and structure
            if len(response) >= 4:
                request_frame_id = response[0]
                slave_id = response[1]
                response_frame_id = response[2]
                additional_data = response[3:]

                print(f"Request Frame ID: {hex(request_frame_id)}")
                print(f"Slave ID: {hex(slave_id)}")
                print(f"Response Frame ID: {hex(response_frame_id)}")
                print(f"Additional Data: {additional_data}")
            else:
                print("Réponse reçue trop courte!")
        else:
            print("Aucune réponse reçue.")
    except Exception as e:
        print(f"Erreur lors de la réception de la réponse : {e}")

# Boucle principale
while True:
    send_request(slave_id)  # Envoyer une requête
    time.sleep(0.1)          # Adding delay before reading the response
    receive_response()      # Recevoir une réponse
    time.sleep(2)           # Attente avant la prochaine requête
