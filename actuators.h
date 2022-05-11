#ifndef __ACTUATORS_H__
#define __ACTUATORS_H__


struct actuator{

    char name[8];
    int fd,port,delay;  //en el cas de ser una pipe i no un socket fd 2 sera fd_write 
    
};
extern struct actuator actuators[3];
int init_actuators();

#endif
