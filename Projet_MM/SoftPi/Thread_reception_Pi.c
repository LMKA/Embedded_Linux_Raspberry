#include "Thread_reception_Pi.h"

extern File* fileAttenteTrame;
extern char	chaine[];
extern char* chaine_trame;
extern int tempsAttente;
extern int frequency;
extern int recu; // Reception trame
extern int fd; // Pour l'utilisation du PORT

void save_trame_temperature(char* trame)
{
	FILE*		fichier			= fopen("/home/pi/public_html/donnees_temperature.txt", "a");
	double	temperature = 0;
	int			heure				= 0,
					min					= 0,
					jour				= 0,
					mois				= 0,
					annee				= 0;

	if(fichier)
	{
		// Recuperation de l'heure
		heure	= trame[9]*10 + trame[10];
		min		= trame[11]*10 + trame[12];
		
		// Recuperation de la date
		jour	= trame[13]*10 + trame[14];
		mois	= trame[15]*10 + trame[16];
		annee	= 2000 + trame[17]*10 + trame[18];
		
		temperature = trame[4]*10 + trame[5] + (trame[7]*10 + trame[8])/((double) 100);
			
			if(trame[3] == '-')
				temperature = 0 - temperature;
				
		fprintf(fichier, " - %d/%d/%d - %d:%d > %.2f °C\n", jour, mois, annee, heure, min, temperature);
		
		if(fclose(fichier) == EOF)
			printf("Erreur lors de la fermeture du fichier donnees_temperature.txt\n");
	}
	else
		printf("Erreur lors de l'ouverture du fichier donnees_temperature.txt\n");
}


void* reception_trame_pi()
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
			case 1: // TEMPERATURE
				save_trame_temperature(trame);
				break;
			case 3: // STOP
				pthread_exit(NULL); /* fin du thread */
				break;
			default:
				printf("Erreur dans la forme de la trame !!!\n");
		}
	}
}



