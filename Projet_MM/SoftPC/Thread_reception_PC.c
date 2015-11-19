#include "Thread_reception_PC.h"



void* reception_freq(double *freq)
{
	FILE*	fichier;
	
	while(1)
	{
		fichier = fopen("./freq.txt", "r");
		if (fichier)
		{
			fscanf(fichier, "%f", freq);
		
			if(fclose(fichier) == EOF)
			{
				printf("Erreur lors de la fermeture du fichier freq.txt");
				pthread_exit(NULL);
			}
		}
		else
		{
			printf("Erreur lors de l'ouverture du fichier freq.txt");
			pthread_exit(NULL);
		}
		
		usleep(10000000);	// Attente de 10s
	}
}

