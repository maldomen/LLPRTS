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

#include <curses.h>


int main() {
    char c;
    // Set the terminal to raw mode
    system("stty raw");
    while(1) {
        c = getchar();
        // terminate when "." is pressed
        if(c == '.') {
            system("stty cooked");
            exit(0);
        }  
        printf("aixo:%C",c);
    }
}