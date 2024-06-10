import serial
import time
import os
import threading
from function import banner

# Replace 'COM4' with your port name (e.g., '/dev/ttyUSB0' for Linux or 'COM4' for Windows)
SERIAL_PORT = 'COM4'
BAUD_RATE = 115200
TIMEOUT = 1  # Timeout for read operations

# Initialize serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=TIMEOUT)

def send_command(command):
    """ Send a command to the Arduino and return the response. """
    ser.write((command + '\r').encode())  # Send command without extra newline

def read_serial():
    """ Continuously read from the serial port and print the output. """
    while True:
        if ser.in_waiting > 0:
            try:
                response = ser.readline().decode('utf-8').strip()
                if response:
                    print(response)
            except Exception as e:
                print(f"Error reading serial data: {e}")

try:
    banner.ban()
    print("[?] Starting the communication. Type 'exit' to end.")
    # Start reading the serial output in a separate thread
    read_thread = threading.Thread(target=read_serial)
    read_thread.daemon = True
    read_thread.start()
    
    while True:
        command = input()
        if command.lower() == 'exit':
            break
        elif command.lower() == 'banner':
            banner.ban()
        elif command.lower() == 'clear':
            os.system('cls' if os.name == 'nt' else 'clear')
        else:
            send_command(command)
        
except KeyboardInterrupt:
    print("Interrupted by user")

finally:
    ser.close()  # Close the serial connection
    print("Serial connection closed.")
