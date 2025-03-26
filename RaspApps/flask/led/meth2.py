from flask import Flask, render_template
import RPi.GPIO as GPIO

app = Flask(__name__)
LED_PIN = 26

GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

@app.route('/')
def index():
    return '''
    <h1>LED Controller</h1>
    <a href="/on">Turn ON</a> |
    <a href="/off">Turn OFF</a>
    '''

@app.route('/on')
def led_on():
    GPIO.output(LED_PIN, GPIO.HIGH)
    return 'LED is ON! <a href="/">Back</a>'

@app.route('/off')
def led_off():
    GPIO.output(LED_PIN, GPIO.LOW)
    return 'LED is OFF! <a href="/">Back</a>'

if __name__ == '__main__':
    try:
        app.run(host='0.0.0.0', port=5000, debug=True)
    finally:
        GPIO.cleanup()
