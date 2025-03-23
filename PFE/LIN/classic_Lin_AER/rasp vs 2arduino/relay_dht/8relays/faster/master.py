import serial
import time
from colorama import Fore, init

# Initialize colorama
init(autoreset=True)

# Initialize LIN communication (assuming connected via serial)
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

print("Master Ready")

try:
    while True:
        # Cycle through Slave 1 relays (Relays 1, 2, 3, 4) with 1 second timing
        for relay in range(1, 5):
            linSerial.write(f"relay_{relay}_on\n".encode())
            print(Fore.YELLOW + f"Slave 1: Relay {relay} ON")
            time.sleep(1)  # Wait for 1 second between each relay
            linSerial.write(f"relay_{relay}_off\n".encode())
            print(Fore.RED + f"Slave 1: Relay {relay} OFF")
            time.sleep(0.2)  # Short delay between relays

        # Cycle through Slave 2 relays (Relays 5, 6, 7, 8) with 1 second timing
        for relay in range(5, 9):
            linSerial.write(f"relay_{relay}_on\n".encode())
            print(Fore.YELLOW + f"Slave 2: Relay {relay} ON")
            time.sleep(1)  # Wait for 1 second between each relay
            linSerial.write(f"relay_{relay}_off\n".encode())
            print(Fore.RED + f"Slave 2: Relay {relay} OFF")
            time.sleep(0.2)  # Short delay between relays

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
