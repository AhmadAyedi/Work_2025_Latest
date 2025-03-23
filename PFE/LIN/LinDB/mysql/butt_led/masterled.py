import serial
import time
import RPi.GPIO as GPIO
import mysql.connector  # Import MySQL connector

# MySQL connection configuration (NEW DATABASE)
mysql_config = {
    "host": "10.20.0.27",     # MySQL server IP (your local computer)
    "user": "root",           # MySQL username
    "password": "root",       # MySQL password
    "database": "lin_project"  # New database
}

# GPIO setup
LED_PIN = 2
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)  # LED setup

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

# LED State
led_state = 0  # Initially OFF

# Function to send data to MySQL
def send_data_to_mysql(button_state, led_state):
    try:
        conn = mysql.connector.connect(**mysql_config)
        cursor = conn.cursor()

        sql_query = "INSERT INTO messages (button_state, led_state) VALUES (%s, %s)"
        cursor.execute(sql_query, (button_state, led_state))
        conn.commit()

        print(f"Data saved: Button={button_state}, LED={led_state}")

        cursor.close()
        conn.close()
    except mysql.connector.Error as err:
        print(f"MySQL Error: {err}")

# Function to toggle LED
def toggle_led():
    global led_state
    led_state = not led_state  # Toggle state
    GPIO.output(LED_PIN, led_state)  # Change LED state
    return led_state

# Listen for messages from Slave
try:
    print("Master Ready")
    while True:
        if linSerial.in_waiting > 0:
            received_data = linSerial.readline().decode('utf-8').strip()

            if received_data == "TOGGLE":
                print("Received TOGGLE command")
                
                # Toggle LED
                new_led_state = toggle_led()
                print(f"LED {'ON' if new_led_state else 'OFF'}")

                # Send data to MySQL
                send_data_to_mysql(1, new_led_state)  # 1 means button was pressed
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
    GPIO.cleanup()
