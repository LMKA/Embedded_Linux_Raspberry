#ifndef _HOTE_RASP_
#define _HOTE_RASP_

#include <pthread.h>
#include <assert.h>
#include "simulateurTemperature.h"
#include "file.h"

char cmd_start[]="*,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_start= &cmd_start[0];

char cmd_retour[]="*,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_retour= &cmd_retour[0];

char cmd_conf[]="*,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
char * pcmd_conf= &cmd_conf[0];


/*unsigned char cmd_start[]="*,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,/";
unsigned char * pcmd_start;
pcmd_start= &cmd_start;*/

/* Pour que la Raspberry Pi puisse faire la différence entre un message
   commande et un message donnée nous noterons le début de la chaine de
   caractère de la manière suivante:
					00... => envoi de hote -> Raspb
					01... => envoi de Raspb -> hote
					10... => commande
*/


typedef struct Arg_struct{
	int fd;
}arg_t;

int compteur=0;
float frequence=1; // en Hz
arg_t a;
float tps=0;


FILE *fic;
FILE *fic2;

char* chaine_Rasp =&cmd_start[0];
char* chaine_hote = (char *)&cmd_start[0];

llist ma_liste_hote = NULL;


void* recuperation_freq(void* arg);
void* synchro(void* arg); /*thread =D*/
void* envoi(void* arg); /*thread*/
void * confirmation_start(void* arg);
void * confirmation_trame(void* arg); /*confirmation de la récéption de la trame envoyé au part avant*/
void* creation_trame(void * arg);


#endif // _HOTE_RASP_
