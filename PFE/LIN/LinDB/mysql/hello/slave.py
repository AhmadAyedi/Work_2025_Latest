import serial

# Serial setup
linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

def listen_for_messages():
    if linSerial.in_waiting > 0:
        received_data = linSerial.readline().decode('utf-8').strip()
        delimiter_index = received_data.find(':')

        if delimiter_index > 0:
            message_id = received_data[:delimiter_index]
            message = received_data[delimiter_index + 1:]

            if message_id == "2":  # Slave is listening for ID 2
                print(f"Received: {message}")

try:
    print("Slave Ready")
    while True:
        listen_for_messages()
except KeyboardInterrupt:
    print("Stopping...")
finally:
    linSerial.close()
