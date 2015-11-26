#include "Thread_envoie_Pi.h"




void envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	// Envoie sur le port (enfiler)
	file_enqueue(&fileAttenteTrame, trame);
}

void envoie_STOP()
{
	char trame[] = "Q03STOPW"; // Trame d'extinction
	
	// Envoie sur le port (enfiler)
	file_enqueue(&fileAttenteTrame, trame);
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

void* envoie_trames(void* arg) /* Fonction Thread */
{
	int 	i				= 0;

	while(1)
	{
		if(file_nombre(fileAttenteTrame) > 0) /* Element(s) dans la file */
		{
			// On envoie les trames par ordre d'arrivee
			strcpy(chaine_trame, file_qeek(fileAttenteTrame)); /* Lecture de la tete de file */
			file_dequeue(&fileAttenteTrame); /* Supprime l'element en tête de file*/

			i = 0;
			while(i < TAILLE_TRAME && !chaine_trame[i] != '\0') /*envoi de la trame*/
			{
				write(fd, &chaine_trame[i], 1);
				++i;
			}
			
			// !!! IL FAUT ATTENDRE DE RECEVOIR ACK AVANT DE SAUVEGARDER LA TRAME COMME ETANT ENVOYER !!!
			if(recu)
				save_trame_envoyer(chaine_trame);
			else
				// Si la centrale n'a pas recu la trame
				file_enqueue(&fileAttenteTrame, chaine_trame);
			
			
		}
		
		// Temps attente avant le prochain envoie (depant de la frequence)
		usleep(1000000);
		
		tcflush(fd, TCIOFLUSH); /* effacer le flux de donnee apres utilisation*/
	}
	
	pthread_exit(NULL); /* fin du thread */
}

// ---------------------------------------------------------------------


