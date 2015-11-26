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

#include "./Thread_envoie_Pi.h"
#include "./Thread_reception_Pi.h"
#include "File/File.h"


File* fileAttenteTrame = (File*) malloc(sizeof(File));
char	chaine[]="*1111111111111111111";
char* chaine_trame = (char *)&chaine[0];

int tempsAttente, frequency = 1;
int recu = 0; // Reception trame

int fd; // Pour l'utilisation du PORT


int open_port();



int main()
{
	pthread_t Thread_envoie;
	pthread_t Thread_reception;

	int rc;
	
	
	fd = open("/dev/ttyUSB0", O_RDWR); /*open device for read&write*/ //"/dev/ttyAMA0" ?
	
	if (fd == -1)
	{
		printf("Erreur durant l'ouverture du port !\n");
	}
  else
	{
		/* Recupere les messages */
		rc = pthread_create(&Thread_reception, NULL, reception_trame_pi, NULL);
		assert(0 == rc);
		printf("Creation thread reception trames et ACK\n");
		
		/* Envoie les trames en tete de file */
		rc = pthread_create(&Thread_envoie, NULL, envoie_trames, NULL);
		assert(0 == rc);
		printf("Creation thread envoie trame en tete de file\n");
		
		
	}

	
	rc = pthread_join(Thread_reception, NULL);
	rc = pthread_join(PthId2, NULL);
	
	return 0;
}


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

