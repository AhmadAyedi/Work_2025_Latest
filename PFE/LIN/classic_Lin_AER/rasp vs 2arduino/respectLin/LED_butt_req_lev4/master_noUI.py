import serial
import time
from colorama import Fore, Style, init

# Initialize colorama for colored terminal output
init(autoreset=True)

# Initialize LIN communication
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

print(Fore.GREEN + Style.BRIGHT + "Master Ready\n")

try:
    while True:
        # Send request to Slave 1
        linSerial.write(b"request_button_press\n")
        print(Fore.CYAN + "Request sent to Slave 1: Press the button to change LED state")
        time.sleep(1)

        # Check for incoming messages
        if linSerial.in_waiting > 0:
            message = linSerial.readline().decode('utf-8').strip()
            print(Fore.YELLOW + f"Received: {message}")

            # If the button was pressed
            if message == "button_pressed":
                print(Fore.GREEN + Style.BRIGHT + "Button pressed! Sending toggle command to Slave 2")
                linSerial.write(b"toggle_led\n")
                time.sleep(0.1)

except KeyboardInterrupt:
    print(Fore.RED + "\nExiting program")
    linSerial.close()
