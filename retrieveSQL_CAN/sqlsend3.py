import mysql.connector
import can
import time
import os
import signal
import sys

# MySQL Configuration
MYSQL_HOST = '192.168.1.13'  # Replace with your MySQL server IP
MYSQL_USER = 'root'
MYSQL_PASSWORD = 'root'
MYSQL_DATABASE = 'can3'

# CAN bus setup
def setup_can():
    try:
        os.system('sudo ifconfig can0 down')
        time.sleep(1)
        os.system('sudo ip link set can0 type can bitrate 500000')
        os.system('sudo ifconfig can0 up')
        bus = can.interface.Bus(channel='can0', interface='socketcan')
        return bus
    except Exception as e:
        print(f"Failed to setup CAN bus: {e}")
        sys.exit(1)

# Connect to MySQL and retrieve frames
def get_frames():
    try:
        connection = mysql.connector.connect(
            host=MYSQL_HOST,
            user=MYSQL_USER,
            password=MYSQL_PASSWORD,
            database=MYSQL_DATABASE
        )
        cursor = connection.cursor()
        cursor.execute("SELECT frame_name, command FROM frame_commands")
        frames = cursor.fetchall()
        cursor.close()
        connection.close()
        return frames
    except mysql.connector.Error as err:
        print(f"MySQL Error: {err}")
        return []

# Convert frame_name and command to 8-byte CAN data (5:2 format)
def string_to_bytes(frame_name, command):
    # Abbreviate frame_name to 5 chars, command to 2 chars
    frame_short = frame_name[:5]
    cmd_short = command[:2]
    data = f"{frame_short}:{cmd_short}"
    return data.encode('utf-8')[:8]

# Cleanup on exit
def cleanup(bus):
    print("Shutting down CAN bus...")
    bus.shutdown()
    os.system('sudo ifconfig can0 down')

# Signal handler for clean exit
def signal_handler(sig, frame):
    cleanup(bus)
    sys.exit(0)

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    bus = setup_can()
    frames = get_frames()
    
    if not frames:
        print("No frames retrieved from database")
        cleanup(bus)
        sys.exit(1)

    print("Sending frames...")
    for frame_name, command in frames:
        arb_id = hash(frame_name) & 0x1FFFFFFF
        data = string_to_bytes(frame_name, command)
        
        msg = can.Message(
            arbitration_id=arb_id,
            data=data,
            is_extended_id=True
        )
        
        try:
            bus.send(msg)
            print(f"Sent: {frame_name} - {command} (Encoded: {data.decode('utf-8')})")
            time.sleep(1)
        except can.CanError as e:
            print(f"CAN Error: {e}")
    
    cleanup(bus)