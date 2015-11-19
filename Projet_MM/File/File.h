#ifndef _FILE_H__
#define _FILE_H__

#include <stdlib.h>

typedef struct file
{
    char *donnee;
    struct file *suivant;

} File;


void	file_enqueue(File **p_file, char *donnee);
int		file_dequeue(File **p_file);
int		file_nombre(File *p_file);
void	file_clear(File **p_file);
char*	file_qeek(File *p_file);



#endif

