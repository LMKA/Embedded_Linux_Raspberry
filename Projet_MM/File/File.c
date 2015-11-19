#include "File.h"



/* Procedure qui permet d'enfiler un element dans la file */
void	file_enqueue(File **p_file, char* donnee)
{
    File *p_nouveau = (File *) malloc(sizeof *p_nouveau);
    
    if (p_nouveau != NULL)
    {
        p_nouveau->suivant = NULL;
        p_nouveau->donnee = donnee;
        
        if (*p_file == NULL)
            *p_file = p_nouveau;
        else
        {
            File *p_tmp = *p_file;
            
            while (p_tmp->suivant != NULL)
                p_tmp = p_tmp->suivant;

            p_tmp->suivant = p_nouveau;
        }
    }
}


/* Fonction qui defile la file d'un seul element */
int		file_dequeue(File **p_file)
{
    int ret = -1;
    
    /* On teste si la file n'est pas vide. */
    if (*p_file != NULL)
    {
        /* Creation d'un element temporaire pointant vers le deuxieme element de la file. */
        File *p_tmp = (*p_file)->suivant;
        
        /* Valeur a retourner */
        ret = (*p_file)->donnee;
        
        /* Effacement du premier element. */
        free(*p_file);
        *p_file = NULL;
        
        /* On fait pointer la file vers le deuxième element. */
        *p_file = p_tmp;
    }
    
    return ret;
}

/* Fonction qui retourne le nombre d'elements dans la file */
int		file_nombre(File *p_file)
{
	if (p_file == NULL)
		return 0;
	
	return file_nombre(p_file->suivant) + 1;
}



/* Procedure qui permet de vider la file */
void	file_clear(File **p_file)
{
    /* Tant que la file n'est pas vide. */
    while (*p_file != NULL)
        file_dequeue(p_file); // On enleve l'element courant.

}


/* Fonction qui retourne la donnee en tete de la file sans la supprimer */
char*	file_qeek(File *p_file)
{
    char *ret = NULL;

    if (p_file != NULL)
        ret = p_file->donnee;
  
    return ret;
}
