# Amigos_da_robotica_IH_manopla

Repositório do projeto de Sistemas embarcados - Primeiro semestre de 2022.

Projeto : IHM para braço robótico de reabilitação.


Daniela Ramos Luz 10883832 

Lucas Fiorotti Valim 10746686

Heloísa Vargas Megda de Oliveira 9912869

Heloisa Vargas

## 1. TEMA

1.1 O projeto visa o aprendizado de implementação da conexão com uma interface para operação com envio de instruções de comando, recepção e apresentação de estado do equipamento.

A ideia é que a interface à qual o user tem acesso seja capaz de enviar comandos e receber estados de posição, velocidade e torque do motor da manopla robótica, abaixo segue um diagrama para melhor explicar o projeto e o fluxo:

![image](https://user-images.githubusercontent.com/71453516/177665929-06410537-126c-4525-b3f2-101160baf363.png)

Como pode ser visto acima a ideia que o usuário na interface gráfica rodando no Host consiga enviar comandos de posições e receber estados do motor da manopla. A comunicação entre o host com a interface gráfica e o sistema embarcado(toradéx) é feita através de um cabo ethernet com comunicação socket e protocolo de comunicação TCP-IP. A ideia é que no host,juntamente com a interface gráfica, rode um socket client para receber e enviar mensagens para o socket server que ficará na Toradéx. A toradéx além de comportar o server, se comunica com o motor da manopla por comunicação, enviando os comandos de posição para o motor e recebendo os estados do enconder.

## 2. CONFECÇÃO

## FRONT-END DA INTERFACE

Primeiramente podemos entender um poUquinho sobre a interface gráfica que pode ser vista abaixo:

![image](https://user-images.githubusercontent.com/71453516/177666702-103cbffb-bc77-4fb0-ab36-21fd07732e08.png)

A interface foi construída usando o Qt Designer para o criar o arquivo 'interface.ui' que foi construído utilizando elementos do Qt Designer que facilitam integração com python para envio e recebimento de dados no back-end.

Alguns exemplos:

## QlineEdit: input de informação do user

![image](https://user-images.githubusercontent.com/71453516/177667333-d0fa60b2-4c9d-4dcf-99c1-e3491919f0a5.png)

## QPushButton: literalmente um botão de estado para triggar um evento

![image](https://user-images.githubusercontent.com/71453516/177667431-c9bffbcb-8a5d-4584-a3b3-646ade44b8ec.png)

## QLCDNumber: display para mostrar informação recebida

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

## BACK-END DA INTERFACE

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
    
## COMUNICAÇÃO SOCKET

Para comunicação socket o cliente foi implementado em python usando para poder ser implementado no mesmo script da interface principal main_window_client.py, um snippet pode ser visto abaixo:

```bash

  ip_text = self.input.text() # can receive IP as text to use 
        print(' IP adress:',ip_text)
        port_send= int(self.input2.text())
        print(' port_send:', port_send)
        port_receive= int(self.input3.text())
        print(' port_receive:', port_receive)
        port = port_send   
        s.connect((ip_text, port))

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
```

O código acima basicamente recebe IP e porta da interface e para fazer o conect do client com o host.

Para o server a ideia também era fazer a programaçõe em Python, mas devidos a questões de cross-compile foi utilizado c++, um snippet do código do servidor pode ser visto abaixo:

Bind do servidor socket:

```bash
  int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        printf("\n Error binding socket to local address \n");
        exit(0);
    }
   printf("\n Waiting for a client to connect... \n");
```

Request do client aceito pelo servidor:
```bash
sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //accept, create a new socket descriptor to 
    //handle the new connection with client
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        printf("\n Error accepting request from client! \n" );
        exit(1);
    }
    printf( "\n Connected with client! \n");
```
Recebimento de mensagem do client pelo servidor e reenvio para o client:
```bash
while(1)
    {
        //receive a message from the client (listen)
        printf("\n Awaiting client response... \n");
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            printf("\n Client has quit the session \n" );
            break;
        }
        printf("\n Position/velocity/torque: %s \n", msg);
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }
```
    
## Testes e adpatações

Como foi comentado acima adpatações foram feitas para que o client pudesse rodar em python enquanto o server rodasse em c++, devido a questões de Cross-compile na toradéx.

Além disso, alguns includes de bilbiotecas para print em c++ foram alteradas para as padrões de C devido a problemas de compilação obtidos durantes os testes de cross-compile com o toolchain da toradéx. Uma vez feito isso, o cross-compile do servidor 'servidor_placa_toradex' foi bem sucedido como pode ser visto abaixo:

![image](https://user-images.githubusercontent.com/71453516/177673008-f8280149-71c3-439f-bdf5-8ca6462a2589.png)

Depois do cross-compile, enviamos o arquivo via scpy e testamos rodar o servidor na toradéx como segue:

![image](https://user-images.githubusercontent.com/71453516/177673313-fff2fef7-0b02-4841-b02e-af53c805b18e.png)

Depois disso inputamos o IP e porta de comunicação do client com o server na interface e clicamos no botão conect

![image](https://user-images.githubusercontent.com/71453516/177673659-2e1cec4f-f9bd-42b9-990c-8493b169b368.png)

Isso gerou a seguinte mensagem no server na toradéx:

![image](https://user-images.githubusercontent.com/71453516/177673862-a3c6072d-c3ac-48a3-b034-24bb0375df28.png)

Uma vez que o client estava conectado com o server, testamos enviar comandos de posição pela interface:

![image](https://user-images.githubusercontent.com/71453516/177674037-be85ad04-187f-4399-b04c-103cf0bb7253.png)

Que foram enviados pelo client, e recebidos da seguinte forma pelo server:

![image](https://user-images.githubusercontent.com/71453516/177674208-1614c0cf-9053-47d9-bd34-4e5fc264f082.png)

Uma vez recebidos pelo server, foram reenviados para o client e mostrados na interface:

![image](https://user-images.githubusercontent.com/71453516/177674456-8bc3acc1-d2ca-49d0-b798-4f17d1fb626b.png)

Assim conlui-se o teste de que o projeto funciona e os arquivos finais e funcionais encontram-se na pasta arquivos_finais.
