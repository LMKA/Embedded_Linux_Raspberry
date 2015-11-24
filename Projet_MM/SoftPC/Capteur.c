#include <pthread.h>
#include <assert.h>
#include <string.>

#include "../File/File.h"

#define TAILLE_TRAME 20

File* fileAttenteTrame = (File*) malloc(sizeof(File));
char chaine[]="*1111111111111111111";
char* chaine_trame = (char *)&chaine[0];

int tempsAttente, frequency = 1;
int recu = 0; // REception trame


int fd;

int open_port(void)
{
  struct termios options;
  int fd,i;
//  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
  fd = open("/dev/ttyAMA0", O_RDWR); // open device for read&write
  if (fd==-1)
    printf("Erreur ouverture port !!!\n");
  else
  {
    fcntl(fd,F_SETFL,0);
    fcntl(fd, F_SETFL, FNDELAY); //ne pas bloquer sur le read
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
