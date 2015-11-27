#include "Thread_envoie_PC.h"


extern	File* FileAttenteTrame;
extern	char	chaine[];
extern	char* chaine_trame;
extern	int tempsAttente;
extern	int frequency;
extern	int recu;
extern	int fd;

void envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	 // Envoie sur le port
	 enfiler(FileAttenteTrame, FileAttenteTrame->fin, trame);
}

void envoie_STOP()
{
	char trame[] = "Q03STOPW"; // Trame d'acquittement
	
	 // Envoie sur le port
	 enfiler(FileAttenteTrame, FileAttenteTrame->fin, trame);
}


int generer_temperature()
{
	int temperature = 0,
			precision		= 0,
			signe				= 1;
	
	// Initialisation du seed pour le random
  srand(time(NULL));

  temperature = (rand() % 61) - 20; // Temp appartient a [-20; +40]
  precision = rand() % 100; // Precsion appartient a [0; 99]
	
	if(temperature < 0)
		signe = -1;
	
	// Mise de la temperature sous la forme de 3450 <- (ex: 34.50 C)
	temperature = (temperature * 1000) + (signe * precision);
	
	return temperature;
}


char* generer_trame_temperature()
{
  
  char				signe		= '+', // Signe + par defaut
							*trame	= "X01+21.501135081015W"; // Trame par defaut
	int					temp		= 0;
	
  struct tm	*current_date; // Structure contenant la date actuelle
  time_t 			t;						// Structure du temps
  
  // Initialisation du temps
  t = time(NULL);
  current_date = localtime(&t);

	temp = generer_temperature();

  if(temp < 0)
	{
		signe = '-'; // Si temperature negative alors on change le signe ( '-' )
		temp = abs(temp);
  }
  

  trame[3] = signe;
  trame[4] = temp/1000 + 48;			// Chiffre des dizaine de la temperature
  trame[5] = (temp/100)%10 + 48;	// Chiffre des unites de la temperature
  
  trame[7] = (temp%100)/10 + 48;
  trame[8] = temp%10 + 48;
  
  trame[9] = current_date->tm_hour/10 + 48;
  trame[10] = current_date->tm_hour%10 + 48;
  
  trame[11] = current_date->tm_min/10 + 48;
  trame[12] = current_date->tm_min%10 + 48;
  
  trame[13] = current_date->tm_mday/10 + 48;
  trame[14] = current_date->tm_mday%10 + 48;
  
  trame[15] = current_date->tm_mon/10 + 48;
  trame[16] = current_date->tm_mon%10 + 48;
  
  trame[17] = (current_date->tm_year/10 - 100) + 48;
  trame[18] = current_date->tm_year%10 + 48;

  return trame;
}


// ---------------------------------------------------------------------
void* simule_temperateur() /* Fonction Thread */
{
	char *trame	= &chaine[0];
	
	while(1)
	{
		trame			= generer_trame_temperature();

		// On affiche la trame
		printf("%s \n", trame);
		
		if(!frequency)
			tempsAttente = 1./frequency;
		else
			printf("Erreur : frequence = 0\n");
		
		// A REVOIR !! (Temps de simulation doit etre inferieur au temps d'envoie de trames)
		usleep(tempsAttente * 1000000); /* Temps attente en seconde */
		
		enfiler(FileAttenteTrame, FileAttenteTrame->fin, trame);
	}
}
// ---------------------------------------------------------------------


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

// ---------------------------------------------------------------------
/*
*	Fonction d'envoi des trames
*	Les trames sont mise dans une file. Cette fonction envoye les trames une par une
*/
void* envoie_trames() /* Fonction Thread */
{
	int 	i	= 0;

	while(1)
	{
		if(FileAttenteTrame->taille > 0) /* Element(s) dans la file */
		{
			// On envoie les trames par ordre d'arrivee
			strcpy(chaine_trame, file_donnee(FileAttenteTrame)); /* Lecture de la tete de file */
			de_filer(FileAttenteTrame);

			i = 0;
			while(i < TAILLE_TRAME && !chaine_trame[i] != 'W') /*envoi de la trame*/
			{
				write(fd, &chaine_trame[i], 1);
				++i;
			}
			write(fd, &chaine_trame[i], 1); // Ecrire le W aussi
			
			usleep(100000); // 0.1
			
			// !!! IL FAUT ATTENDRE DE RECEVOIR ACK AVANT DE SAUVEGARDER LA TRAME COMME ETANT ENVOYER !!!
			if(recu)
				save_trame_envoyer(chaine_trame);
			else
				// Si la centrale n'a pas recu la trame
				enfiler(FileAttenteTrame, FileAttenteTrame->fin, trame);
			
			
		}
		
		// Temps attente avant le prochain envoie (depant de la frequence)
		usleep(100000);
		
		tcflush(fd, TCIOFLUSH); /* effacer le flux de donnee apres utilisation*/
	}
	
	pthread_exit(NULL); /* fin du thread */
}

// ---------------------------------------------------------------------



