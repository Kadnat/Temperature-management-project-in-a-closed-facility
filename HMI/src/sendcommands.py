# -*- coding: utf-8 -*-
import serial
import time
from datetime import datetime
import csv
from src.communication import SerialWorker, decode_frame

# Créer un objet série
class Commands:
    def send_temp(self, temperature):
        ser = serial.Serial('COM6', 9600)

        # Écrire chaque caractère de 'COMMAND:' + temperature sur le port série avec un délai de 100 ms
        # et continuer à envoyer jusqu'à ce que 'command receive' soit reçu sur rx
        while True:
            for char in 'COMMAND:' + temperature + '\n':
                ser.write(char.encode())
                time.sleep(0.1)  # attendre 100 ms
                received = ser.read(ser.inWaiting()).decode()  # lire les données entrantes
                print(f"Reçu : {received}")  # imprimer les données reçues
                if 'command receive' in received:
                    break
            else:
                continue
            break

        # Fermer le port série
        ser.close()


        print(f"Le message 'COMMAND:{temperature}' a été envoyé avec succès sur COM6 à un débit de 9600 bauds avec un délai de 100 ms entre chaque caractère jusqu'à ce que 'command receive' soit reçu.")

    def send_hist(self, days):
        ser = serial.Serial('COM6', 9600)

        # Écrire chaque caractère de 'COMMAND:' + temperature sur le port série avec un délai de 100 ms
        # et continuer à envoyer jusqu'à ce que 'command receive' soit reçu sur rx          
        while True:
            for char in 'HISTORY:'+ days +'\n':
                ser.write(char.encode())
                time.sleep(0.1)  
                received = ser.read(ser.inWaiting()).decode()  
                print(f"Reçu : {received}")  
                if 'command receive' in received:
                    break
            else:
                continue
            break

        ser.close()
        ser = serial.Serial('COM6', 9600)

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