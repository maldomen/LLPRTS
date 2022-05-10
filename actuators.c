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

int init_actuators()
{   struct sensor sensors[6];
    printf("sensor print desde act%s",sensors[0].name);
    fflush(stdout);
    
    return 0;
}

