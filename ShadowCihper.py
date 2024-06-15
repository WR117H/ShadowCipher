import os
import serial
from modules import functions
# import numpy as np
# import matplotlib.pyplot as plt
# from matplotlib.collections import LineCollection
import time
# import termplotlib as tpl
from colorama import Fore,init
import argparse
recorder_buffer=[]
parser = argparse.ArgumentParser(description="ShadowCipher v1")
# Create argument groups
setup_group = parser.add_argument_group('setup', 'Setup related options')
connection_group = parser.add_argument_group('connection', 'Connection related options')
# Add arguments to the appropriate groups
setup_group.add_argument('-s', '--setup', action='store_true', help="Set up your ShadowCipher with this device")
connection_group.add_argument('-p', '--port', help="Serial port (e.g., COM3 or /dev/ttyUSB0)")
connection_group.add_argument('-b', '--baud', type=int, default=9600, help="Baud rate (default: 9600)")
connection_group.add_argument('-a', '--attack', help="Baud rate (default: 9600)")
# Initial parse to check for setup flag
args = parser.parse_args()

option=0

recorder_buffer=[]
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
def seriall(port,baud,attack):
    # Set up the serial port (Replace 'COM3' with the appropriate port for your setup)





    # print("""[1] rxraw\n[2] graph\n""")
    # option=int(input("Choose an option > "))
    banner()
    try:

       ser = serial.Serial(port, baud, timeout=1)

    except:
       print(f"[!] Couldn't find any device on port: {port}, baud: {baud}")
       exit()

    if attack == 1:
        ser.write(b'A\n')
        try:

            while True:
                if ser.in_waiting > 0:
                    line = ser.readline().decode('utf-8').rstrip()
                    print(line)
        except KeyboardInterrupt:
            print("\nExiting...")
    elif attack == 2:
        ser.write(b'B\n')
        try:
            while True:
                if ser.in_waiting > 0:

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

                      print('\n'+Fore.LIGHTGREEN_EX+"[*] "+Fore.RESET+rssi+f"{[frequency]}")
                    elif frequency == "315 MHZ" and freq315_streak == 0:
                      print(Fore.LIGHTCYAN_EX+"[*] "+rssi+f"{[frequency]}")

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
        #  plot
           None
    elif attack==3:

        try:
            ser.write(b'D\n')
            
            show=False
            while True:

                line = ser.readline().decode('utf-8', errors='ignore').rstrip()
                if line == "" or line == " " or line == "\n":
                    pass
                else:
                   if line == "Recorded RAW data:":
                       show=True
                   if line == 'End':
                       break
                   if show:
                       if line == "Recorded RAW data:":
                           None
                       else:
                           
                           recorder_buffer.append(line)
                   else:
                       print(f'[APP] {line}')



            play=input("[INPUT] Do you wanna play the raw(Yes/Y,No/N) > ")
            if play.lower()=="yes" or play.lower()=="y":
               ser.write(b'E\n')
               try:
                  while True:

                      line = ser.readline().decode('utf-8', errors='ignore').rstrip()
                      if line == "" or line == " " or line == "\n":
                          pass
                      elif line == "Replaying RAW data complete." :
                          print(f'[APP] {line}')
                          break
                      else:
                        print(f'[APP] {line}')
               except KeyboardInterrupt:
              #  plot
                None
            else:
               save=input("[INPUT] Do you wanna save the raw(Yes/Y,No/N) > ")
               if save.lower()=="yes" or save.lower()=="y":
                   None
                   
                       




            
#             hex_data = ''.join(recorder_buffer)
#             # Clean up the raw data string
# # Convert hex to binary
#             binary_data = bin(int(hex_data, 16))[2:].zfill(len(hex_data) * 4)



#             # Define segment length for analysis
#             segment_length = 48

#             # Split binary data into segments
#             segments = [binary_data[i:i+segment_length] for i in range(0, len(binary_data), segment_length)]

#             # Print segments for analysis
#             for i, segment in enumerate(segments):
#                 print(f"Segment {i}: {segment}")

        except KeyboardInterrupt:
        #  plot
           None
    # else:
    #     print(attack)
    elif attack==4:
        ser.write(b'E\n')
        try:
            while True:

                line = ser.readline().decode('utf-8').rstrip()
                if line == "" or line == " " or line == "\n":
                    pass
                else:
                   print(line)
        except KeyboardInterrupt:
        #  plot
           None
    # else:
    elif attack==6:
        ser.write(b'J\n')
    # else:
    #     print(attack)

if __name__=="__main__":

    banner()
    if args.setup:
       print("Setting up ShadowCipher...")
       # Add your setup logic here
    elif args.port and args.baud:

        if args.attack == "replay":

          #  seriall(args.port, args.baud,4)
           exit()
        elif args.attack == "sniffing":

           seriall(args.port, args.baud,1)
        elif args.attack == "scan":
           seriall(args.port, args.baud,2)
        elif args.attack == "capture":
           seriall(args.port, args.baud,3)
           exit()
 
        elif args.attack == "graph":
            seriall(args.port, args.baud,5)
        elif args.attack == "jammer": 
            seriall(args.port, args.baud,6)
