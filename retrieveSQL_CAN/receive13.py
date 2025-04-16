import can
import cantools

# Load the DBC
db = cantools.database.load_file('dbc13.dbc')
msg = db.get_message_by_name('TemperatureMessage')

# Create CAN bus
bus = can.interface.Bus(channel='can0', bustype='socketcan')

print("Waiting for CAN message...")

# Receive message
while True:
    message = bus.recv()
    if message.arbitration_id == msg.frame_id:
        decoded = msg.decode(message.data)
        print("Received Temperature:", decoded['temperature'], "°C")
