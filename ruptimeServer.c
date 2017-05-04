#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

/* Server Example */
int main(int argc, char *argv[]) 
{ 
    int sersock, consock; // socket file descriptors 
    char buffer[256]; // buffer for transfer 
    struct sockaddr_in serveraddr, clientaddr; // sockaddr structs 
    int len = sizeof(clientaddr);  // length of client address. 
    int n;  // size of data transmitted 
    char yes = '1';

    // try to open socket 
    if ( (sersock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket() error:\n"); 
        exit(1); 
    }

    if ( setsockopt(sersock, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) < 0) {
	perror("setsockopt() error:\n");
	exit(1);
    }
   
    // set address information 
    serveraddr.sin_family = PF_INET; 
    serveraddr.sin_port = htons(atoi(argv[1])); 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 

    // bind socket to address 
    if ( ( bind(sersock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) ) { 
        perror("bind() error:\n"); 
        exit(1); 
    } 

    // begin listening for connections on socket 
    listen(sersock, 10); 

    // process requests 
    while(1) { 
        // wait for a connection to arrive and process it 
        consock = accept(sersock, (struct sockaddr *) &clientaddr, &len); 

        while(1) {
	        // read from client 
	        n = read(consock, buffer, 255); 

	        printf("Received message: %s\n",buffer); 
	        
	        if (strncmp(buffer,"quit",4) == 0) {
	        	n = write(consock, "Quitting",8);
	        	close(consock);  // close socket 
	        	break;
	        }
	        bzero(buffer,255);
	        //write to client 
	        n = write(consock, "Message Received.",17);
        }

    } 
    close(sersock); 
    return 0; 
}
