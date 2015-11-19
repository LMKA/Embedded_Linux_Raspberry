#ifndef _CHAINE_RASP_
#define _CHAINE_RASP_


#include <stdio.h>
#include <unistd.h>
#include <string.h> /* pour l'utilisation des fonctions sur les chaines de caractères*/
#include <stdlib.h> /* pour les allocs*/
#include <time.h>   /* pour comptabiliser le temps*/
#include <math.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <assert.h>

#include "file.h"

char cmd_start[]="*,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_start= &cmd_start[0];

char cmd_retour[]="*,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_retour= &cmd_retour[0];

char cmd_conf[]="*,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_conf= &cmd_conf[0];

typedef struct Arg_struct{
	int fd;
}arg_t;

arg_t a;

char* chaine_Rasp= (char *)&cmd_retour[0];
char* chaine_hote;
llist ma_liste_Rasp = NULL;

FILE *fic;


void* synchro(void* arg);
void * reception(void* arg);
void* file(void *arg);



#endif // _CHAINE_RASP_
