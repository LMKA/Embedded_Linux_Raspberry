#ifndef __Thread_envoie_Pi_H
#define __Thread_envoie_Pi_H


#include <stdio.h>
#include <string.h>
#include "../File/File.h"


#define TAILLE_TRAME 20

void envoie_ACK();
void envoie_STOP();
int load_frequence();
void envoie_frequence();
void* envoie_trames(void* arg);





#endif

