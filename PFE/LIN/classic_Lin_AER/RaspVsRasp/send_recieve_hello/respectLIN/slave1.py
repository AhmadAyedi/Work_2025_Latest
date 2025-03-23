import serial

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

def listen_for_request():
    """Listen for a request from the master."""
    if linSerial.in_waiting > 0:
        received_data = linSerial.readline().decode('utf-8').strip()
        delimiter_index = received_data.find(':')

        if delimiter_index > 0:
            message_id = received_data[:delimiter_index]
            message = received_data[delimiter_index + 1:]

            # Check if the message ID is 1 (Request for temperature)
            if message_id == "1" and message == "get_temperature":
                temperature_data = "20"  # Fixed temperature value to send to master
                response_message = f"{message_id}:{temperature_data}"
                linSerial.write(response_message.encode('utf-8'))
                print(f"Slave Sent: {response_message.strip()}")

try:
    print("Slave Ready")
    while True:
        listen_for_request()  # Listen for request from master
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
