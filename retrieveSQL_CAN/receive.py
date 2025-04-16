import can

def receive_can_data():
    try:
        bus = can.interface.Bus(channel='can0', interface='socketcan')

        print("Listening for CAN messages...")

        while True:
            message = bus.recv()  # Wait for a message
            if message is not None:
                print(f"Received CAN Message:")
                print(f"  ID        : {hex(message.arbitration_id)}")
                print(f"  DLC       : {message.dlc} bytes")
                print(f"  Data      : {list(message.data)}")

    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    receive_can_data()
