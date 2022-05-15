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

#include "control.h"
#include "simulation_parameters.h"
#include "sensors.h"



struct sensor sensors[6]={}; //inicialitzem tot a 0 de manera global
int init_sensors(){
    
    
    //speed
    strcpy(sensors[0].name,"speedpipe");
    
    sensors[0].port=0;
    sensors[0].delay=3;
    sensors[0].fd=0;
    sensors[0].fd2=0;
    connect_pipe(sensors[0].name,&sensors[0].fd,&sensors[0].fd2);
    //altitude1
    strcpy(sensors[1].name,"Altitude_1");
    sensors[1].port=20010;
    sensors[1].delay=100;
    
    //altitude2
    strcpy(sensors[2].name,"Altitude_2");
    sensors[2].port=20011;
    sensors[2].delay=100;
    
    //altitude3
    strcpy(sensors[3].name,"Altitude_3");
    sensors[3].port=20012;
    sensors[3].delay=100;
    
    //left sensor
    strcpy(sensors[4].name,"Left_sensor");
    sensors[4].port=20015;
    sensors[4].delay=300;
    
    //right sensor
    strcpy(sensors[5].name,"Right_sensor");
    sensors[5].port=20016;
    sensors[5].delay=300;
    
    //fd sockets
    for(int i=1;i<6;i++){
        connect_socket(sensors[i].port, &sensors[i].fd);
    } 
    return 0;
    
}

