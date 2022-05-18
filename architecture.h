#ifndef __ARCHITECTURE_H__
#define __ARCHITECTURE_H__

int init_architecture();
extern PREADYQUEUE sensors_queue,actuators_queue,control_queue;
extern PSCHEDULER sensors_scheduler,actuators_scheduler,control_scheduler;


#endif
