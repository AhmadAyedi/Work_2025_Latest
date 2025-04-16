import can
import cantools
import signal
import sys

# Load DBC
db = cantools.database.load_file('can_network14.dbc')
msg = db.get_message_by_name('TemperatureMessage')

# Setup CAN bus
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Graceful exit
def signal_handler(sig, frame):
    print("\nStopping receiver...")
    bus.shutdown()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

print("Listening for TemperatureMessage... Press Ctrl+C to stop.")
while True:
    can_msg = bus.recv()  # blocking
    if can_msg.arbitration_id == msg.frame_id:
        decoded = msg.decode(can_msg.data)
        print(f"Received Temperature: {decoded['temperature']} °C")
