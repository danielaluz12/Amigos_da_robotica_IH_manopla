// Amigos_da_robotica_IH_manopla
// Repositório do projeto de Sistemas embarcados - Primeiro semestre de 2022.
// Projeto: IHM para braço robótico de reabilitação.
// Componentes do grupo:
// Daniela Ramos Luz 10883832
// Lucas Fiorotti Valim 10746686
// Heloísa Vargas Megda de Oliveira 9912869

#include<sys/socket.h>
#include<arpa/inet.h>	
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h> 
#include <time.h>   
#include <iostream>
#include <vector>


int socket_desc , new_socket , c;

int main(int argc , char *argv[]){
	
	struct sockaddr_in server , client;
	char message[]= "Hello Client , I have received your connection. But I have to go now, bye\n";

	//Cria o socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
	
	//Prepara a estrutura do sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5560);
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) puts("bind failed");
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept e incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
		puts("Connection accepted");		
	}
	if (new_socket<0){
		perror("accept failed");
		return 1;
	}
	return 0;
}
