import can
import cantools
import time
import signal
import sys

# Load the DBC
db = cantools.database.load_file('simple_network.dbc')
msg = db.get_message_by_name('TemperatureMessage')

# Graceful shutdown on Ctrl+C
def signal_handler(sig, frame):
    print("\nStopping CAN transmission...")
    bus.shutdown()
    sys.exit(0)

signal.signal(signal.SIGINT, signal_handler)

# Create CAN bus
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Loop and send message every 1 second
temperature = 42
print("Starting to send CAN messages... Press Ctrl+C to stop.")

while True:
    data = msg.encode({'temperature': temperature})
    can_msg = can.Message(arbitration_id=msg.frame_id, data=data, is_extended_id=False)
    try:
        bus.send(can_msg)
        print("Sent Temperature:", temperature, "°C")
    except can.CanError:
        print("Message NOT sent!")
    time.sleep(1)  # send every second
