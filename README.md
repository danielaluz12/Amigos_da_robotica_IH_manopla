# Amigos_da_robotica_IH_manopla

Repositório do projeto de Sistemas embarcados - Primeiro semestre de 2022.

Projeto : IHM para braço robótico de reabilitação.


Daniela Ramos Luz 10883832 

## 1. TEMA

1.1 O projeto visa o aprendizado de implementação da conexão com uma interface para operação com envio de instruções de comando, recepção e apresentação de estado do equipamento.

A ideia é que a interface à qual o user tem acesso seja capaz de enviar comandos e receber estados de posição, velocidade e torque do motor da manopla robótica, abaixo segue um diagrama para melhor explicar o projeto e o fluxo:

![image](https://user-images.githubusercontent.com/71453516/177665929-06410537-126c-4525-b3f2-101160baf363.png)

Como pode ser visto acima a ideia que o usuário na interface gráfica rodando no Host consiga enviar comandos de posições e receber estados do motor da manopla. A comunicação entre o host com a interface gráfica e o sistema embarcado(toradéx) é feita através de um cabo ethernet com comunicação socket e protocolo de comunicação TCP-IP. A ideia é que no host,juntamente com a interface gráfica, rode um socket client para receber e enviar mensagens para o socket server que ficará na Toradéx. A toradéx além de comportar o server, se comunica com o motor da manopla por comunicação, enviando os comandos de posição para o motor e recebendo os estados do enconder.

## 2. CONFECÇÃO

# FRONT-END DA INTERFACE

Primeiramente podemos entender um poquinho sobre a interface gráfica que pode ser vista abaixo:

![image](https://user-images.githubusercontent.com/71453516/177666702-103cbffb-bc77-4fb0-ab36-21fd07732e08.png)

A interface foi construída usando o Qt Designer para o criar o arquivo interface.ui que foi construído utilizando elementos do Qt Designer que facilitam integraçaõ com python para envio e recebimento de dados no back-end.

Alguns exemplos:

# QlineEdit: permite recebimento de informação a partir campo

![image](https://user-images.githubusercontent.com/71453516/177667333-d0fa60b2-4c9d-4dcf-99c1-e3491919f0a5.png)

# QPushButton: literalmente um botão de estado para triggar um evento

![image](https://user-images.githubusercontent.com/71453516/177667431-c9bffbcb-8a5d-4584-a3b3-646ade44b8ec.png)

# QLCDNumber: display para mostrar informação recebida

![image](https://user-images.githubusercontent.com/71453516/177667558-182bedc7-c09f-40bf-a030-cb9d1cfd6e2a.png)

O Qt Designer utiliza linguagem html para criar um arquivo de descrição da MainWindows da interface com todos os elementos presentes, seus nomes, tipos de elemento do Qt Designer,suas cores e outras propriedades como cores e tamanho como pode ser visto no snippet do arquivo Interface.ui abaixo :

```bash
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
 <class>MainWindow</class>
 <widget class="QMainWindow" name="MainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>1102</width>
    <height>685</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>MainWindow</string>
  </property>
  <widget class="QWidget" name="centralwidget">
   <property name="styleSheet">
    <string notr="true">background:rgb(91,90,90);</string>
   </property>
   <layout class="QVBoxLayout" name="verticalLayout">
    <property name="spacing">
     <number>0</number>
    </property>
    <property name="leftMargin">
     <number>0</number>
    </property>
    <property name="topMargin">
     <number>0</number>
    </property>
    <property name="rightMargin">
     <number>0</number>
    </property>
    <property name="bottomMargin">
     <number>0</number>
    </property>
    <item>
```

Observe que há uma Main Window abaixo da qual ficam todos os widgets com suas propriedades e atributos.

# BACK-END DA INTERFACE

Para confeccionar o a lógica por trás da interface foi usado python e a biblioteca PyQt5 que facilita imensamente a utilização dos widgets padrão do QT Designer que foram mencionados anteriormente.

Abaixo seguem alguns de como é feita a inicialização e utilização dos widgets usando a  PyQt5.

Primeira é criada uma classe que contará com o objetos de cada tipo de widget

```bash
class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__()
        uic.loadUi('/home/daniela/9 SEMESTRE/Embarcados/nossa interface/ui_main.ui', self)
```
Uma vez feito isso os widgets são inicializados conforme seus nomes descritos no interface.ui:

```bash
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
    ```
    Para receber informação inputada na tela:
    
    ```bash
    ip_text = self.input.text()
    ```
    Para imprmir informação na tela:
    
    ```bash
    self.output_pos.display(split_msg[0])
    ```
    
    Botão para triggar eventos:
    
    ```bash
     self.button2 = self.findChild(QtWidgets.QPushButton, 'send_button') # Find the button
     self.button2.clicked.connect(self.sendButtonPressed) # Remember to pass the definition/method
     
     #triggers makes what the function commands
     def sendButtonPressed(self):
    ```
    
    Usando essas três funcionalidades do biblioteca fizemos a conexão da interface com as mensagens enviadas/recebidas por conexão socket.
    
# COMUNICAÇÃO SOCKET


    
## Testes e adpatações

## Arquivos finais e funcionaus
