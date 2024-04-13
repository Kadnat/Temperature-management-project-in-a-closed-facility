import threading
from PyQt5.QtWidgets import QApplication, QMainWindow, QPushButton, QHBoxLayout, QVBoxLayout, QFrame, QWidget, QLabel, QLineEdit, QMessageBox, QSpacerItem, QSizePolicy, QTextEdit
from PyQt5.QtGui import QIcon, QTextCursor
from PyQt5.QtCore import Qt
from src.communication import SerialWorker, decode_frame
import json
import time
import subprocess
import serial
import threading
from functools import partial
from src.sendcommands import Commands

from src.simulate_serial import update_current_frame

def apply_stylesheet(app):
    stylesheet = ("""
        QMainWindow {
                background-color: #34495e;
            }
            QLabel {
                font-size: 16px;
                color: #ecf0f1;
                padding-top: 50px;
                margin: 0px;
            }
            QLineEdit {
                border: 2px solid #2980b9;
                border-radius: 10px;
                padding: 5px;
                font-size: 14px;
                color: #ecf0f1;
                background: #2c3e50;
                margin-bottom: 15px;
            }
            QPushButton {
                background-color: #2980b9;
                border-style: none;
                padding: 10px;
                margin-top: 50px;
                border-radius: 5px;
                color: #ecf0f1;
                font-size: 16px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #3498db;
            }
        """)
    app.setStyleSheet(stylesheet)
    
# Chargement des données d'authentification à partir d'un fichier JSON
def load_credentials():
    with open('./src/data/credentials.json', 'r') as file:
        return json.load(file)

# Classe pour la fenêtre de connexion
class LoginWindow(QMainWindow):
    def __init__(self, credentials):
        super().__init__()
        self.credentials = credentials
        self.init_ui()

    def init_ui(self):
        # Configuration de style et de fenêtre
        self.setWindowIcon(QIcon('./src/img/Icone.ico'))
        self.setWindowTitle('Log In - Thermal Station Controller')
        self.resize(900, 680)

        layout = QVBoxLayout()
        layout.setSpacing(10)  # Espace entre les widgets
        layout.setContentsMargins(50, 20, 50, 100)  # Gauch  e/Haut/Droite/Bas

        label_username = QLabel('Nom d’utilisateur:')
        self.username_input = QLineEdit(self)
        label_password = QLabel('Mot de passe:')
        self.password_input = QLineEdit(self)
        self.password_input.setEchoMode(QLineEdit.Password)
        self.login_button = QPushButton('Se connecter', self)
        self.login_button.clicked.connect(self.check_credentials)

        layout.addWidget(label_username)
        layout.addWidget(self.username_input)
        layout.addWidget(label_password)
        layout.addWidget(self.password_input)
        layout.addWidget(self.login_button)

        central_widget = QWidget()
        central_widget.setLayout(layout)
        self.setCentralWidget(central_widget)

    def check_credentials(self):
        # Récupération des identifiants saisis
        username = self.username_input.text()
        password = self.password_input.text()
        
        # Vérification des identifiants
        for user in self.credentials['users']:
            if username.lower() == user['username'].lower() and password == user['password']:
                self.accept_login(username, int(user['level']))
                return
        QMessageBox.warning(self, 'Erreur', 'Identifiants incorrects')

    def accept_login(self, username, access_level):
        # Accès autorisé, ouverture de la fenêtre principale
        self.main_window = MainWindow(username, access_level)
        self.main_window.show()
        self.close()

