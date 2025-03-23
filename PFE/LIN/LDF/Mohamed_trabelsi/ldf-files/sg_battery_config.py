# This is a small ad hoc script that can be useful in creating a configuration payload for the HELLA IBS 24V

def batt_cnominal_encode(value):
    return int(value)

def batt_u0min(value):
    return int((value - 10.96) * (1.0/0.005))

def batt_u0max(value):
    return int((value - 12.26) * (1.0/0.005))

def batt_ccanominal(value):
    return int((value - 200.0) * (1.0/5.0))

def temperature_upred(value):
    return int((value + 40.0) * (1.0))

def batt_technology(value):
    return int(value)

def batt_change_notification(value):
    return int(1 if value is True else 0)

sg_battery_config = ((0x01 & batt_change_notification(True)) << 43) | \
                    ((0x07 & batt_technology(0))             << 40) | \
                    ((0xFF & temperature_upred(25))          << 32) | \
                    ((0xFF & batt_ccanominal(0))             << 24) | \
                    ((0xFF & batt_u0max(13.00))              << 16) | \
                    ((0xFF & batt_u0min(11.00))              <<  8) | \
                    ((0xFF & batt_cnominal_encode(1.3 + 1.3))<<  0)

# Is entered LSB in CANedge configuration
sg_battery_config_string = ""
for byte in sg_battery_config.to_bytes(6,'little'):
    sg_battery_config_string += f"{byte:02X}"
print(sg_battery_config_string)