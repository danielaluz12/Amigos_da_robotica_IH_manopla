from asyncio import open_connection
from codecs import charmap_build
from pickletools import uint8
from PyQt5 import QtWidgets, uic
import sys
import socket  
import numpy as np

#socket server - raspberry/toradex
#socket client - pc- onde tem interface

# Create a socket object
s = socket.socket()  

message =np.uint8([1,2,3])


class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi('/home/daniela/9 SEMESTRE/Embarcados/nossa interface/ui_main.ui', self)

        #button connect
        self.button = self.findChild(QtWidgets.QPushButton, 'connect_button') # Find the button
        self.button.clicked.connect(self.connectButtonPressed) # Remember to pass the definition/method, 
        #not the return value!

        #inputs connect
        self.input = self.findChild(QtWidgets.QLineEdit, 'ip_input')
        self.input2 = self.findChild(QtWidgets.QLineEdit, 'portsend_input')
        self.input3 = self.findChild(QtWidgets.QLineEdit, 'portreceive_input')
        
        #button send
        self.button2 = self.findChild(QtWidgets.QPushButton, 'send_button') # Find the button
        self.button2.clicked.connect(self.sendButtonPressed) # Remember to pass the definition/method, 
        #not the return value!

        #inputs connect
        self.input_pos = self.findChild(QtWidgets.QLineEdit, 'position_input')
        self.input_vel = self.findChild(QtWidgets.QLineEdit, 'velocity_input')
        self.input_tor = self.findChild(QtWidgets.QLineEdit, 'torque_input')

        #button receive
        self.button3 = self.findChild(QtWidgets.QPushButton, 'receive_button') # Find the button
        self.button3.clicked.connect(self.receiveButtonPressed) # Remember to pass the definition/method, 

        #lcd output connect
        self.output_pos = self.findChild(QtWidgets.QLCDNumber, 'position_receive')
        self.output_vel = self.findChild(QtWidgets.QLCDNumber, 'velocity_receive')
        self.output_torque= self.findChild(QtWidgets.QLCDNumber, 'torque_receive')

        self.show()

    

    def connectButtonPressed(self):
        # This is executed when the button is pressed
        #inputs from connect
        #print('Input IP:' + self.input.text())
        #print('Input send_port:' + self.input2.text())
        #print('Input receive_port:' + self.input3.text())
        #print('Input pos:' + self.input_pos.text())
        #print('Input veloc:' + self.input_vel.text())
        #print('Input torque:' + self.input_tor.text())

        #variables ip, port_send and port_receive

        ip_text = self.input.text() # can receive IP as text to use 
        print(' IP adress:',ip_text)
        port_send= int(self.input2.text())
        print(' port_send:', port_send)
        port_receive= int(self.input3.text())
        print(' port_receive:', port_receive)
        port = port_send   
        s.connect((ip_text, port))


    def sendButtonPressed(self):
        #variables pos, vel, torque 
        pos= float(self.input_pos.text())
        print(' pos:', pos)
        vel= float(self.input_vel.text())
        print(' veloc:', vel)
        torque= float(self.input_tor.text())
        print('torque:', torque)

        #palavra de 8 bits- 2 bytes
        #supondo  que fosse 125E    
        pos_send= str(pos)
        vel_send= str(vel)
        torque_send= str(torque)
        all_string = "{} {} {}".format(pos_send,vel_send,torque_send)
        #print(all_string)
        #all_string=pos_send+vel_send+torque_send
        s.send(all_string.encode())
    
    def receiveButtonPressed(self):

        msg = s.recv(1024)
  
        # repeat as long as message
        # string are not empty
       
        print('Received position/velocity/torque: ' + msg.decode())
        received_msg = msg.decode()
        split_msg= received_msg.split(" ", 2)
        #print(split_msg[0])

            # msg = s.recv(1024)
        
        self.output_pos.display(split_msg[0])
        self.output_vel.display(split_msg[1])
        self.output_torque.display(split_msg[2])

           
   
app = QtWidgets.QApplication(sys.argv)
window = Ui()
app.exec_()

