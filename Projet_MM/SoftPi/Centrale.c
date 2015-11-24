#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h> 
#include <time.h>
#include <math.h>

#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <pthread.h>
#include <assert.h>

#include "Thread_envoie_Pi.h"
#include "Thread_reception_Pi.h"

int fd; // Pour l'utilisation du PORT

int open_port()
{
  struct termios options;
  int fd,i;
//  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open("/dev/ttyUSB0", O_RDWR);
  if (fd==-1)
    printf("Erreur ouverture port !!!\n");
  else
  {
    fcntl(fd,F_SETFL,0);
//    fcntl(fd, F_SETFL, FNDELAY); //ne pas bloquer sur le read
    tcgetattr(fd,&options);
    usleep(10000);
    cfsetospeed(&options,B115200);
    cfsetispeed(&options,B115200);
	options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; /* Parite   : none */
    options.c_cflag &= ~CSTOPB; /* Stop bit : 1    */
    options.c_cflag &= ~CSIZE;  /* Bits     : 8    */
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
   // options.c_iflag &= ~(IXON);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST; // raw output
    options.c_lflag &= ~(ICANON | ECHO | ECHONL|IEXTEN | ISIG);
    // c_cc
    options.c_cc[VMIN]= 1;
    options.c_cc[VTIME]= 4;
    tcflush(fd,TCIOFLUSH); // flushIO buffer
    tcsetattr(fd, TCSANOW,&options); // set new configure immediately
//    tcflush(fd,TCIOFLUSH);
    usleep(10000);
  }

  return fd;	
}



int main()
{
	pthread_t PthId0;
	pthread_t PthId1;
	pthread_t PthId2;
	int rc;

    /* Ouverture du fichier pour enregistrer les trames qui arrive du PC (capteur) */
    /*fichier = fopen("trames", "w+");
	
    if(fichier == NULL)
    {
		 printf("Impossible d'ouvrir le fichier: trames\n");
		 exit(1);
	}
	
	if(fclose(fichier) == EOF)
	{
		printf("Probleme de fermeture du fichier liste_trame");
		exit(1);
	}
	*/
	
	
	fd = open("/dev/ttyUSB0", O_RDWR); /*open device for read&write*/ //"/dev/ttyAMA0" ?
	
	if (fd == -1)
	{
		printf("Erreur durant l'ouverture du port !\n");
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
			rc = pthread_create(&PthId2, NULL, reception_trame_pi, NULL); // TODO Recevoir les trames
			assert(0 == rc);
		}
	}

	
	rc = pthread_join(PthId1, NULL);
	rc = pthread_join(PthId2, NULL);
	
	return 0;
}
