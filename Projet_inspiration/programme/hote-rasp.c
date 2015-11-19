
#include "hote-rasp.h"

/****************************************************/
/*          tread:recuperation_freq                 */
/*  Permet de recuperer la frequence provenant d'un */
/*                  fichier .txt                    */
/****************************************************/
void* recuperation_freq(void* arg)
{
	while(1)
	{
		/*ouverture du fichier*/
		fic=fopen("frequency.txt", "w+"); /*ouverture du fichier contenant la frequence*/
		if(fic == NULL)/*si pas de fichier ou echec à l'ouverture*/
		{
			 printf("Impossible d'ouvrir le fichier frequence\n");
			 pthread_exit(NULL);/* fin du thread */
		}
		else fscanf(fic, "%f",&frequence);/*recuperation valeur frequance*/
		tps=1/frequence;
		if(fclose(fic) == EOF)/*fermeture fichier*/
		{
			printf("Probleme de fermeture du fichier frequence\n");
			pthread_exit(NULL);/* fin du thread */
		}
		usleep(5000000); /*attente de 5 secondes avant de réactualiser la donnée*/
	}
	pthread_exit(NULL);/* fin du thread */
}


/****************************************************/
/*                  thread:synchro                   */
/* Permet de verifier que la carte RASPBERRY-PI soit*/
/*                 bien connectée                   */
/****************************************************/
void* synchro(void* arg) /*thread =D*/
{
	int pret=0, i;
	printf("sdfh\n");
	while(pret==0) /*bouleen de connexion*/
	{
		if(0!=strcmp(chaine_Rasp, cmd_retour)) /*La chaine provenant de la carte RASP est differante de la commande de présence de connexion avec l'hote*/
		{
		     /*si pas prete on envoi la commande start à la carte*/
			for(i=0; i< TAILLE*2; i++) /*envoi de la commande cmd_start*/
			{
				write(a.fd,&cmd_start[i],1);/* envoi terme à terme*/
			}
			usleep(200000); /* attente de 0.2s*/
		}
		else pret=1;
	}
	
	tcflush(a.fd,TCIOFLUSH); /* efface le bus*/
	pthread_exit(NULL); /* fin du thread */
}


/****************************************************/
/*                   tread:envoi                    */
/*  Permet d'envoyer (en fonction de la valeur du  */
/*              compteur) la trame                    */
/****************************************************/
void* envoi(void* arg) /*thread*/
{
	int i;
	int tps2=(int)floor(tps/3);/*nouveau calcul du temps afin de pouvoir envoyer au maximum 3 fois la trame durant un cycle*/
	while(1)/*tourne tout le temps*/
	{
		if(compteur<=2) /*si le nombre d'envoi de trame est <=3*/
		{
			if(1==estVide(ma_liste_hote) && compteur==0) /*si la liste est vide et qu'il n'y a pas de valeur à charger*/
			{
				usleep(tps2);/* on attend qu'une valeur arrive dans la file*/
			}
			else if(0==estVide(ma_liste_hote) && compteur==0) /*si element dans la file et nouvelle trame a recuperer (compteur=0)*/
			{

				chaine_hote=ma_liste_hote->donnee;/*lecture dans la file nouvelle valeur*/
				supprimerElementEnTete(ma_liste_hote);/*suppression de l'element en tête de file*/
				for(i=0; i< TAILLE*2; i++) /*envoi de la trame*/
				{
					write(a.fd,&chaine_hote[i],1);
				}
				compteur++;/*incrementation compteur*/
				usleep(tps2);/*attente de tps2*/
			}
			else/* sinon compteur !=0 alors envoi trame + incrementation de compteur*/
			{
				for(i=0; i< TAILLE*2; i++) /*envoi de la trame*/
				{
					write(a.fd,&chaine_hote[i],1);
				}
				compteur++; /*incrementation compteur*/
				usleep(tps2);/*attente de tps2*/
			}

		}
		else if(compteur==3) /*si la trame a deja ete envoyée 3 fois alors sauvegarde dans un fichier de la trame non envoyée etcompteur remis à 0*/
		{
			fic=fopen("donne_capt_temp.txt", "r");/*ouverture fichier*/
			if(fic && !feof(fic))/*si le fcihier ne s'ouvre pas*/
			{
				fprintf(fic, "%s", chaine_hote);/*sauvegarde de la chaine de caractère non envoyée*/
				fprintf(fic, "\n");/*retour à la ligne*/
			}
			else
			compteur=0; /*remise à 0 du compteur d'envoi*/
		}
		tcflush(a.fd,TCIOFLUSH); /* effacer le flux de donnée apres utilisation*/
	}
	pthread_exit(NULL); /* fin du thread */
}


