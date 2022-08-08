// Amigos_da_robotica_IH_manopla
// Repositório do projeto de Sistemas embarcados - Primeiro semestre de 2022.
// Projeto: IHM para braço robótico de reabilitação.
// Componentes do grupo:
// Daniela Ramos Luz 10883832
// Lucas Fiorotti Valim 10746686
// Heloísa Vargas Megda de Oliveira 9912869

from asyncio import open_connection
from codecs import charmap_build
from pickletools import uint8
from PyQt5 import QtWidgets, uic
import sys
import socket  
import numpy as np

#Socket server - Raspberry/Toradex
#Socket client - Local onde a interface está sendo utlizada

# Cria um socket
s = socket.socket()  

message =np.uint8([1,2,3])


class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi('/home/daniela/9 SEMESTRE/Embarcados/nossa interface/ui_main.ui', self)
        
        #Realiza a integração entre a interface e os servidores
        #É feita para cada um dos elementos descritos abaixo
        
        #button connect
        self.button = self.findChild(QtWidgets.QPushButton, 'connect_button')
        self.button.clicked.connect(self.connectButtonPressed)

        #inputs connect
        self.input = self.findChild(QtWidgets.QLineEdit, 'ip_input')
        self.input2 = self.findChild(QtWidgets.QLineEdit, 'portsend_input')
        self.input3 = self.findChild(QtWidgets.QLineEdit, 'portreceive_input')
        
        #button send
        self.button2 = self.findChild(QtWidgets.QPushButton, 'send_button') 
        self.button2.clicked.connect(self.sendButtonPressed) 
        
        #inputs connect
        self.input_pos = self.findChild(QtWidgets.QLineEdit, 'position_input')
        self.input_vel = self.findChild(QtWidgets.QLineEdit, 'velocity_input')
        self.input_tor = self.findChild(QtWidgets.QLineEdit, 'torque_input')

        #button receive
        self.button3 = self.findChild(QtWidgets.QPushButton, 'receive_button') 
        self.button3.clicked.connect(self.receiveButtonPressed) 

        #lcd output connect
        self.output_pos = self.findChild(QtWidgets.QLCDNumber, 'position_receive')
        self.output_vel = self.findChild(QtWidgets.QLCDNumber, 'velocity_receive')
        self.output_torque= self.findChild(QtWidgets.QLCDNumber, 'torque_receive')

        self.show()

    
    #Processo quando o botão connect é apertado
    def connectButtonPressed(self):
        #Atribui os valores de ip, da porta de envio e da porta de recebimento
        #Os valores também são printados
        ip_text = self.input.text() 
        print(' IP adress: ',ip_text)
        port_send= int(self.input2.text())
        print(' port_send: ', port_send)
        port_receive= int(self.input3.text())
        print(' port_receive: ', port_receive)
        port = port_send   
        s.connect((ip_text, port))

    #Processo quando o send conexão é apertado
    def sendButtonPressed(self):
        #Atribui os valores de posição, de velocidade e de torque
        #Os valores também são printados
        pos= float(self.input_pos.text())
        print(' pos:', pos)
        vel= float(self.input_vel.text())
        print(' veloc:', vel)
        torque= float(self.input_tor.text())
        print('torque:', torque)

        #Para uma palavra de 8 bits = 2 bytes
        #Supondo  que fosse 125E    
        pos_send= str(pos)
        vel_send= str(vel)
        torque_send= str(torque)
        all_string = "{} {} {}".format(pos_send,vel_send,torque_send)
        #print(all_string)
        #all_string=pos_send+vel_send+torque_send
        s.send(all_string.encode())
    
    
    #Processo quando o receive conexão é apertado
    def receiveButtonPressed(self):
        msg = s.recv(1024)
        print('Received position/velocity/torque: ' + msg.decode())
        received_msg = msg.decode()
        split_msg= received_msg.split(" ", 2)     
        self.output_pos.display(split_msg[0])
        self.output_vel.display(split_msg[1])
        self.output_torque.display(split_msg[2])

           
   
app = QtWidgets.QApplication(sys.argv)
window = Ui()
app.exec_()

