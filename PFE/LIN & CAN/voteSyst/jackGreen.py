import serial
import RPi.GPIO as GPIO

GREEN_LED = 27  # GPIO Pin for Jack's Green LED
vote_count = 0  # Counter for Jack's votes
led_status = False  # Track LED state to prevent duplicate counts

GPIO.setmode(GPIO.BCM)
GPIO.setup(GREEN_LED, GPIO.OUT)
GPIO.output(GREEN_LED, GPIO.LOW)

linSerial = serial.Serial("/dev/serial0", baudrate=9600, timeout=1)

print("🟢 Raspberry Pi 1 (Jack) Ready\n")

try:
    while True:
        if linSerial.in_waiting:
            received_data = linSerial.readline().decode('utf-8').strip()

            try:
                data_id, command = received_data.split(',')

                if data_id == "1":  # Jack's LED
                    if command == "LED_ON" and not led_status:
                        GPIO.output(GREEN_LED, GPIO.HIGH)
                        vote_count += 1  # Increment count only if LED was OFF
                        led_status = True
                        print(f"✅ Jack's LED ON | Total Votes: {vote_count}")
                    
                    elif command == "LED_OFF" and led_status:
                        GPIO.output(GREEN_LED, GPIO.LOW)
                        led_status = False
                        print("⚪ Jack's LED OFF")

            except ValueError:
                pass  # Ignore invalid data

except KeyboardInterrupt:
    GPIO.cleanup()
    linSerial.close()
