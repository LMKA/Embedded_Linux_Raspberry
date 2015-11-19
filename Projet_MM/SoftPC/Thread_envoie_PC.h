#ifndef __Thread_envoie_PC_H
#define __Thread_envoie_PC_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>



#define ATTENTE for(i=0;i<100000;i++)

char* envoie_ACK();
char* envoie_trame();
int open_port(void);

#endif

