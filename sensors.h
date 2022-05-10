#ifndef __SENSORS_H__
#define __SENSORS_H__

struct sensor{

    char name[14];
    int fd,fd2,port,delay;  //en el cas de ser una pipe i no un socket fd 2 sera fd_write 
    
};
extern struct sensor sensors[6];


int init_sensors();

#endif