/****************************************************/
/*           thread:confirmation_start               */
/*   Permet de dederminer l'activité de la carte    */
/*                    RASPBERRY_PI                  */
/****************************************************/
void * confirmation_start(void* arg)
{
	int i=0;
	while(0 != strcmp(cmd_retour, chaine_Rasp)) /*tant que l'hote n'a pas reçu la confirmation le thread continu*/
	{
		for(i=0; i< TAILLE*2; i++)/*recupere de la trame provenant de la raspberry et la met dans un char* chaine_Rasp*/
		{
			if (i<(TAILLE*2-1) && read(a.fd, &chaine_Rasp[i], 1) < 1) {
				printf("rien lu\n");
				--i ;
			}
		}
		printf("sortie de la de boucle\n");
		for(i=0; i< TAILLE*2; i++) /*envoi de la commande cmd_start*/
		{
			printf("%c",chaine_Rasp[i]);/* envoi terme à terme*/
		}
		printf("\n");

		for(i=0; i< TAILLE*2; i++) /*envoi de la commande cmd_start*/
		{
			printf("%c",cmd_retour[i]);/* envoi terme à terme*/
		}
		printf("\n");
		
		
		usleep(10000); /*attente de 0.001s*/
		}

	pthread_exit(NULL);/* fin du thread */

}


/****************************************************/
/*            thread:confirmation_trame              */
/*  Permet de confirmer la reception de la carte de */
/*                     la trame                     */
/****************************************************/
void * confirmation_trame(void* arg) /*confirmation de la récéption de la trame envoyé au part avant*/
{
	int tps1 = (int)floor(tps*(2-compteur)/3);
	int i;
	for(i=0; i< TAILLE*2; i++)/*recupere de la trame provenant de la raspberry et la met dans un char* chaine_Rasp*/
	{
		printf("bla\n");
		if (read(a.fd, &chaine_Rasp[i], 1) < 1) {
			printf("rien lu\n");
			--i ;
		}
	}
	if(0 == strcmp(pcmd_conf, chaine_Rasp) )/*si trame acquitement reçu*/
	{
		compteur=0; /*remise a 0 du compteur d'envoi de trame*/
		usleep(tps1);/*attente d'un tier du temps consacré*/
	}
	pthread_exit("1");/* fin du thread: retourne 1 */

}


/****************************************************/
/*              tread:creation_trame                */
/*      Permet de creer une trame et l'incorpore    */
/*                   dans la file                   */
/****************************************************/
void* creation_trame(void * arg)
{
	char *trame=&cmd_start[0];

	trame= (char*)simulateur_trame();/* fonction création trame à récupérer de l'autre fichier*/
	/* rajout dans la file*/
	ajouterEnFin(ma_liste_hote, trame);
	usleep(tps);
	pthread_exit(NULL);/* fin du thread */
}


int main()
{

	pthread_t PthId0;
	pthread_t PthId1;
	pthread_t PthId2;
	pthread_t PthId3;
	pthread_t PthId4;
	pthread_t PthId5;

	int rc, rd=0;
	void *retval;

	/* recuperer du php la valeur de la fréquence ==> tps=1/val*/
	//rc = pthread_create(&PthId5, NULL, recuperation_freq, NULL);
	printf("pan1\n");

	a.fd = open_port(); /* open device for read&write*/
	if (a.fd==-1)
	{
		printf("Erreur ouverture port !!!\n");
	}
    else
    {
		while(rd!=1)
		{
			printf("pan2\n");
			rc = pthread_create(&PthId0, NULL, synchro, NULL);
			printf("egf\n");
			//assert(0 == rc); //permet de comparer la réponse de retour d'une fct appelée
			printf("pan3\n");
			printf("numero du thread = %d\n", (int)PthId0);
			rd = pthread_create(&PthId1, NULL, confirmation_start, NULL);
			rc = pthread_join(PthId0, &retval);
			rd = pthread_join(PthId1, &retval);
			assert(1 == rd);

			if(rd!=1)
			{
				printf("la carte raspberry ne repond pas\n");
				usleep(100000);
			}
		}
		if(rd==1)
		{

			rc = pthread_create(&PthId2, NULL, creation_trame, NULL);
			assert(0 == rc);
			printf("trame cree\n");

			rc = pthread_create(&PthId3, NULL, envoi, NULL);
			assert(0 == rc);
			printf("trame envoyee\n");

			rc = pthread_create(&PthId4, NULL, confirmation_trame, NULL);
			assert(0 == rc);
			printf("trame recue et validee\n");

		}

	}
	rc = pthread_join(PthId2, &retval);
	rc = pthread_join(PthId3, &retval);
	rc = pthread_join(PthId4, &retval);
	rc = pthread_join(PthId5, &retval);

	tcflush(a.fd,TCIOFLUSH); /*vide le flux de donnée*/
	close(a.fd);
	return 0;
}
