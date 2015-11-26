#ifndef __Thread_reception_H_
#define __Thread_reception_H_



#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#include "File/File.h"


#define TAILLE_TRAME 20



void* reception_trame_pc();








#endif


