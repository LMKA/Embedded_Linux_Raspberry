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

#include "File.h"

#define ATTENTE for(i=0;i<100000;i++);

#define TAILLE_TRAME 20

void	envoie_ACK();
void	envoie_STOP();
int		generer_temperature();
char*	generer_trame_temperature();
void* simule_temperateur();
void	save_trame_envoyer(char trame[]);
void*	envoie_trames();


#endif

