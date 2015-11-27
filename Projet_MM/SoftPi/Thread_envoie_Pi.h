#ifndef __Thread_envoie_Pi_H
#define __Thread_envoie_Pi_H


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

#include "Queue.h"

#define TAILLE_TRAME 20

void envoie_ACK();
void envoie_STOP();
int load_frequence();
void envoie_frequence();
void save_trame_envoyer(char trame[]);
void* envoie_trames();





#endif

