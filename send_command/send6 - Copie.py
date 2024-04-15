import serial
import csv
import time
from datetime import datetime

ser = serial.Serial('COM7', 9600)

while True:
    for char in 'ALARMS\n':
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
ser = serial.Serial('COM7', 9600)

# Obtenir la date d'aujourd'hui et la convertir en chaîne de caractères
today = datetime.now().strftime('%Y-%m-%d')

# Ouvrir le fichier CSV en mode écriture
with open(f'{today}.csv', 'w', newline='') as file:
    writer = csv.writer(file)
    # Écrire l'en-tête du fichier CSV
    writer.writerow(["Year", "Month", "Day", "Hour", "Minute", "Second", "Temp Decimal", "Temp Fraction", "Error Type", "Command Decimal", "Command Fraction"])

    while True:
        # Lire une trame depuis le port série
        trame = ser.readline().decode().strip()

        # Si le mot "sector" est reçu, arrêter de lire
        if 'Sec' in trame:
            break

        # Enlever les accolades et convertir en entiers
        trame = trame.strip("{}")
        resultat = [int(trame[i:i+2], 16) for i in range(0, len(trame), 2)]

        # Afficher le résultat
        print(resultat)

        # Écrire le résultat dans le fichier CSV
        writer.writerow(resultat)

ser.close()
