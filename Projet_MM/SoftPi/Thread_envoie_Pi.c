#include "Thread_envoie_Pi.h"




void envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	// Envoie sur le port
}

void envoie_STOP()
{
	char trame[] = "Q03STOPW"; // Trame d'extinction
	
	// Envoie sur le port
}

int load_frequence()
{
	FILE*		fichier = fopen("../Site/frequency.txt", "r");
	int			freq		= 0;
	
	if(fichier)
	{
		fscanf(fichier, "%d", &freq);
		
		if(fclose(fichier) == EOF)
			printf("Erreur lors de la fermeture du fichier frequency.txt\n");
	}
	else
		printf("Erreur lors de l'ouverture du fichier frequency.txt\n");
	
	return freq;
}

void envoie_frequence()
{
	char	trame[] = "Z02000060W"; // Trame par defaut Frequence 1/60 Hz
	int		freq		= load_frequence();


	// Creation de la trame depuis la frequence (int)
	trame[8] = freq%10;
	trame[7] = (freq/10)%10;
	trame[6] = (freq/100)%10;
	trame[5] = (freq/1000)%10;
	trame[4] = (freq/10000)%10;
	trame[3] = (freq/100000)%10;
	
	
	// Envoie sur le port
}





