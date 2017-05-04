#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

/* Client Example */
int main(int argc, char *argv[]) 
{ 

    int clisock;  // client socket 
    int n;  // size of data 
    struct sockaddr_in remoteaddr;  // address of remote server 

    char buffer[256];  // buffer for data 

    // create client socket 
    if ( (clisock = socket(PF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket() error:\n"); 
        exit(1); 
    } 

    // set up address of remote server 
    remoteaddr.sin_family = PF_INET; 
    remoteaddr.sin_port = htons(atoi(argv[1])); //htons(5003); 
    remoteaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    // connect to socket with remote address 
    connect(clisock, (struct sockaddr *) &remoteaddr, sizeof(remoteaddr)); 

    while(1) { 
        // get user input 
        fgets(buffer,255,stdin); 
        // write input to socket 
        n = write(clisock,buffer,strlen(buffer));
        printf("strncmp:%d\n",strncmp(buffer,"quit",4)); 
        bzero(buffer,255);
        // get message from server 
        n = read(clisock,buffer,255); 
        printf("%s\n",buffer); 
        if (strncmp(buffer,"Quitting",4) == 0) break;
        bzero(buffer,255);
        
    }
    close(clisock);
    return 0; 
}
