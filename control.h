#ifndef __CONTROL_H__
#define __CONTROL_H__

int connect_socket(int port, int *fd); 
int connect_pipe(char *pipe_name, int *fd_read, int *fd_write);
int read_pipe_active_float(int fd, float *value);
int read_socket_passive_float(int fd, float *value);

void task_read_speed(void *param);
void task_motor(void *param);
void task_w(void *param);


int init_tasks();

#endif
