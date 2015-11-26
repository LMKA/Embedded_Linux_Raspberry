#ifndef __Thread_reception_Pi_H
#define __Thread_reception_Pi_H


#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <assert.h>

#include "File/File.h"

#define TAILLE_TRAME 20

void save_trame_temperature(char* trame);
void* reception_trame_pi();











#endif

