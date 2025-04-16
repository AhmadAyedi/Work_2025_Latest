import can
import cantools
import time
import signal
import sys
import random

# Load DBC and message
db = cantools.database.load_file("dbc15.dbc")
sensor_msg = db.get_message_by_name("SensorData")
ack_msg = db.get_message_by_name("AckMessage")

# Setup CAN bus
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Graceful shutdown
def shutdown(sig, frame):
    print("\nShutting down sender...")
    bus.shutdown()
    sys.exit(0)

signal.signal(signal.SIGINT, shutdown)

# Simulate sensor data
last_values = {"temperature": None, "voltage": None, "current": None, "status_flag": None}

print("Starting advanced CAN sender... Ctrl+C to exit.")
while True:
    # Randomly generate fake sensor data
    data = {
        "temperature": random.randint(20, 80),
        "voltage": round(random.uniform(10.0, 14.0), 1),
        "current": round(random.uniform(0.5, 5.0), 2),
        "status_flag": random.randint(0, 1),
    }

    # Only send if values have changed
    if data != last_values:
        encoded = sensor_msg.encode(data)
        can_msg = can.Message(arbitration_id=sensor_msg.frame_id, data=encoded, is_extended_id=False)
        bus.send(can_msg)
        print(f"Sent: {data}")
        last_values = data

    # Check for acknowledgment
    ack = bus.recv(timeout=0.5)
    if ack and ack.arbitration_id == ack_msg.frame_id:
        decoded_ack = ack_msg.decode(ack.data)
        if decoded_ack.get("ack"):
            print("✅ Acknowledgment received.")

    time.sleep(1)
