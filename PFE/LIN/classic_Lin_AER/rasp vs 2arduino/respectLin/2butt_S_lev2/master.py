import serial
import time

# Initialize LIN communication
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

print("Master Ready")

# Function to send LIN header with the given ID
def send_lin_header(lin_id):
    sync_break = b'\x00' * 10  # Sync Break (minimum 13-bit low, approximated here)
    sync_byte = b'\x55'         # Sync Byte (0x55 standard for LIN)
    protected_id = bytes([lin_id & 0x3F])  # Protected ID (6-bit identifier)
    
    linSerial.write(sync_break + sync_byte + protected_id)
    print(f"Sent LIN Header with ID: {lin_id}")

try:
    while True:
        # Send headers for both slaves to check if any button is pressed
        for lin_id in [0x01, 0x02]:
            send_lin_header(lin_id)
            time.sleep(0.1)  # Short delay for the slave to respond
            
            if linSerial.in_waiting > 0:
                message = linSerial.readline().decode('utf-8').strip()
                print(f"Received: {message}")

        time.sleep(0.5)  # Delay before the next polling cycle

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
