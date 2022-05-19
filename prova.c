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
#include <math.h>


int main() {
    float lunar_gravity = 1.0f;       // Moon gravity in m/s2


    float pump_acceleration = -1.5f;  // Acceleration of a pump

    //v=at
    float micro=1000000;
    float a=3.8,b=a*micro;
    //double temps_motor=micro*sqrt((double)((1.5f)/-(2*pump_acceleration+lunar_gravity)));
    double temps_motor=1;
    printf("a=%f b=%f\n, pasat a int=%d, pasat a double %lf\n tempsmotor=%d\n",a,b,(int)b,(double)b,(int)temps_motor);
    float alfa=3.2;
    printf("a truncat %d alfa trunc %d",(int)a,(int)alfa);
    }