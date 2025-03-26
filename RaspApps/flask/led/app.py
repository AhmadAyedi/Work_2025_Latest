from flask import Flask, render_template
import RPi.GPIO as GPIO

app = Flask(__name__)

LED_PIN = 26
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED_PIN, GPIO.OUT)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/on")
def led_on():
    GPIO.output(LED_PIN, GPIO.HIGH)
    return "LED ON"

@app.route("/off")
def led_off():
    GPIO.output(LED_PIN, GPIO.LOW)
    return "LED OFF"

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True)
