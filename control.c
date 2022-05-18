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
#include "simulation_parameters.h"
#include <math.h>
#include <semaphore.h>

static float micro=1000000;
static float speed=0.0,alt1=100.0,alt2=100.0,alt3=100.0;
static int left=0,right=0;




//************************************ conectar pod
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

int read_socket_passive_int(int fd, int *value){
char c=0;
send(fd, (char*)&c, sizeof(char),0);
recv(fd, value, sizeof(int),0);
return 0;
}

int send_action(int fd, char*message){
    char c=0;
    int length = strlen(message)+1;
    send(fd, (char*)&length, sizeof(int), 0);
    send(fd, message, length, 0);


}

//********************* Tasca si activem mode conduccio manual
void task_manual(void *param){
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


//********************** Tasques sensors scheduler
 



void task_read_speed(void *param) {
    read_pipe_active_float( sensors[0].fd,&speed);
    //printf("speed read\n");
}

void task_read_alt1(void *param) {
    read_socket_passive_float(sensors[1].fd,&alt1);
    //printf("alt1 read\n");
}

void task_read_alt2(void *param) {
    read_socket_passive_float(sensors[2].fd,&alt2);
    //printf("alt2 read\n");

}

void task_read_alt3(void *param) {
    read_socket_passive_float(sensors[3].fd,&alt3);
    //printf("alt3 read\n");

}

void task_read_left(void *param){
    read_socket_passive_int(sensors[4].fd,&left);
}

void task_read_right(void *param){
    read_socket_passive_int(sensors[5].fd,&right);
}


//************************* Tasques control scheduler


void task_mantenir_velocitat(void *param){
    
    
    if (speed>0.1){
        //printf("entra manternir speed valor =%f\n",speed);
        //*** ptask aperdiodiques
        PTASK taskfrenar=NULL;
        //ACTUATORS TASKCREATE 
        Task_create(&taskfrenar, "frenar pod", task_frenar_pod, NULL, 0, 300);
        Readyqueue_enqueue(actuators_queue,taskfrenar);
        
    
    }

}

sem_t sem_1metre;

static int i=0;
void task_mantenir_altura(void *param){
    sem_wait(&sem_1metre);
    //printf("entra manternir alt valor alt3=%f\n",alt3);
    if((alt1<=2.5 || alt2<=2.5 || alt3<=2.5) && (speed>-0.08)){
        printf("entra ifmanternir alt valor alt3=%f i=%d%d%d\n",alt3,i,i,i);
        i++;
        //*** ptask aperdiodiques
        PTASK task1metre=NULL;
        //ACTUATORS TASKCREATE 
        Task_create(&task1metre, "pujar 1 metre", task_pujar_1metre, NULL, 0, 250);
        Readyqueue_enqueue(actuators_queue,task1metre);
        
    }
    else sem_post(&sem_1metre);
}

//********************** Tasques actuators queue

void task_frenar_pod(void *param){
    
    float temps_motor=(((-speed/((2*pump_acceleration)+lunar_gravity))*micro)+30000); // + per corretgit delay
    //printf("entra en frenar\n valor=%f",temps_motor);
    if (temps_motor>0 && (speed>-0.08)){
        send_action(actuators[0].fd,"start");
        send_action(actuators[1].fd,"start");
        usleep((int)temps_motor);
        send_action(actuators[0].fd,"turnoff");
        send_action(actuators[1].fd,"turnoff");
    }
    
    /*
    send_action(actuators[0].fd,"start");
    send_action(actuators[1].fd,"start");
    while(speed>0.1);
    send_action(actuators[0].fd,"turnoff");
    send_action(actuators[1].fd,"turnoff");
    */

}

void task_pujar_1metre(void *param){
    //float micro2=100000;
    double temps_motor=((double)micro)*sqrt((double)((1.5f)/-(2*pump_acceleration+lunar_gravity))); //la formula hauria de ser aquesta pero puja molt mes i es surt :/
    double factor=0.35;
    temps_motor=temps_motor*factor;//minvento un factor i provo optim
    if (temps_motor>0 && (speed>-0.08)){
        send_action(actuators[0].fd,"start");
        send_action(actuators[1].fd,"start");
        usleep((int)temps_motor);
        send_action(actuators[0].fd,"turnoff");
        send_action(actuators[1].fd,"turnoff");
        sem_post(&sem_1metre);
    }
    

}
int init_tasks() {
    
    sem_init(&sem_1metre,0,1);
    
    char c;
    

    printf("\nApreta cualsevol tecla per mode automatic, apreta m per mode manual (usuaris experts en pilotatge P.O.D.)\n");
    fflush(stdout);
    // Set the terminal to raw mode
    system("stty raw");
    
    //Codi per mode manual (No forma part de la misió LLPRTS)
    c=getchar();
    if(c=='m'){
        while(c!=' '){
        // Set the terminal to raw mode
        system("stty raw");
        printf("pilotatge manual selecionat, bona sort pilot.\n Com ja deus saber si has selecionat aquesta opcio els controls w,a,d activen els motors corresponents durant 350 ms.\n Recorda que si uses varis controls seguits s'encuaran un darrere l'altre.\nApreta ESPAI per començar la aventura\n");
        fflush(stdout);
        c=getchar();
        }
        PTASK manualtask=NULL;
        Task_create(&manualtask, "manualtask", task_manual, NULL, 0, 0);
        Readyqueue_enqueue(sensors_queue, manualtask);
    }
    //codi misió:

    else{
    
    
    
    //SENSORS QUEUE *****************************
    PTASK taskpeed=NULL,taskalt1=NULL,taskalt2=NULL,taskalt3=NULL,taskright=NULL;
    Task_create(&taskpeed, "Update speed", task_read_speed, NULL, 20, 3);
    Readyqueue_enqueue(sensors_queue, taskpeed);

    Task_create(&taskalt1, "Update alt1", task_read_alt1, NULL,60, 100);
    Readyqueue_enqueue(sensors_queue, taskalt1);
    
    Task_create(&taskalt2, "Update alt2", task_read_alt2, NULL, 50, 100);
    Readyqueue_enqueue(sensors_queue, taskalt2);

    Task_create(&taskalt3, "Update alt3", task_read_alt3, NULL, 70, 100);
    Readyqueue_enqueue(sensors_queue, taskalt3);

    //CONTROL QUEUE **************************
    PTASK maltura=NULL,mspeed=NULL;
    Task_create(&maltura, "manteniralt", task_mantenir_altura, NULL, 300, 500);
    Readyqueue_enqueue(control_queue, maltura);
    
    Task_create(&mspeed, "mantenirspeed", task_mantenir_velocitat, NULL, 200, 500);
    Readyqueue_enqueue(control_queue, mspeed);
    
    }



}