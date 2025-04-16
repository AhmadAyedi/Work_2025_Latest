import can
import cantools
import time
import signal
import sys

# Load DBC
db = cantools.database.load_file('can_network14.dbc')
msg = db.get_message_by_name('TemperatureMessage')

# Graceful exit
def signal_handler(sig, frame):
    print("\nStopping sender...")
    bus.shutdown()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

# Setup CAN bus
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Send loop
temperature = 42
print("Sending temperature every second. Press Ctrl+C to stop.")
while True:
    data = msg.encode({'temperature': temperature})
    can_msg = can.Message(arbitration_id=msg.frame_id, data=data, is_extended_id=False)
    try:
        bus.send(can_msg)
        print(f"Sent Temperature: {temperature} °C")
    except can.CanError:
        print("Failed to send CAN message.")
    time.sleep(1)
