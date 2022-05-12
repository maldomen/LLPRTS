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
#include <fcntl.h>

#include "os.h"
#include "architecture.h"
#include "control.h"
#include "sensors.h"
#include "actuators.h"


int connect_socket(int port, int *fd){
struct sockaddr_in serv_addr;
int try=0;
if ((*fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("\n Socket creation error \n");
    return -1;
}

serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port);
// Convert IPv4 and IPv6 addresses from text to binary form
if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){

    printf("\nInvalid address/ Address not supported \n");
    return -1;
}
while ((connect(*fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)&&(try<5)){
    try++;
    usleep(500000);
}
if (try==5) return -1;

return 0;
}

int connect_pipe(char *pipe_name, int *fd_read, int *fd_write){

char channel_read[256];
char channel_write[256];
int try;
sprintf(channel_read, "%s_read", pipe_name);
sprintf(channel_write, "%s_write", pipe_name);
try=0;

while (((*fd_read = open(channel_read, O_RDONLY))<0)&&(try<5)){
    usleep(500000);
    try++;
}
if (*fd_read<0) return -1;
try=0;
while (((*fd_write = open(channel_write, O_RDWR|O_NONBLOCK))<0)&&(try<5)){
    usleep(500000);
    try++;
}
if (*fd_write<0) return -1;
return 0;
}

int read_pipe_active_float(int fd, float *value){
read(fd, value, sizeof(float));
return 0;
}

int read_socket_passive_float(int fd, float *value){
char c=0;
send(fd, (char*)&c, sizeof(char),0);
recv(fd, value, sizeof(float),0);
return 0;
}

int send_action(int fd, char*message){
    char c=0;
    int length = strlen(message)+1;
    send(fd, (char*)&length, sizeof(int), 0);
    send(fd, message, length, 0);


}



static float speed=0.0;
void task_read_speed(void *param) {
read_pipe_active_float( sensors[0].fd,&speed);
}
/*
void task_motor(void *param){
    send_action(actuators[0].fd,"start");
}*/
void task_w(void *param){
    char c;
   
    while(1) {
        
        c=getchar();
        if(c=='w'){
            send_action(actuators[0].fd,"start");
            send_action(actuators[1].fd,"start");
            usleep(350000);
            c='\0';
            fflush(stdin);
            send_action(actuators[0].fd,"turnoff");
            send_action(actuators[1].fd,"turnoff");
        }
        else if(c=='a'){
            send_action(actuators[2].fd,"right");
            usleep(350000);
            c='\0';
            fflush(stdin);
            send_action(actuators[2].fd,"release");
            
        }
        else if(c=='d'){
            send_action(actuators[2].fd,"left");
            usleep(350000);
            c='\0';
            fflush(stdin);
            send_action(actuators[2].fd,"release");
            
        }
      
        
    }
}

int init_tasks() {
    // Set the terminal to raw mode
    system("stty raw");
    char c;
    PTASK taskpeed=NULL,ww=NULL;
    printf("Apreta cualsevol tecla per mode automatic, apreta m per mode manual (usuaris experts en pilotatge P.O.D.)");
    fflush(stdout);
    c=getchar();
    if(c=='m'){
        while(c!=' '){
        printf("pilotatge manual selecionat, bona sort pilot.\n Com ja deus saber si has selecionat aquesta opcio els controls w,a,d activen els motors corresponents durant 350 ms\n Apreta ESPAI per començar la aventura");
        fflush(stdout);
        c=getchar();
        }
        Task_create(&ww, "ww", task_w, NULL, 3, 3);
        Readyqueue_enqueue(tasks_queue, ww);
    }
    else{
    Task_create(&taskpeed, "Update speed", task_read_speed, NULL, 3, 3);
    Readyqueue_enqueue(tasks_queue, taskpeed);
    }



}