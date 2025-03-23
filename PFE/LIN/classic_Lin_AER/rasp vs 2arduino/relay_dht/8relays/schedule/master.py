import serial
import time
from colorama import Fore, init

# Initialize colorama
init(autoreset=True)

# Initialize LIN communication (assuming connected via serial)
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

# Define the schedule table (relay_id, activation_time_in_sec, loops)
# Format: (relay_id, activation_time_in_sec, loops)
schedule_slave1 = [
    (1, 1, 5),  # Relay 1, activate for 1 second, repeat 5 times
    (2, 1, 5),  # Relay 2, activate for 1 second, repeat 5 times
    (3, 1, 5),  # Relay 3, activate for 1 second, repeat 5 times
    (4, 1, 5),  # Relay 4, activate for 1 second, repeat 5 times
]

schedule_slave2 = [
    (5, 1, 5),  # Relay 5, activate for 1 second, repeat 5 times
    (6, 1, 5),  # Relay 6, activate for 1 second, repeat 5 times
    (7, 1, 5),  # Relay 7, activate for 1 second, repeat 5 times
    (8, 1, 5),  # Relay 8, activate for 1 second, repeat 5 times
]

# Function to send relay control commands to a slave
def send_relay_command(slave, relay, action):
    linSerial.write(f"slave{slave}_relay_{relay}_{action}\n".encode())
    print(Fore.YELLOW + f"Slave {slave}: Relay {relay} {action.upper()}")

# Main loop to send commands from the schedule table
print("Master Ready")

try:
    while True:
        # Cycle through Slave 1 relays
        for relay, duration, loops in schedule_slave1:
            for _ in range(loops):
                send_relay_command(1, relay, "on")
                time.sleep(duration)
                send_relay_command(1, relay, "off")
                time.sleep(0.2)  # Short delay before next relay in Slave 1

        # Cycle through Slave 2 relays
        for relay, duration, loops in schedule_slave2:
            for _ in range(loops):
                send_relay_command(2, relay, "on")
                time.sleep(duration)
                send_relay_command(2, relay, "off")
                time.sleep(0.2)  # Short delay before next relay in Slave 2

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
