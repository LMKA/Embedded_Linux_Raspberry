#include "Thread_envoie_Pi.h"


void envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	// Envoie sur le port
}

void envoie_STOP()
{
	char trame[] = "Q03STOPW"; // Trame d'extinction
}

void envoie_frequence()
{
	char	trame[] = "Z02000060W"; // Trame par defaut Frequence 1/60 Hz
	int		freq	= 0;
	FILE* fichier = fopen("../Site/frequency.txt","r");
	
	if(fichier)
		fscanf(fichier, "%d", &freq);
	else
		printf("Erreur ouverture fichier frequency.txt !!!\n");
	
	// Creation de la trame depuis la frequence (int)
	trame[8] = freq%10;
	trame[7] = (freq/10)%10;
	trame[6] = (freq/100)%10;
	trame[5] = (freq/1000)%10;
	trame[4] = (freq/10000)%10;
	trame[3] = (freq/100000)%10;
	
	
	// Envoie sur le port
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



