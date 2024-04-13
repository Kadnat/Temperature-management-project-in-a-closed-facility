import serial
import time

# Créer un objet série
ser = serial.Serial('COM6', 9600)

# Écrire chaque caractère de 'ALARMS' sur le port série avec un délai de 100 ms
# et continuer à envoyer jusqu'à ce que 'command receive' soit reçu sur rx
while True:
    for char in 'HISTORY:1\n':
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

print("Le message 'ALARMS' a été envoyé avec succès sur COM7 à un débit de 9600 bauds avec un délai de 100 ms entre chaque caractère jusqu'à ce que 'command receive' soit reçu.")
