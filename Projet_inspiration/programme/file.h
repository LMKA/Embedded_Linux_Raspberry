/* En tête du fichier file.c */
#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------- Structure -------------------------------------- */

/* Structure de la pile */

typedef struct element element;
struct element
{
    char * donnee;
    struct element *nxt;
};
typedef element* llist;

/* Structure de reperage */



/* -------------------------------- Prototypes ------------------------------------ */
llist ajouterEnFin(llist liste, char* valeur);
void afficherListe(llist liste);
int estVide(llist liste);
llist supprimerElementEnTete(llist liste);
int nombreElements(llist liste);
llist effacerListe(llist liste);

#endif
