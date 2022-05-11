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
#include "sensors.h"

#include "actuators.h"
struct actuator actuators[3]={};
int init_actuators()
{   
    //pump1
    strcpy(actuators[0].name,"Pump_1");
    actuators[0].port=20000;
    actuators[0].delay=50;
    //pump2
    strcpy(actuators[1].name,"Pump_2");
    actuators[1].port=20001;
    actuators[0].delay=50;
    //drift
    strcpy(actuators[2].name,"Drift");
    actuators[2].port=20002;
    actuators[0].delay=500;
    for(int i=0;i<3;i++){
        connect_socket(actuators[i].port,&actuators[i].fd);
    }
    return 0;
}

