
import serial
import time
import csv

ser = serial.Serial('COM6', 9600)

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

    while True:
        try:
            frame = ser.read(ser.inWaiting()).decode()
        except UnicodeDecodeError:
            print("Erreur de dcodage Unicode")
            continue
        if 'Sector' in frame:
            break
        byte_list = [frame[i:i+2] for i in range(0, len(frame), 2)]
        data_values = [int(b, 16) if b else 0 for b in byte_list]

        def get_value(index, default=0):
            try:
                return data_values[index]
            except IndexError:
                return default

        data = {
            'year': get_value(0),
            'month': get_value(1),
            'day': get_value(2),
            'hour': get_value(3),
            'minute': get_value(4),
            'second': get_value(5),
            'temperature_decimal': get_value(6),
            'temperature_fraction': get_value(7),
            'error_type': get_value(8),
            'command_decimal': get_value(9),
            'command_fraction': get_value(10),
        }
        writer.writerow(data.values())

ser.close()
