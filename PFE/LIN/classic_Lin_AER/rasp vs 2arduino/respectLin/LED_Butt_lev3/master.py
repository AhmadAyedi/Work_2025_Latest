import serial
import time

# Initialize LIN communication
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

# LIN Protocol IDs
ID_BUTTON_PRESS = "0x01"  # Request button status from Slave 1
ID_TOGGLE_LED = "0x02"     # Command to toggle LED on Slave 2

print("Master Ready")

def send_lin_frame(lin_id, data=""):
    """Send a LIN frame with ID and optional data."""
    frame = f"{lin_id}:{data}\n"
    linSerial.write(frame.encode('utf-8'))
    print(f"Sent Frame: {frame.strip()}")

try:
    while True:
        # Continuously poll Slave 1 for button status
        send_lin_frame(ID_BUTTON_PRESS)
        time.sleep(0.1)

        # Check for response from Slave 1
        if linSerial.in_waiting > 0:
            message = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {message}")

            # If the button is pressed, send command to toggle LED on Slave 2
            if message == "button_pressed":
                print("Routing toggle command to Slave 2")
                send_lin_frame(ID_TOGGLE_LED, "toggle_led")
                time.sleep(0.1)

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
