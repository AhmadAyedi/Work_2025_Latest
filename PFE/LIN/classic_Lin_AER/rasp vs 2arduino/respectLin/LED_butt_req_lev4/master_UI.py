import serial
import time
import pygame

# Initialize LIN communication
linSerial = serial.Serial('/dev/serial0', baudrate=9600, timeout=1)

# Initialize Pygame
pygame.init()

# Screen settings
screen_width, screen_height = 800, 400
screen = pygame.display.set_mode((screen_width, screen_height))
pygame.display.set_caption("Raspberry Pi LIN Master Interface")

# Colors
WHITE = (255, 255, 255)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BLACK = (0, 0, 0)

# Font
font = pygame.font.SysFont(None, 80)

def display_message(text, color):
    screen.fill(WHITE)
    message = font.render(text, True, color)
    text_rect = message.get_rect(center=(screen_width/2, screen_height/2))
    screen.blit(message, text_rect)
    pygame.display.flip()

print("Master Ready")

try:
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                linSerial.close()
                exit()

        # Send request to Slave 1
        linSerial.write(b"request_button_press\n")
        display_message("Requesting Button Press...", BLACK)
        time.sleep(1)

        # Check for incoming messages
        if linSerial.in_waiting > 0:
            message = linSerial.readline().decode('utf-8').strip()
            print(f"Received: {message}")

            if message == "button_pressed":
                print("Button pressed! Sending toggle command to Slave 2")
                display_message("Button Pressed!", GREEN)
                linSerial.write(b"toggle_led\n")
                time.sleep(1)  # Show message for a second

except KeyboardInterrupt:
    print("\nExiting program")
    linSerial.close()
    pygame.quit()
