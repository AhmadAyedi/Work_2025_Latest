
import mysql.connector
import can

print("[RASP1] Connecting to database...")

conn = mysql.connector.connect(
    host="10.20.0.27",       # Replace with your PC's IP address
    user="canuser",        # Replace with your MySQL username
    password="123",    # Replace with your password
    database="CAN1"
)

cursor = conn.cursor()
cursor.execute("SELECT command FROM frames WHERE frame_name='led_control'")
result = cursor.fetchone()
conn.close()

if result:
    frame_data = result[0].encode('utf-8')  # 'on' or 'off' becomes b'on' or b'off'
    print(f"[RASP1] Retrieved from DB: {result[0]}")

    bus = can.interface.Bus(channel='can0', interface='socketcan')
    msg = can.Message(arbitration_id=0x123, data=frame_data, is_extended_id=False)
    
    try:
        bus.send(msg)
        print(f"[RASP1] Sent: {frame_data}")
    except can.CanError:
        print("[RASP1] Message not sent")
    bus.shutdown()
else:
    print("[RASP1] No result found")
