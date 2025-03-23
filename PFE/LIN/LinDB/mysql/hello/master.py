import serial
import time
import RPi.GPIO as GPIO
import mysql.connector  # Import MySQL connector

# MySQL connection configuration
mysql_config = {
    "host": "10.20.0.27",     # IP of the MySQL server (your local computer)
    "user": "root",           # MySQL username
    "password": "root",       # MySQL password
    "database": "lin_data2"    # Name of the database you created in Workbench
}

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

# Function to send message to MySQL
def send_message_to_mysql(message, message_id):
    try:
        # Establish a connection to MySQL
        conn = mysql.connector.connect(**mysql_config)
        cursor = conn.cursor()

        # SQL query to insert the message
        sql_query = "INSERT INTO messages (message_id, message) VALUES (%s, %s)"
        cursor.execute(sql_query, (message_id, message))
        
        # Commit the transaction
        conn.commit()
        
        print(f"Message '{message}' inserted into MySQL.")
        
        # Close the cursor and connection
        cursor.close()
        conn.close()
    except mysql.connector.Error as err:
        print(f"Error: {err}")

def send_message(message, message_id):
    formatted_message = f"{message_id}:{message}\n"
    linSerial.write(formatted_message.encode('utf-8'))
    print(f"Sent: {formatted_message.strip()}")
    
    # Send the message to MySQL as well
    send_message_to_mysql(message, message_id)

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
