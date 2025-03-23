import serial
import time
import RPi.GPIO as GPIO
from pymongo import MongoClient  # Import pymongo to interact with MongoDB

# MongoDB setup - Connect to the MongoDB instance running on your Windows machine
mongo_client = MongoClient("mongodb://10.20.0.27:27017/")  # Replace with your Windows IP address
db = mongo_client["lin_messages_db"]  # Database name
collection = db["messages"]  # Collection name

# GPIO setup
SLP_PIN = 18
GPIO.setmode(GPIO.BCM)
GPIO.setup(SLP_PIN, GPIO.OUT)
GPIO.output(SLP_PIN, GPIO.HIGH)  # Wake up LIN module

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# Define schedule table with message IDs and corresponding data
schedule_table = [
    {"id": 2, "message": "hello raspberry2"}  # Sending to Slave (raspberry2)
]

def send_message(message, message_id):
    formatted_message = f"{message_id}:{message}\n"
    
    # Insert the message into MongoDB
    collection.insert_one({
        "message_id": message_id,
        "message": message,
        "timestamp": time.time()
    })
    print(f"Message saved to MongoDB: {formatted_message.strip()}")
    
    # Send the message via serial to LIN module
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Sent: {formatted_message.strip()}")
    
    time.sleep(1)

try:
    print("Master Ready")
    while True:
        for entry in schedule_table:
            send_message(entry["message"], entry["id"])
            time.sleep(2)  # Schedule table period
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
