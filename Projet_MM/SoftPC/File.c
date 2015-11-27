/***********************\

 * file_function.h     *
\***********************/
#include "File.h"

void initialisation_file (File *suite)
{
  suite->debut = NULL;
  suite->fin = NULL;
  suite->taille = 0;
}

/* enfiler (ajouter) un élément dans la file */
int enfiler (File *suite, Element *courant, char *donnee)
{
  Element *nouveau_element;
  int			ret = 0;
  
  if ((nouveau_element = (Element *) malloc (sizeof (Element))) == NULL)
    ret = -1;
    
  if ((nouveau_element->donnee = (char *) malloc (50 * sizeof (char))) == NULL)
    ret = -1;
    
  strcpy (nouveau_element->donnee, donnee);

  if(courant == NULL)
  {
    if(suite->taille == 0)
      suite->fin = nouveau_element;
    
    nouveau_element->suivant = suite->debut;
    suite->debut = nouveau_element;
  }
  else
  {
    if(courant->suivant == NULL)
			suite->fin = nouveau_element;
		
    nouveau_element->suivant = courant->suivant;
    courant->suivant = nouveau_element;
  }
  
  suite->taille++;
  
  return ret;
}

/* de_filer (supprimer) un élément de la file */
int de_filer (File * suite)
{
  Element *supp_element;
  int ret = 0;
  
  if (suite->taille == 0)
    ret = -1;

  supp_element = suite->debut;
  suite->debut = suite->debut->suivant;
  
  free(supp_element->donnee);
  free(supp_element);
  
  suite->taille--;
  
  return 0;
}

/* affichage de la file */
void affiche(File *suite)
{
  Element		*courant	= NULL;
  int				i 				= 0;
  
  courant = suite->debut;

  for(i = 0; i < suite->taille; ++i)
  {
    printf(" %s ", courant->donnee);
    courant = courant->suivant;
  }
}


