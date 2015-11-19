#include "Thread_reception_Pi.h"



void save_trame_temperature(char* trame)
{
	FILE*		fichier			= fopen("/home/pi/public_html/donnees_temperature.txt", "a");
	double	temperature = 0;
	int			heure				= 0,
					min					= 0,
					jour				= 0,
					mois				= 0,
					annee				= 0;

	if(fichier)
	{
		// Recuperation de l'heure
		heure	= trame[9]*10 + trame[10];
		min		= trame[11]*10 + trame[12];
		
		// Recuperation de la date
		jour	= trame[13]*10 + trame[14];
		mois	= trame[15]*10 + trame[16];
		annee	= 2000 + trame[17]*10 + trame[18];
		
		temperature = trame[4]*10 + trame[5] + (trame[7]*10 + trame[8])/((double) 100);
			
			if(trame[3] == '-')
				temperature = 0 - temperature;
				
		fprintf(fichier, " - %d/%d/%d - %d:%d > %.2f °C\n", jour, mois, annee, heure, min, temperature);
		
		if(fclose(fichier) == EOF)
			printf("Erreur lors de la fermeture du fichier donnees_temperature.txt\n");
	}
	else
		printf("Erreur lors de l'ouverture du fichier donnees_temperature.txt\n");
}


void reception_trame_pi(char* trame)
{
	if(trame != NULL)
		switch(trame[2])
		{
			case 0: // ACK
				// Traitement de l'ACK
				break;
			case 1: // TEMPERATURE
				save_trame_temperature(trame);
				break;
			case 3: // STOP
				pthread_exit(NULL); /* fin du thread */
				break;
			default:
				printf("Erreur dans la forme de la trame !!!\n");
		}
}




int open_port(void)
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



