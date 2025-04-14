import can
import cantools
import time

# Load the DBC file to get message details
db = cantools.database.load_file('simple_can.dbc')

# Initialize the CAN interface
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Message details
message_name = 'HelloMessage'
message = db.get_message_by_name(message_name)

# Prepare the string "hello" as 8-byte data
hello_str = "hello"
data_bytes = hello_str.encode('utf-8').ljust(8, b'\x00')  # Pad to 8 bytes: b'hello\x00\x00\x00'

# Create CAN message
msg = can.Message(
    arbitration_id=message.frame_id,  # 0x100
    data=data_bytes,
    is_extended_id=False
)

try:
    print(f"Sending '{hello_str}'...")
    while True:
        bus.send(msg)
        print(f"Sent: {hello_str}")
        time.sleep(1)  # Send every 1 second
except KeyboardInterrupt:
    print("Stopped by user")
except can.CanError as e:
    print(f"CAN error: {e}")
finally:
    bus.shutdown()