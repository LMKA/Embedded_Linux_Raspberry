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



#define ATTENTE for(i=0;i<100000;i++);

void	envoie_ACK();
void	envoie_STOP();
int		load_frequence();
int		generer_temperature();
char*	generer_trame_temperature();
void* simule_temperateur(void* arg);
void	save_trame_envoyer(char trame[]);
void*	envoie_trames(void* arg);


#endif

