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
int init_architecture()
{
    return 0;
}
