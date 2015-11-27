/*********************\

 *      file.h       *
\*********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FirstInFirstOut */
#define file_donnee(suite) (suite)->debut->donnee


typedef struct ElementListe
{
  char *donnee;
  struct ElementListe *suivant;
} Element;

typedef struct ListeRepere
{
  Element *debut;
  Element *fin;
  
  int taille;
} File;

/* initialisation */
void initialisation_file (File * suite);

/* ENFILER*/
int enfiler (File * suite, Element * courant, char *donnee);

/* DE_FILER*/
int de_filer (File * suite);

/* Affiche la file */
void affiche(File *suite);
