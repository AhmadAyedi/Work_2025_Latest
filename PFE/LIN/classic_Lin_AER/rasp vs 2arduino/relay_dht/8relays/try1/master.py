import serial
import time
from colorama import Fore, init

# Initialize colorama
init(autoreset=True)

# Initialize LIN communication (assuming connected via serial)
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

print("Master Ready")

# Relay schedule: (Relay X: time in seconds)
schedule_slave1 = [1, 2, 3, 4]  # Relay 1, 2, 3, 4 for Slave 1 (Arduino 1)
schedule_slave2 = [1, 1, 1, 3]  # Relay 1, 2, 3, 4 for Slave 2 (Arduino 2)

try:
    while True:
        # Cycle through the schedule for Slave 1 (Arduino 1)
        for i in range(4):
            relay = i + 1
            duration = schedule_slave1[i]
            linSerial.write(f"relay_{relay}_on:{duration}\n".encode())
            print(Fore.YELLOW + f"Slave 1: Relay {relay} ON for {duration}s")
            time.sleep(duration)
            linSerial.write(f"relay_{relay}_off\n".encode())
            print(Fore.RED + f"Slave 1: Relay {relay} OFF")
            time.sleep(0.5)

        # Cycle through the schedule for Slave 2 (Arduino 2)
        for i in range(4):
            relay = i + 1
            duration = schedule_slave2[i]
            linSerial.write(f"relay_{relay}_on:{duration}\n".encode())
            print(Fore.YELLOW + f"Slave 2: Relay {relay} ON for {duration}s")
            time.sleep(duration)
            linSerial.write(f"relay_{relay}_off\n".encode())
            print(Fore.RED + f"Slave 2: Relay {relay} OFF")
            time.sleep(0.5)

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
