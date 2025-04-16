import can
import cantools
import signal
import sys

# Load DBC
db = cantools.database.load_file("dbc15.dbc")
sensor_msg = db.get_message_by_name("SensorData")
ack_msg = db.get_message_by_name("AckMessage")

# Setup CAN bus
bus = can.interface.Bus(channel='can0', interface='socketcan')

# Graceful shutdown
def shutdown(sig, frame):
    print("\nShutting down receiver...")
    bus.shutdown()
    sys.exit(0)

signal.signal(signal.SIGINT, shutdown)

print("Listening for SensorData... Ctrl+C to stop.")
while True:
    msg = bus.recv()  # Blocking

    if msg.arbitration_id == sensor_msg.frame_id:
        decoded = sensor_msg.decode(msg.data)
        print(f"ðŸ“¡ Received SensorData: "
              f"{decoded['temperature']}Â°C | "
              f"{decoded['voltage']}V | "
              f"{decoded['current']}A | "
              f"Flag: {decoded['status_flag']}")

        # Send acknowledgment
        encoded_ack = ack_msg.encode({"ack": 1})
        ack_can_msg = can.Message(arbitration_id=ack_msg.frame_id, data=encoded_ack, is_extended_id=False)
        try:
            bus.send(ack_can_msg)
            print("â†ªï¸  Sent Acknowledgment")
        except can.CanError:
            print("âŒ Failed to send acknowledgment")
