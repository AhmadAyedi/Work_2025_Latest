# -*- coding: utf-8 -*-
import mysql.connector

# Replace with your actual PC IP and MySQL credentials
conn = mysql.connector.connect(
    host="10.20.0.23",       # ? your PC IP
    user="canuser",           # ? your MySQL username
    password="123",   # ? your MySQL password
    database="CAN1"
)

cursor = conn.cursor()
cursor.execute("SELECT command FROM frames WHERE frame_name = 'led_control'")
result = cursor.fetchone()

if result:
    frame_data = result[0]
    print(f"[RASP1] Retrieved frame: {frame_data}")
    print(f"[RASP1] Simulating CAN send: {frame_data}")
else:
    print("[RASP1] No frame found.")

conn.close()
