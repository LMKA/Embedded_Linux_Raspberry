#include "simulateurTemperature.h"

char *trame_one=k; /*trame de données*/
/*trame=(char *)malloc(TAILLE*sizeof(char));*/

float generationTemperature(int k, int l) /*fonction qui génère aléatoirement la température*/
{
	float temp=0;

	srand(time(NULL));

	temp=rand()%k-l;

	return temp;
}
char *simulateur_trame()
{
	int i;
	float temperature=0;

	struct tm *instant;
	time_t t;

	t=time(NULL); /*date à partir du 1er nov 1970*/
	instant=localtime(&t); /*récuperation du temps actuel*/

	trame_one[0]='*'; /*indique le début de la trame*/

	/*génération de la temperature en réel*/
	temperature=generationTemperature(60, 20);
	temperature=temperature + generationTemperature(100, 0)/100;
	printf("la température est de :%.2f \n", temperature);

	if (temperature>0)
		trame_one[3]='+';
	else
		trame_one[3]='-';
	/*mise de la temperature dans la trame en ASCII*/
	trame_one[4]=floor((int)temperature/10)+48;
	trame_one[5]=floor((int)temperature %10)+48;
	trame_one[6]='.';
	trame_one[7]=floor((int)(temperature *10)%10)+48;
	trame_one[8]=floor((int)(temperature *100)%10)+48;

	/*mise de l'heure dans la trame en ASCII*/
	trame_one[9]=floor(instant->tm_hour/10)+48;
	trame_one[10]=floor(instant->tm_hour%10)+48;
	trame_one[11]=floor(instant->tm_min/10)+48;
	trame_one[12]=floor(instant->tm_min%10)+48;

	/*mise de la date dans la trame en ASCII*/
	trame_one[13]=floor(instant->tm_mday/10)+48;
	trame_one[14]=floor(instant->tm_mday%10)+48;

	/*mise du mois dans la trame en ASCII*/
	trame_one[15]=floor((instant->tm_mon+1)/10)+48;
	trame_one[16]=floor((instant->tm_mon+1)%10)+48;

	/*mise de l'année dans la trame en ASCII*/
	trame_one[17]=floor((instant->tm_year-100)/10)+48;
	trame_one[18]=floor(instant->tm_year%10)+48;

	/*derniere case de la trame*/
	trame_one[19]='/';

	/*affichage de la trame*/
	printf("%c ",trame_one[0]);
	for(i=1;i<TAILLE-1;++i)
	{
		printf("%d ", trame_one[i]);
	}
	printf("%c \n",trame_one[TAILLE]);

	return trame_one;
}
int open_port(void)
{
  struct termios options;
  int fd;
/*  fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);*/
  fd = open("/dev/ttyUSB0", O_RDWR); /* open device for read&write*/
  if (fd==-1)
    printf("Erreur ouverture port !!!\n");
  else
  {
    fcntl(fd,F_SETFL,0);
/*    fcntl(fd, F_SETFL, FNDELAY); */ /*ne pas bloquer sur le read*/
    tcgetattr(fd,&options);
    usleep(10000);
    cfsetospeed(&options,B115200);
    cfsetispeed(&options,B115200);
    options.c_cflag &= ~PARENB; /* Parite   : none */
    options.c_cflag &= ~CSTOPB; /* Stop bit : 1    */
    options.c_cflag &= ~CSIZE;  /* Bits     : 8    */
    options.c_cflag |= CS8;
    /*options.c_oflag &= ~CRTSCTS;*/
   /* options.c_iflag &= ~(IXON);
    tcsetattr(fd,TCSANOW,&options);*/
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHONL|IEXTEN | ISIG);
    /* c_cc*/
    options.c_cc[VMIN]= 1;
    options.c_cc[VTIME]= 4;
/* flush!
    tcflush(fd,TCIOFLUSH);*/
    tcsetattr(fd, TCSANOW,&options);
    tcflush(fd,TCIOFLUSH);
    usleep(10000);
  }

  return fd;
}

