from ldfparser import parseLDF

# Load the LDF file
ldf_file_path = 'ldf44.ldf'  # Replace with the actual path to your .ldf file
ldf = parseLDF(ldf_file_path)

# Display basic LDF information
print("LIN Protocol Version:", ldf.protocol_version)
print("LIN Speed:", ldf.baudrate, "kbps")

# Master node
print("\nMaster Node:")
master_node = ldf.get_master()
print("  Name:", master_node.name)

# Slave nodes
print("\nSlave Nodes:")
for slave_node in ldf.get_slaves():
    print(f"  Name: {slave_node.name}, Configured NAD: {slave_node.configured_nad}")

# Signals
print("\nSignals:")
for signal in ldf.get_signals():
    signal_name = getattr(signal, 'name', 'Unknown')
    signal_length = getattr(signal, 'length', 'Length not available')
    signal_sender = getattr(signal, 'sender', 'Unknown sender')
    print(f"  Signal Name: {signal_name}, Length: {signal_length}, Sender: {signal_sender}")

# Frames
print("\nFrames:")
for frame in ldf.get_unconditional_frames():
    print(f"  Frame Name: {frame.name}, ID: {frame.frame_id}, Length: {frame.length} bytes")

    # Access the signals related to the frame through another approach
    if hasattr(frame, 'signals'):
        for signal in frame.signals:
            print(f"    Signal: {signal.name}, Length: {signal.length} bits")
    else:
        print(f"    No signals directly associated with this frame.")

# Schedule tables
print("\nSchedule Tables:")
schedule_tables = ldf.get_schedule_tables()

# Convert dict_values to a list and iterate over it
for schedule in schedule_tables:
    print(f"  Schedule Name: {schedule.name}")
    for entry in schedule.schedule:
        # Check if entry has a frame (regular frame) or is a diagnostic frame
        if hasattr(entry, 'frame'):
            print(f"    Frame: {entry.frame.name}, Delay: {entry.delay} ms")
        else:
            # If it's not a frame, check if it has a diagnostic frame or related name
            if hasattr(entry, 'signal_name'):
                print(f"    Diagnostic Frame: {entry.signal_name}, Delay: {entry.delay} ms")
            elif hasattr(entry, '__str__'):
                # Print the string representation of the entry for better understanding
                print(f"    Diagnostic Frame: {str(entry)}, Delay: {entry.delay} ms")
            else:
                print(f"    Unknown Diagnostic Frame: {entry}, Delay: {entry.delay} ms")
