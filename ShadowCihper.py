import os
import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.collections import LineCollection
import time
import termplotlib as tpl
from colorama import Fore,init
freq433_streak=0
freq315_streak=0
init()
def DELAY(x):
   time.sleep(x)
timestamps = []
rssi_list = []
def read_serial_data(rssi_list):
    for rssi_data in rssi_list:
        yield rssi_data

def banner():
    os.system("cls||clear")
    header = """
  _________.__                .___            _________ .__       .__                  
 /   _____/|  |__ _____     __| _/______  _  _\\_   ___ \\|__|_____ |  |__   ___________ 
 \\_____  \\ |  |  \\\\__  \\   / __ |/  _ \\ \\/ \\/ /    \\  \\/|  \\____ \\|  |  \\_/ __ \\_  __ \\
 /        \\|   Y  \\/ __ \\_/ /_/ (  <_> )     /\\     \\___|  |  |_> >   Y  \\  ___/|  | \\/
/_______  /|___|  (____  /\\____ |\\____/ \\/\\_/  \\______  /__|   __/|___|  /\\___  >__|   
        \\/      \\/     \\/      \\/                     \\/   |__|        \\/     \\/       
"""
    print(header)
    print("[c] ShadowCipher - 2024\n")
def seriall():
    # Set up the serial port (Replace 'COM3' with the appropriate port for your setup)
    ser = serial.Serial('COM3', 115200)  # Change 'COM3' to your port name

    
    print("""[1] rxraw\n[2] scan\n""")
    option=int(input("Choose an option > "))
    banner()
    if option == 1:
        ser.write(b'A\n')
        try:
            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
        except KeyboardInterrupt:
            print("\nExiting...")
    elif option == 2:
        ser.write(b'B\n')
        try:
            while True:
                if ser.in_waiting > 0:
                    DL=False
                    # Read RSSI data from serial port
                    rssi = int(ser.readline().decode().strip())
                    rssi_list.append(rssi)  
                                        # Set thresholds for each frequency
                    threshold_433 = -50  # Adjust this threshold as needed
                    threshold_315 = -70  # Adjust this threshold as needed
                    frequency=""
                    # Check if RSSI corresponds to 433 MHz or 315 MHz
                    if rssi > threshold_433:
                        frequency = "433 MHz"

                    elif rssi > threshold_315:
                        frequency = "315 MHz"

                    # Print ASCII representation of RSSI
                    rssi=(f"{'*'* (rssi // 10)} ({rssi})")
                    
                    if frequency == "433 MHz" and freq433_streak == 0:

                      print(Fore.LIGHTGREEN_EX+"\n[*] "+Fore.RESET+rssi+f"{[frequency]}")
                    elif frequency == "315 MHZ" and freq315_streak == 0:
                      print(Fore.LIGHTCYAN_EX+"\n[*] "+rssi+f"{[frequency]}")  
                    else:
                      print('\r'+"[*] "+rssi, end='')  
                    if frequency == "433 MHz":

                        freq433_streak =+ 1
                        freq315_streak = 0
                    elif frequency == "315 MHz":

                        freq315_streak =+ 1
                        freq433_streak = 0
                    else: 
                        freq315_streak = 0 
                        freq433_streak = 0
                                      
                        
        except KeyboardInterrupt:
        #    plot
if __name__=="__main__":
    banner()
    seriall()
