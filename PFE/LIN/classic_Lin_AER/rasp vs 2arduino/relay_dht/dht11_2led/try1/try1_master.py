import serial
import time

# Initialize serial communication with the LIN bus
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

# LIN Frame IDs
ID_REQUEST_TEMP = "0x10"  # Request temperature and humidity from Slave 1
ID_CONTROL_LEDS = "0x20"   # Control LEDs on Slave 2

# Temperature threshold
TEMP_THRESHOLD = 20.0

print("Master Ready")

def send_lin_frame(lin_id, data=""):
    """Send a LIN frame with ID and optional data."""
    frame = f"{lin_id}:{data}\n"
    linSerial.write(frame.encode('utf-8'))
    print(f"Sent Frame: {frame.strip()}")

try:
    while True:
        # Request temperature and humidity from Slave 1
        send_lin_frame(ID_REQUEST_TEMP)
        time.sleep(0.5)

        # Check for response from Slave 1
        if linSerial.in_waiting > 0:
            message = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {message}")

            # Parse temperature from the response (expected format: "temp:25.0")
            if message.startswith("temp:"):
                temp_value = float(message.split(":")[1])
                print(f"Temperature: {temp_value} C")

                # Send command to Slave 2 to control LEDs based on temperature
                if temp_value > TEMP_THRESHOLD:
                    print("Temperature is high! Turning on the RED LED.")
                    send_lin_frame(ID_CONTROL_LEDS, "red")
                else:
                    print("Temperature is low! Turning on the GREEN LED.")
                    send_lin_frame(ID_CONTROL_LEDS, "green")

        time.sleep(1)

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
