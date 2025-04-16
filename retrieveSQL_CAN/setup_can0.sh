#!/bin/bash
ip link set can0 down
ip link set can0 up type can bitrate 500000
