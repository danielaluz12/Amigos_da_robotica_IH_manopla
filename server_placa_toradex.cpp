// #include <iostream>
// #include <string>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <netdb.h>
// #include <sys/uio.h>
// #include <sys/time.h>
// #include <sys/wait.h>
// #include <fcntl.h>
// #include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;
//Server side
int main(int argc, char *argv[])
{
   
    //grab the port number
    int port = 5560;
    //buffer to send and receive messages with
    char msg[1500];
     
    //setup a socket and connection tools
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    //open stream oriented socket with internet address
    //also keep track of the socket descriptor
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSd < 0)
    {
       printf("\n Error establishing the server socket \n");
        exit(0);
    }
    //bind the socket to its local address
    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, 
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        printf("\n Error binding socket to local address \n");
        exit(0);
    }
   printf("\n Waiting for a client to connect... \n");
    //listen for up to 5 requests at a time
    listen(serverSd, 5);
    //receive a request from client using accept
    //we need a new address to connect with the client
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
   
    //also keep track of the amount of data sent as well
    int bytesRead, bytesWritten = 0;
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
        //cout << ">";
        //string data;
        //getline(cin, data);
        //memset(&msg, 0, sizeof(msg)); //clear the buffer
        //strcpy(msg, data.c_str());
        //if(data == "exit")
        //{
            //send to the client that server has closed the connection
           // send(newSd, (char*)&msg, strlen(msg), 0);
           // break;
       // }
        //send the message to client
        bytesWritten += send(newSd, (char*)&msg, strlen(msg), 0);
    }
    //we need to close the socket descriptors after we're all done
    close(newSd);
    close(serverSd);
    printf( "\n ********Session******** \n" );
    printf("\n Bytes written: %d \n " ,bytesWritten);
    printf ("\n Bytes read: %d \n", bytesRead );
    printf("\n Connection closed... \n");
    return 0;   
}
