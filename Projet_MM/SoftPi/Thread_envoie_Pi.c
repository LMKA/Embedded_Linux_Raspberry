#include "Thread_envoie_Pi.h"
#include "Thread_reception_Pi.h"

extern Queue* QueueAttenteTrame;
extern char	chaine[];
extern char* chaine_trame;
extern int tempsAttente;
extern int frequency;
extern int recu; // Reception trame
extern int fd; // Pour l'utilisation du PORT




void envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	// Envoie sur le port (enfiler)
	Queue_enqueue(&QueueAttenteTrame, trame);
}

void envoie_STOP()
{
	char trame[] = "Q03STOPW"; // Trame d'extinction
	
	// Envoie sur le port (enfiler)
	Queue_enqueue(&QueueAttenteTrame, trame);
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
	Queue_enqueue(&QueueAttenteTrame, trame);
}

void save_trame_envoyer(char trame[])
{
	FILE*		fichier =fopen("donnee_envoyer.txt", "r");
			
	if(fichier)
	{
		fprintf(fichier, "%s", trame); /* Sauvegarde de la trame */
		
		if(fclose(fichier) == EOF)
			printf("Erreur : lors de la fermeture du fichier donnee_envoyer.txt\n");
	}
	else
		printf("Erreur : lors de l'ouverture du fichier donnee_envoyer.txt\n");
}

void* envoie_trames() /* Fonction Thread */
{
	int 	i	= 0;

	while(1)
	{
		if(Queue_nombre(QueueAttenteTrame) > 0) /* Element(s) dans la file */
		{
			// On envoie les trames par ordre d'arrivee
			strcpy(chaine_trame, Queue_qeek(QueueAttenteTrame)); /* Lecture de la tete de file */
			Queue_dequeue(&QueueAttenteTrame); /* Supprime l'element en tête de file*/

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
				Queue_enqueue(&QueueAttenteTrame, chaine_trame);
			
			
		}
		
		// Temps attente avant le prochain envoie (depant de la frequence)
		usleep(1000000);
		
		tcflush(fd, TCIOFLUSH); /* effacer le flux de donnee apres utilisation*/
	}
	
	pthread_exit(NULL); /* fin du thread */
}

// ---------------------------------------------------------------------


