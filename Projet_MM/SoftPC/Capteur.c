#include <pthread.h>
#include <assert.h>
#include <string.>

#include "file.h"

#define TAILLE 20

File* fileAttenteTrame = (File*) malloc(sizeof(File));
char cmd_sync[]="*1111111111111111111";
char* chaine_trame = (char *)&cmd_sync[0];

int tempsAttente, frequency = 1;
int fd;

/**
*	Fonction d'envoi des trames
*	Les trames sont mise dans une file. Cette fonction envoye les trames une par une
*/
void* envoie_trames(void* arg) /* Fonction Thread */
{
	int i;
	FILE* fichier;

	while(1) /* Boucle à l'infini */
	{
		tempsAttente = 1./frequency;
		
		if(file_nombre(fileAttenteTrame) > 0) /* Element(s) dans la file */
		{
			// On envoie les trames par ordre d'arrivee
			strcpy(chaine_trame,file_qeek(fileAttenteTrame)); /* Lecture de la tete de file */
			file_dequeue(&fileAttenteTrame); /* Supprime l'element en tête de file*/

			i = 0;
			while(i < TAILLE && !chaine_trame[i] != '\0') /*envoi de la trame*/
			{
				write(fd, &chaine_trame[i], 1);
				++i;
			}
		
		
			fichier = fopen("donne_envoye.txt", "r");/*ouverture fichier*/
			
			if(fichier)/*si le fcihier ne s'ouvre pas*/
			{
				fprintf(fichier, "%s", chaine_trame); /* Sauvegarde de la trame */
			}
			
		}
		usleep(100000); /* 0.1s */
		tcflush(fd, TCIOFLUSH); /* effacer le flux de donnée apres utilisation*/
	}
	
	pthread_exit(NULL); /* fin du thread */
}


void* simule_temperateur(void* arg) /* Fonction Thread */
{
	char *trame = &cmd_sync[0];
	trame = (char*) trame_temperateur();

	printf("%s \n", trame);
	
	usleep(tempsAttente * 1000000); /* Temps attente en seconde */
	
	file_enqueue(&fileAttenteTrame, trame_temperateur());
}

// PC
int main()
{
	pthread_t PthId0;
	pthread_t PthId1;
	pthread_t PthId2;
	pthread_t PthId3;

	int rc, rd=0;
	void *retval;

	printf("Lancement du capteur. . .\n");

	fd = open_port(); /* open device for read&write*/
	
	if (fd == -1)
	{
		printf("Erreur durant l'ouverture du port !\n");
	}
    else
    {
		/* Recupere les messages */
		rc = pthread_create(&PthId0, NULL, recuperation_trames, NULL);
		assert(0 == rc);
		printf("Creation thread reception trames et ACK\n");
		
		/* Envoie les trames en tete de file */
		rc = pthread_create(&PthId1, NULL, envoie_trames, NULL);
		assert(0 == rc);
		printf("Creation thread envoie trame en tete de file\n");
		
		/* Recupere les messages */
		rc = pthread_create(&PthId2, NULL, simule_temperateur, NULL);
		assert(0 == rc);
		printf("Creation thread creation de la trame de temperature\n");
		
		/*rc = pthread_create(&PthId3, NULL, ack_trame, NULL);
		assert(0 == rc);
		printf("Trame recue et validee\n");*/

		rc = pthread_join(PthId0, &retval);
		rc = pthread_join(PthId1, &retval);
		rc = pthread_join(PthId2, &retval);
		/*rc = pthread_join(PthId3, &retval);*/
	}
	

	tcflush(a.fd, TCIOFLUSH);
	close(a.fd);
	
	return 0;
}
