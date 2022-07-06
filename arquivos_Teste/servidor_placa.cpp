#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include <stdio.h> // standard input / output functions
#include <string.h> // string function definitions
#include <stdint.h>
#include <unistd.h> // UNIX standard function definitions //write
#include <fcntl.h> // File control definitions
#include <errno.h> // Error number definitions
#include <termios.h> // POSIX terminal control definitionss
#include <time.h>   // time calls
#include <iostream>
#include <vector>


int socket_desc , new_socket , c;

int main(int argc , char *argv[]){
	
	struct sockaddr_in server , client;
	char message[]= "Hello Client , I have received your connection. But I have to go now, bye\n";

	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1) printf("Could not create socket");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(5560);
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) puts("bind failed");
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection

	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c))){
		puts("Connection accepted");
		
		// //Reply to the client
		// //write(new_socket , message , strlen(message));
		// ReadPack(rmsg);
		// for (int i = 0; i < 6; i++) {
		// 	cmd2[i]=rmsg[i];
		// }
		// SendPack(cmd_2);
			
	}

	if (new_socket<0){
		perror("accept failed");
		return 1;
	}

	return 0;
}



