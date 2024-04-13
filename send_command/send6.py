
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
ser = serial.Serial('COM6', 9600)

# Ouvrir le port COM6


while True:
    # Lire une trame depuis le port série
    trame = ser.readline().decode().strip()

    # Enlever les accolades et convertir en entiers
    trame = trame.strip("{}")
    resultat = [int(trame[i:i+2], 16) for i in range(0, len(trame), 2)]

    # Afficher le résultat
    print(resultat)

ser.close()