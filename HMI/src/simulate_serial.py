import threading
import serial
import time

from src.communication import encode_temperature

current_frame = '180409141c3a16190200000000000000'  # Variable globale pour la trame actuelle
current_frame_lock = threading.Lock()

def update_current_frame(new_temperature):
    global current_frame
    current_frame = encode_temperature(new_temperature)

def send_predefined_frame(port, baudrate, interval):
    global current_frame
    with serial.Serial(port, baudrate) as ser:
        print(f"Ouverture du port série {port} à {baudrate} bauds.")
        try:
            while True:
                frame_to_send = current_frame
                print(f"Envoi de la trame : {frame_to_send}")
                ser.write(bytes.fromhex(frame_to_send))
                time.sleep(interval)
        except KeyboardInterrupt:
            print("Arrêt de la simulation sur demande de l'utilisateur.")
        except serial.SerialException as e:
            print(f"Erreur de port série : {e}")

# Cette partie permet de lancer la fonction send_predefined_frame si le fichier est exécuté directement.
if __name__ == "__main__":
    predefined_frame = "180409141c3a16190200000000000000"
    send_predefined_frame('COM8', 9600, predefined_frame, 10)
