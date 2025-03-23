CREATE DATABASE lin_project;
USE lin_project;


CREATE TABLE messages (
    id INT AUTO_INCREMENT PRIMARY KEY,
    button_state BOOLEAN,   -- Stores the state of the button (1 = pressed, 0 = not pressed)
    led_state BOOLEAN,      -- Stores the state of the LED (1 = ON, 0 = OFF)
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


SELECT * FROM messages;
