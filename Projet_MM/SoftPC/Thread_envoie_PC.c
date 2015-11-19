#include "Thread_envoie_PC.h"

char* envoie_ACK()
{
	char trame[] = "Y00ACKW"; // Trame d'acquittement
	
	 return trame;
}

char* trame_temperateur()
{
  int temp = 0;
  int precision = 0;
  char signe = '+'; // Signe + par defaut
  char trame[] = "X01+21.501135081015W"; // Trame par defaut

  struct tm* current_date; // Structure contenant la date actuelle
  time_t t;
  
  t = time(NULL);
  current_date = localtime(t);

// A mettre dans une autre fonction
  srand(time(NULL)); // Initialisation du seed pour le random

  temp = (rand() % 61) - 20; // Temp appartient a [-20; +40]
  precision = rand() % 100; // Precsion appartient a [0; 99]
// ---------------------------------

  if(temp < 0)
	signe = '-'; // Si temperature negative alors on change le signe ( '-' )
  
  temp = abs(temp);

  trame[3] = signe;
  trame[4] = temp/10 + 48; // Chiffre des dizaine de la temperature
  trame[5] = temp%10 + 48; // Chiffre des unites de la temperature
  
  trame[7] = precision/10 + 48;
  trame[8] = precision%10 + 48;
  
  trame[9] = current_date.tm_hour/10 + 48;
  trame[10] = current_date.tm_hour%10 + 48;
  
  trame[11] = current_date.tm_min/10 + 48;
  trame[12] = current_date.tm_min%10 + 48;
  
  trame[13] = current_date.tm_mday/10 + 48;
  trame[14] = current_date.tm_mday%10 + 48;
  
  trame[15] = current_date.tm_mon/10 + 48;
  trame[16] = current_date.tm_mon%10 + 48;
  
  trame[17] = (current_date.tm_year/10 - 100) + 48;
  trame[18] = current_date.tm_year%10 + 48;

  return trame;

}



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

