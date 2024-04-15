from datetime import datetime
import time
from PyQt5.QtCore import QObject, pyqtSignal, QThread
import serial
from threading import Lock

current_temperature = 25.0  # Température initiale
serial_port_lock = Lock()

class SerialWorker(QObject):
    data_received = pyqtSignal(str)

    def __init__(self, port='COM7', baudrate=9600, timeout=1):
        super().__init__()
        self.serial_port = serial.Serial(port=port, baudrate=baudrate, timeout=timeout)
        self.thread = QThread()
        self.moveToThread(self.thread)
        self.thread.started.connect(self.run)
        self.running = False

    def start(self):
        if not self.thread.isRunning():
            self.running = True
            self.thread.start()

    def stop(self):
        self.running = False
        self.thread.quit()
        self.thread.wait()
        if self.serial_port.is_open:
            self.serial_port.close()

    def run(self):
        while self.running:
            try:
                if self.serial_port.in_waiting > 0:
                    data_bytes = self.serial_port.readline()
                    data_hex = data_bytes.hex()
                    # print(f"Trame reçue : {data_hex}")  # Debug: Affiche la trame reçue
                    self.data_received.emit(data_hex)
            except serial.SerialException as e:
                print(f"Exception série : {e}")
    

    def send_temp(self, temperature):
        # Écrire chaque caractère de 'COMMAND:' + temperature sur le port série avec un délai de 100 ms
        # et continuer à envoyer jusqu'à ce que 'command receive' soit reçu sur rx
        while True:
            for char in 'COMMAND:' + temperature + '\n':
                self.send_data(char.encode())
                time.sleep(0.1)  # attendre 100 ms
                received = self.serial_port.read(self.serial_port.inWaiting()).decode()  # lire les données entrantes
                print(f"Reçu : {received}")  # imprimer les données reçues
                if 'command receive' in received:
                    break
            else:
                continue
            break

        print(f"Le message 'COMMAND:{temperature}' a été envoyé avec succès sur COM6 à un débit de 9600 bauds avec un délai de 100 ms entre chaque caractère jusqu'à ce que 'command receive' soit reçu.")

    def send_hist(self, days):
            ser = self.serial_port

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
            ser = self.serial_port

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

    def send_frame(self, frame):
        with serial_port_lock:
            if not self.serial_port.is_open:
                self.serial_port.open()
            self.serial_port.write(bytes.fromhex(frame))
        
def send_temperature_frame(port, baudrate, interval):
    global current_temperature
    with serial.Serial(port, baudrate) as ser:
        try:
            while True:
                for char in 'COMMAND:'+current_temperature+'\n':
                    ser.write(char.encode())
                    time.sleep(0.1)  # attendre 100 ms
                    received = ser.read(ser.inWaiting()).decode()  # lire les données entrantes
                    if 'command receive' in received:
                        break
                else:
                    continue
                break
            ser.close()
            #while True:
             #   with serial_port_lock:
              #      frame = encode_temperature(str(current_temperature))
               #     ser.write(bytes.fromhex(frame))
               # time.sleep(interval)
        except KeyboardInterrupt:
            print("Arrêt de la simulation sur demande de l'utilisateur.")
        except serial.SerialException as e:
            print(f"Erreur de port série : {e}")
            
def encode_temperature(temperature):
    # Obtient la date et l'heure actuelles
    now = datetime.now()
    
    # Convertit la température en un nombre à virgule flottante
    temp_float = float(temperature)
    temp_int = int(temp_float)
    temp_frac = int((temp_float - temp_int) * 100)
    
    # Formate les données en hexadécimal
    year_hex = '{:02x}'.format(now.year % 100)  # Utilise les deux derniers chiffres de l'année
    month_hex = '{:02x}'.format(now.month)
    day_hex = '{:02x}'.format(now.day)
    hour_hex = '{:02x}'.format(now.hour)
    minute_hex = '{:02x}'.format(now.minute)
    second_hex = '{:02x}'.format(now.second)
    temp_hex = '{:02x}{:02x}'.format(temp_int, temp_frac)
    
    # Crée la trame complète
    frame = year_hex + month_hex + day_hex + hour_hex + minute_hex + second_hex + temp_hex + '00' * 9  # Remplir avec des zéros
    
    return frame


def decode_frame(hex_string):
    # Supprimer les accolades et le retour à la ligne
    hex_string = hex_string.replace("7b", "").replace("7d", "").replace("0d0a", "")
    
    # Convertir les hexa en ASCII
    ascii_values = [chr(int(hex_string[i:i+2], 16)) for i in range(0, len(hex_string), 2)]
    
    # Regrouper les caractères par deux
    hex_values = [''.join(ascii_values[i:i+2]) for i in range(0, len(ascii_values), 2)]
    
    # Convertir les hexa en decimal
    data_values = [int(hv, 16) for hv in hex_values]
    
    # Use a helper function to safely get data values with defaults
    def get_value(index, default=0):
        try:
            return data_values[index]
        except IndexError:
            return default

    year = get_value(0)
    month = get_value(1)
    day = get_value(2)
    hour = get_value(3)
    minute = get_value(4)
    second = get_value(5)
    temp_decimal = get_value(6)
    temp_fraction = get_value(7)
    error_type = get_value(8)
    command_decimal = get_value(9)
    command_fraction = get_value(10)

    # Return a dictionary of the values
    return {
        'year': year,
        'month': month,
        'day': day,
        'hour': hour,
        'minute': minute,
        'second': second,
        'temperature_decimal': temp_decimal,
        'temperature_fraction': temp_fraction,
        'error_type': error_type,
        'command_decimal': command_decimal,
        'command_fraction': command_fraction,
    }


if __name__ == "__main__" :
    frame = "180409141c3a16190200000000000000"
    print(len(frame))
    if len(frame) == 32:  # Expected length for 16 bytes (octet) of data
        decoded_values = decode_frame(frame)
        for key, value in decoded_values.items():
            print(f"{key}: {value}")
    else:
        print("Error: The frame is not the expected length.")
