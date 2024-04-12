import threading
from PyQt5.QtWidgets import QApplication
import sys

from src.ihm_simulate import LoginWindow, apply_stylesheet, load_credentials
from src.simulate_serial import send_predefined_frame

def main():
    app = QApplication([])                  # Instanciation de l'interface
    apply_stylesheet(app)                   # Appel du style
    credentials = load_credentials()        # Appel des logins
    login_window = LoginWindow(credentials) # Appel de la 1ere page
    login_window.show()                     # Affichage Actif
    
    simulation_thread = threading.Thread(target=send_predefined_frame, args=('COM8', 9600, 2))
    simulation_thread.daemon = True  # Cela permettra au thread de se fermer avec le programme.
    simulation_thread.start()
    
    sys.exit(app.exec_()) 


if __name__ == '__main__':
    main()
