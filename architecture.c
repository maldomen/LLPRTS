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




#include "os.h"

#include "architecture.h"

#include "simulation_parameters.h"

// Put all the architecture related initialization code here
PREADYQUEUE sensors_queue=NULL,actuators_queue=NULL,control_queue=NULL;
PSCHEDULER sensors_scheduler=NULL,actuators_scheduler=NULL,control_scheduler=NULL;


int init_architecture(){

Readyqueue_create(&sensors_queue);
Readyqueue_create(&actuators_queue);
Readyqueue_create(&control_queue);

Scheduler_create(&sensors_scheduler, SCHEDULING_SHORTEST_JOB_FIRST, sensors_queue,1);
Scheduler_create(&actuators_scheduler, SCHEDULING_SHORTEST_JOB_FIRST,actuators_queue, 1);
Scheduler_create(&control_scheduler, SCHEDULING_SHORTEST_JOB_FIRST, control_queue,1);

return 0;
}