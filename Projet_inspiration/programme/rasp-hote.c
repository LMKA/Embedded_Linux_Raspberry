#include "rasp-hote.h"


#define FREQUENCE 0.2    /* en Hz*/
#define TAILLE 19

/****************************************************/
/*                   tread:syncro                   */
/*     Permet de verifier l'envoi de la commande    */
/*                 start par l'hote                 */
/****************************************************/
void* synchro(void* arg) /*thread =D*/
{
	int pret=0, i;
	while(pret==0)/*bouleen de connexion*/
	{
		if(0==strcmp(chaine_Rasp, pcmd_start))/*La chaine provenant de la carte RASP est differante de la commande de présence de connexion avec l'hote*/
		{
		    /*si hote pret on envoi la commande retour à l'hote*/
			for(i=0; i< TAILLE; i++) /* envoi de la commande cmd_start*/
			{
				write(a.fd,&cmd_start[i],1);
			}
			pret=1;
		}
		else usleep(100000);/*attendre 0.1s*/
	}
	tcflush(a.fd,TCIOFLUSH);/* effacer le flux de donnée apres utilisation*/
	pthread_exit(NULL);/* fin du thread */
}

/****************************************************/
/*                  tread:reception                 */
/*      Permet de creer une trame et l'incorpore    */
/*                   dans la file                   */
/****************************************************/
void * reception(void* arg)
{
	int i;
    while(1)
    {
        if(0>=read(a.fd, &chaine_hote[0], 1))/*si donnée sur le bus*/
        {
            for(i=0; i< TAILLE; i++)/*on récupere la trame dans la chaine_hote*/
            {
                read(a.fd, &chaine_hote[i], 1);
            }
            tcflush(a.fd,TCIOFLUSH);/* effacer le flux de donnée apres utilisation*/
            ajouterEnFin(ma_liste_Rasp, chaine_hote);/*stoquage dans file*/
        }
        usleep(50000);/*attendre 0.05s*/
    }
	pthread_exit(NULL);/* fin du thread */
}

/****************************************************/
/*                   tread:fike                     */
/*      verifie s'il y a une trame dans la file     */
/*   si oui sauvegarde dans un fichier et envoi la  */
/*              commande acquitance                 */
/*               sinon attend 0.1s                  */
/****************************************************/
void* file(void *arg)
{
	int i;
	while(1)
    {
        if(0==estVide(ma_liste_Rasp)) /* information sur la pile*/
        {
            for(i=0; i< TAILLE; i++)
            {
                read(a.fd, &chaine_hote[i], 1);
            }

            fprintf(fic,"%s", chaine_hote);/* écrire dans le fichier à l'endroi du pointeur la nouvelle trame*/


            for(i=0; i< TAILLE; i++) /*envoi de la trame */
            {
                write(a.fd,&cmd_conf[i],1);// ecrire sur le bus la trame modifiée
            }
            supprimerElementEnTete(ma_liste_Rasp);
        }
        usleep(100000);/*attente */
    }
    tcflush(a.fd,TCIOFLUSH); /* effacer le flux de donnée apres utilisation*/
	pthread_exit(NULL); /* fin du thread */
}


int main()
{

	pthread_t PthId0;
	pthread_t PthId1;
	pthread_t PthId2;
	int rc;

    /*ouverture du fichier*/
    fic=fopen("liste_trame", "w+");
    if(fic == NULL)
    {
		 printf("Impossible d'ouvrir le fichier liste_trame\n");
		 exit(1);
	}
	a.fd = open("/dev/ttyUSB0", O_RDWR); /*open device for read&write*/
	if (-1==a.fd)
	{
		printf("Erreur ouverture port !!!\n");
	}
    else
    {

		printf("test de l'activation de l'envoi de donnee\n");
		rc = pthread_create(&PthId0, NULL, synchro, NULL);
		assert(0 == rc);
		rc = pthread_join(PthId0, NULL);
		rc = pthread_create(&PthId1,NULL, synchro, NULL);
		assert(0 == rc);

		while(0!=rc)
		{
			printf("attente de l'envoi de l'activation du flux\n");
			usleep(50000);
		}
		if(0==rc)
		{
			printf("activation de l'envoi de donnee\n");
			rc = pthread_create(&PthId2, NULL, reception, NULL);
			assert(0 == rc);
		}
	}
	if(fclose(fic) == EOF)
	{
		printf("Probleme de fermeture du fichier liste_trame");
		exit(1);
	}
	rc = pthread_join(PthId1, NULL);
	rc = pthread_join(PthId2, NULL);
	return 0;
}
