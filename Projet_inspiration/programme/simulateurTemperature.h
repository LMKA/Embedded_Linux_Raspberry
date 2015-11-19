#ifndef simulateur_H
#define simulateur_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

#define ATTENTE for(i=0;i<100000;i++)
#define TAILLE 20/*longueur de la trame de donnée*/

char k[TAILLE];


float generationTemperature(int, int); /*fonction qui génère aléatoirement la température*/
char *simulateur_trame();
int open_port(void);

#endif

