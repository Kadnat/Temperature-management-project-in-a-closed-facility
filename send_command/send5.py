import re
import serial
import time

ser = serial.Serial('COM6', 9600)

while True:
        for char in 'HISTORY:1\n':
            ser.write(char.encode())
            time.sleep(0.1)  
            try:
                received = ser.read(ser.inWaiting()).decode()  
            except UnicodeDecodeError:
                print("Erreur de dcodage Unicode")
                continue
            print(f"Reçu : {received}")  
            if 'command receive' in received:
                break
        else:
            continue
        break
ser.close()

def store_frames(port, filename):
    # Pattern for frame type with exactly 16 occurrences of %02f
    pattern = re.compile(r"(\{%02f\}){16}")

    # Open the serial port
    with serial.Serial(port, baudrate=9600, timeout=1) as ser, open(filename, 'w') as file:
        while True:
            # Read a line from the serial port
            line = ser.readline().decode('utf-8').strip()

            # If the line contains the word "sector", stop reading
            if 'sector' in line:
                break

            # If the line contains exactly 16 occurrences of %02f, write it to the file
            if pattern.fullmatch(line):
                file.write(line + '\n')

# Example usage:
store_frames('COM6', 'frames.txt')
