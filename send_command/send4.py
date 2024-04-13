# -*- coding: utf-8 -*-

import serial
import time
import csv
import re

ser = serial.Serial('COM6', 9600)

def is_hex(s):
    try:
        int(s, 16)
        return True
    except ValueError:
        return False

def get_value(data_values, index, default=0):
    try:
        # Convert hexadecimal to decimal
        return int(str(data_values[index]), 16)
    except IndexError:
        return default


with open('data.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["year", "month", "day", "hour", "minute", "second", "temperature_decimal", "temperature_fraction", "error_type", "command_decimal", "command_fraction"])

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
    
    frame = ""
    while True:
        try:
            char = ser.read().decode()
        except UnicodeDecodeError:
            print("Erreur de dcodage Unicode")
            continue
        frame += char
        if char == '}':
            byte_list = [frame[i:i+2] for i in range(0, len(frame), 2)]
            data_values = [int(b, 16) if is_hex(b) else 0 for b in byte_list]

            data = {
                'year': get_value(data_values, 0),
                'month': get_value(data_values, 1),
                'day': get_value(data_values, 2),
                'hour': get_value(data_values, 3),
                'minute': get_value(data_values, 4),
                'second': get_value(data_values, 5),
                'temperature_decimal': get_value(data_values, 6),
                'temperature_fraction': get_value(data_values, 7),
                'error_type': get_value(data_values, 8),
                'command_decimal': get_value(data_values, 9),
                'command_fraction': get_value(data_values, 10),
            }
            writer.writerow(data.values())
            print(data.values())
            frame = ""

ser.close()
