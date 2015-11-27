#include "Thread_reception_PC.h"


extern	File* FileAttenteTrame;
extern	char	chaine[];
extern	char* chaine_trame;
extern	int tempsAttente;
extern	int frequency;
extern	int recu;
extern	int fd;

void* reception_trame_pc()
{
	int		i				= 0;
	char	trame[TAILLE_TRAME + 1],
				car			=	'\0';

	while(1)
	{
		i = 0;
		
		// Lecture d'une trame depuis le port
		do
		{
			if(read(fd, &car, 1) >= 1)
			{
				trame[i] = car;
				++i;
			}
		}while(car != 'W' && i < TAILLE_TRAME);
		
		// Suivant le type de la trame on fait le traitement adequat
		switch(trame[2])
		{
			case 0: // ACK
				// Traitement de l'ACK
				break;
			case 2: // FREQ !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				frequency = trame[3]*100000 + trame[4]*10000 + trame[5]*1000 + trame[6]*100 + trame[7]*10 + trame[3];
				break;
			case 3: // STOP
				pthread_exit(NULL); /* fin du thread */
				break;
			default:
				printf("Erreur dans la forme de la trame !!!\n");
		}
	}
}
