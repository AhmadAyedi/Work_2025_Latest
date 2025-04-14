import can
import cantools

# Load the DBC file
db = cantools.database.load_file('simple_can.dbc')

# Initialize the CAN interface
bus = can.interface.Bus(channel='can0', interface='socketcan')

print("Listening for CAN messages...")

try:
    while True:
        # Receive CAN message
        msg = bus.recv(timeout=1.0)
        if msg is None:
            continue
        # Decode message using DBC
        try:
            decoded = db.decode_message(msg.arbitration_id, msg.data)
            # Extract the 64-bit integer
            hello_int = decoded.get('HelloString', 0)
            # Convert to 8 bytes (little-endian)
            hello_bytes = hello_int.to_bytes(8, byteorder='little')
            # Convert to string, removing padding
            hello_str = hello_bytes.decode('utf-8', errors='ignore').rstrip('\x00')
            print(f"Received: {hello_str}")
        except cantools.database.errors.DecodeError:
            print(f"Unknown message ID: {hex(msg.arbitration_id)}")
        except Exception as e:
            print(f"Decode error: {e}")
except KeyboardInterrupt:
    print("Stopped by user")
except can.CanError as e:
    print(f"CAN error: {e}")
finally:
    bus.shutdown()