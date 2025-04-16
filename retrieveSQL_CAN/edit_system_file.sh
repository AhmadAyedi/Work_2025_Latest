#!/bin/bash
if [[ "$1" == "/boot/config.txt" || "$1" == "/etc/sudoers.d/can_setup" ]]; then
    sudo nano "$1"
else
    echo "Error: Only /boot/config.txt and /etc/sudoers.d/can_setup are allowed."
    exit 1
fi
