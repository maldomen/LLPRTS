#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "sensors.h"

/* init sensors creara els threads que atenen els sockets dels diferents sensors 
*/


int create_socket(int *socket_fd,int port,char *nom){
    int server_fd,valread;
    struct sockaddr_in adress_in;
    int addrlenin = sizeof(adress_in);
    char buffer[256];

    // Creating socket file descriptor
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))
        == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

 
    adress_in.sin_family = AF_INET;
    adress_in.sin_addr.s_addr = INADDR_ANY;
    adress_in.sin_port = htons(port);
    // Forcefully attaching socket to the port pin
    if (bind(server_fd, (struct sockaddr*)&adress_in,
             sizeof(adress_in))
        < 0) {
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    
    
    if ((*socket_fd= accept(server_fd, (struct sockaddr*)&adress_in,(socklen_t*)&addrlenin)) <0)  {
        printf("accept\n");
        exit(EXIT_FAILURE);
    }
    
    printf("%s conectat a control\n", nom);

    }

 
int init_sensors()
{
    return 0;
}