# Classe pour la fenêtre principale
class MainWindow(QMainWindow):
    def __init__(self, username, access_level):
        super().__init__()
        self.username = username
        self.access_level = access_level
        self.init_ui()
        self.log_entries = []  # Initialize an empty list to hold the log entries.
        self.max_log_entries = 20  # Set a maximum for the log entries.
        self.serial_worker = SerialWorker('COM6')
        self.serial_worker.data_received.connect(self.display_log)
        self.serial_worker.data_received.connect(self.display_temperature)
        self.serial_worker.start()
        self.commands = Commands()

    def init_ui(self):
        # Configuration de la fenêtre
        self.setWindowIcon(QIcon('./src/img/Icone.ico'))
        self.setWindowTitle('Home - Thermal Station Controller')
        self.resize(900, 680)
        
       # Layout vertical principal
        main_vertical_layout = QVBoxLayout()

        # Création du cadre et du layout pour le message de bienvenue
        welcome_frame = QFrame()
        welcome_frame.setFrameShape(QFrame.StyledPanel)
        welcome_frame.setFrameShadow(QFrame.Raised)
        welcome_frame.setFixedHeight(150)  # La hauteur en pixels
        welcome_layout = QHBoxLayout()
        welcome_label = QLabel(f'Bienvenue {self.username} !')
        welcome_label.setAlignment(Qt.AlignCenter)
        welcome_layout.addWidget(welcome_label)
        welcome_frame.setLayout(welcome_layout)

        # Stylisation du cadre de bienvenue
        welcome_frame.setStyleSheet("""
            background-color: #5e4934;
            color: #FFFFFF;
            font-size: 32px;
            padding: 0px;
            margin-bottom: 20px;
            height: 50px;
        """)

        # Ajout du cadre de bienvenue au layout principal
        main_vertical_layout.addWidget(welcome_frame)

        # Création du cadre pour les layouts des colonnes
        columns_frame = QFrame()
        columns_layout = QHBoxLayout()

        # Création et stylisation des cadres individuels pour chaque colonne
        left_frame, center_frame = QFrame(), QFrame()
        for frame in (left_frame, center_frame):
            frame.setFrameShape(QFrame.StyledPanel)
            frame.setFrameShadow(QFrame.Raised)
            frame.setStyleSheet("background-color: #4d6072;")  # Un style de base pour exemple

        # Création des layouts de colonne et ajout des widgets
        left_layout = QVBoxLayout()
        center_layout = QVBoxLayout()
        # right_layout = QVBoxLayout()

        
        # |------------------------------------------|
        # |----------------- RIGHT ------------------|
        # |------------------------------------------|
        
        # Frame
        right_frame = QFrame()
        right_frame.setStyleSheet("""
                                    background-color: #4d6072;
                                    padding: 10px;
                                    margin: 0px;
                                """)
        right_frame.setFixedWidth(250)  # Définissez la largeur que vous souhaitez
        
        # Add To Layout
        right_layout = QVBoxLayout(right_frame)
        
        right_layout.setAlignment(Qt.AlignCenter)
        

        if self.access_level >= 2 :
            # Title
            right_title = QLabel('Logs :')
            
            # Logs
            self.right_content = QTextEdit()
            self.right_content.setReadOnly(True)  # Rendre le widget en lecture seule
            self.right_content.setStyleSheet("""
                background-color: #2e2e2e;
                color: white;
                font-family: 'Consolas';
                border: none;
                padding: 5px;
            """)
            self.right_content.setFrameShape(QFrame.NoFrame)  # Pour enlever le cadre par défaut
            self.right_content.setFixedWidth(200)  # Limitez la largeur du QTextEdit à 300 pixels
            self.right_content.setMaximumHeight(600)
            self.right_content.setAlignment(Qt.AlignCenter)

            # Entrée pour définir une nouvelle température
            self.nbDay = QLineEdit()
            self.nbDay.setPlaceholderText('Entrez le nombre de jour a récupérer :')
            
            # Bouton pour envoyer la nouvelle température
            self.getHistory = QPushButton('Recuperer historique')
            self.getHistory.clicked.connect(self.send_history)
            
        else :
            # Title
            right_title = QLabel('! Logs Indisponibles !')
            # Content
            right_content = QTextEdit('Seuls les utilisateurs de niveau 2 ou plus peuvent voir les logs de températures')
            right_content.setFrameShape(QFrame.NoFrame)
            right_content.setAlignment(Qt.AlignCenter)
            right_content.setMaximumHeight(200)
            right_content.setReadOnly(True)
            right_content.setStyleSheet("""
                background-color: #2e2e2e;
                color: white;
                font-family: 'Consolas';
                border: none;
                padding: 5px;
                margin-top: 50px;
            """)
        
        # Display Title
        right_title.setAlignment(Qt.AlignCenter | Qt.AlignTop)  # Centré horizontalement, aligné en haut
        right_title.setStyleSheet("""font-weight: 700;""")
        right_layout.addWidget(right_title)
        
        # Display content
        if self.access_level >= 2 :
            right_layout.addWidget(self.right_content)
            right_layout.addWidget(self.nbDay)
            right_layout.addWidget(self.getHistory)

        else :
            right_layout.addWidget(right_content)
        
        right_frame.setLayout(right_layout)
        
        # |------------------------------------------|
        # |---------------- CENTER ------------------|
        # |------------------------------------------|
        
        center_frame = QFrame()
        center_frame.setStyleSheet("""
                                    background-color: #4d6072;
                                    padding: 10px;
                                    margin: 0px;
                                  """)
        center_layout = QVBoxLayout(center_frame)
        
        # Titre
        self.title_label = QLabel('Température')
        self.title_label.setAlignment(Qt.AlignCenter)

        # Affichage de la température actuelle
        self.current_temp_label = QLabel('Température actuelle: --°C')
        self.current_temp_label.setAlignment(Qt.AlignCenter)

        if self.access_level >= 3:
            # Entrée pour définir une nouvelle température
            self.temp_input = QLineEdit()
            self.temp_input.setPlaceholderText('Entrer nouvelle température...')
            
            # Bouton pour envoyer la nouvelle température
            self.set_temp_button = QPushButton('Définir température')
            self.set_temp_button.clicked.connect(self.send_temperature)
            
        else:
            # Titre indiquant que le contrôle est indisponible
            center_title = QLabel('! Contrôle Indisponible !')
            center_title.setAlignment(Qt.AlignCenter)
            center_layout.addWidget(center_title)
            
            # Message indiquant que seuls les utilisateurs de niveau 3 peuvent contrôler la température
            center_content = QTextEdit('Seuls les utilisateurs de niveau 3 ou plus peuvent contrôler la température')
            center_content.setFrameShape(QFrame.NoFrame)
            center_content.setAlignment(Qt.AlignCenter)
            center_content.setMaximumHeight(200)
            center_content.setReadOnly(True)
            center_content.setStyleSheet("""
                background-color: #2e2e2e;
                color: white;
                font-family: 'Consolas';
                border: none;
                padding: 5px;
                margin-top: 50px;
            """)
            

        center_layout.addWidget(self.title_label)
        center_layout.addWidget(self.current_temp_label)
        
        if self.access_level >= 3 :
            center_layout.addWidget(self.temp_input)
            center_layout.addWidget(self.set_temp_button)
        else :
            center_layout.addWidget(center_content)

        self.setCentralWidget(center_frame)
        
        # |------------------------------------------|
        # |----------------- LEFT -------------------|
        # |------------------------------------------|
        
        # Frame
        left_frame = QFrame()
        left_frame.setStyleSheet("""
                                    background-color: #4d6072;
                                    padding: 10px;
                                    margin: 0px;
                                """)
        left_frame.setFixedWidth(250)  # Définissez la largeur que vous souhaitez
        
        # Titre
        self.title_label = QLabel('Vos droits :')
        self.title_label.setAlignment(Qt.AlignCenter)

        # Layout pour le contenu de gauche
        left_layout = QVBoxLayout()
        left_layout.addWidget(self.title_label)
        
        # Ajouter le contenu basé sur le niveau d'accès
        if self.access_level >= 1:
            # Tous les utilisateurs ont accès à la température actuelle
            self.temperature_label = QLabel('Lecture de la température')
            self.temperature_label.setAlignment(Qt.AlignCenter)
            left_layout.addWidget(self.temperature_label)

        if self.access_level >= 2:
            # Ajouter d'autres fonctionnalités pour les utilisateurs de niveau 2 et plus
            self.log_access_label = QLabel('Accès aux logs')
            self.log_access_label.setAlignment(Qt.AlignCenter)
            left_layout.addWidget(self.log_access_label)

        if self.access_level == 3:
            # Fonctionnalités exclusives pour les utilisateurs de niveau 3
            self.temperature_control_label = QLabel('Contrôle de la température')
            self.temperature_control_label.setAlignment(Qt.AlignCenter)
            left_layout.addWidget(self.temperature_control_label)
        
        left_frame.setLayout(left_layout)

        # Ajout des cadres des colonnes au layout des colonnes
        columns_layout.addWidget(left_frame)
        columns_layout.addWidget(center_frame)
        columns_layout.addWidget(right_frame)

        # Stylisation et ajout du cadre des colonnes au layout principal
        columns_frame.setLayout(columns_layout)
        main_vertical_layout.addWidget(columns_frame)

        # Widget central et définition du layout principal
        central_widget = QWidget()
        central_widget.setLayout(main_vertical_layout)
        self.setCentralWidget(central_widget)

    def display_log(self, data):
        if self.access_level >= 2 :
            # Same as before: decode the incoming data and create the log entry.
            decoded_values = decode_frame(data)
            print(decoded_values)
            temperature = f"{decoded_values['temperature_decimal']}.{decoded_values['temperature_fraction']}°C"
            log_entry = f"[{decoded_values['hour']}:{decoded_values['minute']}:{decoded_values['second']}] T = {temperature}"

            if len(self.log_entries) >= self.max_log_entries:
                # Supprimez le premier log pour faire de la place au nouveau.
                self.right_content.moveCursor(QTextCursor.Start)
                self.right_content.moveCursor(QTextCursor.Down, QTextCursor.KeepAnchor)
                self.right_content.textCursor().removeSelectedText()
                self.right_content.textCursor().deleteChar()  # Supprimez le caractère de nouvelle ligne restant.
            
            self.right_content.append(log_entry)  # Ajoutez le nouveau log à la fin.
            self.log_entries.append(log_entry)  # Ajoutez également à la liste des logs.
            if len(self.log_entries) > self.max_log_entries:
                self.log_entries.pop(0)  # Gardez la taille de la liste contrôlée.

    def update_current_temperature(self, temperature):
        self.current_temp_label.setText(f'Température actuelle: {temperature}°C')


    def send_temperature(self):
        temperature_str = self.temp_input.text()  # Remplacez ceci par la température que vous souhaitez envoyer
        self.commands.send_temp(temperature_str)

    def send_history(self):
        history_str = self.nbDay.text()
        self.commands.send_hist(history_str)
        
            
    def display_temperature(self, data):
        decoded_values = decode_frame(data)
        temperature = f"{decoded_values['temperature_decimal']}.{decoded_values['temperature_fraction']}°C"
        self.current_temp_label.setText(f"Température actuelle : {temperature}")

    
    # Assurez-vous d'arrêter le thread lorsque la fenêtre est fermée.
    def closeEvent(self, event):
        self.serial_worker.stop()
        super().closeEvent(event)
    
if __name__ == '__main__':
    
    app = QApplication([])                  # Instanciation de l'interface
    apply_stylesheet(app)                   # Appel du style
    credentials = load_credentials()        # Appel des logins
    login_window = LoginWindow(credentials) # Appel de la 1ere page
    login_window.show()                     # Affichage Actif
    app.exec_()                             # Execution
