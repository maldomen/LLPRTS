#ifndef __CONTROL_H__
#define __CONTROL_H__

static int connect_socket(int port, int *fd); 
static int connect_pipe(char *pipe_name, int *fd_read, int *fd_write);
int read_pipe_active_float(int fd, float *value);
int read_socket_passive_float(int fd, float *value);

int init_tasks();

#endif
