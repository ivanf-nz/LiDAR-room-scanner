#old serial port reading code 

import serial
import time

# Configuration
arduino_port = "COM5"  # Update to your Arduino's port
baud_rate = 115200
output_file = "lidar_data3.xyz"

def is_valid_xyz_format(line):
    """
    Validate if the line is in the format of 'x y z',
    where x, y, z are floating-point or integer numbers.
    """
    try:
        parts = line.split()
        if len(parts) != 3:
            return False
        float(parts[0])  # Check x
        float(parts[1])  # Check y
        float(parts[2])  # Check z
        return True
    except ValueError:
        return False

def main():
    ser = None
    try:
        # Open serial connection
        print(f"Connecting to Arduino on {arduino_port}...")
        ser = serial.Serial(arduino_port, baud_rate, timeout=5)
        time.sleep(2)  # Wait for Arduino to initialize

        # Wait for "READY" signal
        while True:
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                if line == "READY":
                    print("Arduino is ready. Starting data logging.")
                    break

        # Open file for logging
        with open(output_file, "w") as f:
            print(f"Logging data to {output_file}... Press Ctrl+C to stop.")
            while True:
                if ser.in_waiting > 0:
                    # Read data from Arduino
                    try:
                        line = ser.readline().decode('utf-8').strip()
                        print(f"Received: {line}")  # Debugging: Log raw received data
                        if is_valid_xyz_format(line):
                            print(line)  # Optional: Print to console
                            f.write(line + "\n")
                            f.flush()
                        else:
                            print(f"Ignored invalid format: {line}")
                    except Exception as e:
                        print(f"Error processing line: {e}")
    except KeyboardInterrupt:
        print("\nLogging stopped.")
    except Exception as e:
        print(f"Error: {e}")
    finally:
        if ser:
            ser.close()

if __name__ == "__main__":
    main()