#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;
//Lado do server
int main(int argc, char *argv[])
{
   
    //Número da porta
    int port = 5560;
    //Buffer para receber e enviar mensagens
    char msg[1500];
     
    //Faz o setup do socket e das ferramentas de conexão
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //Inicia o socket com o endereço da internet
    //Também acompanha o decriptor do socket
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
       printf("\n Error establishing the server socket \n");
        exit(0);
    }
    //Associa o socket ao seu endereço local
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        printf("\n Error binding socket to local address \n");
        exit(0);
    }
    printf("\n Waiting for a client to connect... \n");
    //Recebe no máximo 5 requests de uma vez
    listen(serverSd, 5);
    //Recebe um request do cliente
    //É necessário um novo endereço para conectar com o cliente
    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    //Cria um novo decriptor do socket para lidar com a nova conexão com o cliente
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if(newSd < 0)
    {
        printf("\n Error accepting request from client! \n" );
        exit(1);
    }
    printf( "\n Connected with client! \n");
   
    //Monitora a quantidade de dados recebidos e enviados
    int bytesRead, bytesWritten = 0;
    while(1)
    {
        //Recebe uma mensagem do cliente (listen)
        printf("\n Awaiting client response... \n");
        memset(&msg, 0, sizeof(msg));//clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            printf("\n Client has quit the session \n" );
            break;
        }
        printf("\n Position/velocity/torque: %s \n", msg);
        //Envia mensagens ao cliente
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }
    //Fecha o decriptor do socket depois que a operação acaba
    close(newSd);
    close(serverSd);
    printf( "\n ********Session******** \n" );
    printf("\n Bytes written: %d \n " ,bytesWritten);
    printf ("\n Bytes read: %d \n", bytesRead );
    printf("\n Connection closed... \n");
    return 0;   
}
