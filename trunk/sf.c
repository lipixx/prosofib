/*
 * sf.c - Sistem de Fitxers: 
 */

#include <sf.h>
#include <types.h>
#include <sched.h>
#include <devices.h>
#include <errno.h>
#include <utils.h>

/* Retorna el primer bloc lliure de nblocks encadenats
 * a la fat. -1 si no hi ha prou espai.
 */
int balloc (int nblocks)
{
  int total, primer, anterior;
  
  
  if (bloc_de_la_llibertat == -1 || nblocks < 1) 
    return -1;
  
  if (fat[bloc_de_la_llibertat] == -1)
    {
      if (nblocks == 1)
	{
	  anterior = bloc_de_la_llibertat;
	  bloc_de_la_llibertat = -1;
	  return anterior;
	}
	  return -1;
    }
  
  anterior = bloc_de_la_llibertat;
  primer = bloc_de_la_llibertat;
  bloc_de_la_llibertat = fat[bloc_de_la_llibertat];
  total = 1; 

  while (total != nblocks && fat[bloc_de_la_llibertat] != -1)
    {
      anterior = bloc_de_la_llibertat;
      bloc_de_la_llibertat = fat[bloc_de_la_llibertat];
      total++;
    }
  
  if (fat[bloc_de_la_llibertat] == -1)
    {
      if (nblocks - total == 1)
	{
	  anterior = bloc_de_la_llibertat;
	  bloc_de_la_llibertat = -1;
	}
      else
	{
	  if (nblocks - total > 1)
	    {
	      bloc_de_la_llibertat = primer;
	      return -1;
	    }
	}
    }
  
  fat[anterior] = -1;
  
  return primer;
}

/*
 * Allibera espai ocupat per un fitxer.
 * Pre: no s'intenta esborrar espai lliure i ibloc0 es necessariament el primer bloc
 * d'un fitxer.
 */
int freed(int ibloc0)
{
  int i;

  if (ibloc0 >= MAX_BLOCKS || ibloc0 < 0 || ibloc0 == bloc_de_la_llibertat)
    return -1;

  if (bloc_de_la_llibertat == -1)
    {
      bloc_de_la_llibertat = ibloc0;
      return 0;
    }

  for (i = bloc_de_la_llibertat; fat[i] != -1; i++);
  fat[i] = ibloc0;

  return 0;
}

struct file * create_file(const char * path)
{
  int i, size, fblock;

  for (i=0; directori[i].n_refs != -1 && i < MAX_FILES;i++);
  if (i == MAX_FILES) return (struct file *) -ENFILE;
  
  for (size=0; path[size] != '\0'; ++size);
  if (size >= MAX_NAME_LENGTH) return (struct file *) -EINVAL;
  
  fblock = balloc(1);
  if (fblock < 0)
    return (struct file *) -EIO;

  //copy_from_user((char *)path,directori[i].nom,size);
  directori[i].nom = (char *) path;
  directori[i].mode_acces_valid = O_RDWR; //default per fitxers
  directori[i].operations->sys_open_dev = sys_open_file;
  directori[i].operations->sys_close_dev = sys_close_file;
  directori[i].operations->sys_read_dev = sys_read_file;
  directori[i].operations->sys_write_dev = sys_write_file;
  directori[i].first_block = fblock;
  directori[i].size = 0;
  directori[i].n_blocs = 1;
  directori[i].n_refs = 0;
  return &directori[i];
}


void init_tfo()
{
  int i;
  for (i=0; i<MAX_OPEN_FILES; i++)
    {
      taula_fitxers_oberts[i].refs = 0;
      taula_fitxers_oberts[i].mode_acces = 0;
      taula_fitxers_oberts[i].lseek = 0;
      taula_fitxers_oberts[i].opened_file = NULL;
   }
}

void init_disk()
{
  int i,j;
   for (i=0; i<MAX_BLOCKS; i++)
     for (j=0; j<BLOCK_SIZE; j++)
     disk[i][j] = 0;
}

void init_directori()
{
  int i;

  for (i=0; i<MAX_FILES; i++)
    {
      directori[i].n_refs = -1;
      ops[i].sys_open_dev = NULL;
      ops[i].sys_close_dev = NULL;
      ops[i].sys_read_dev = NULL;
      ops[i].sys_write_dev = NULL;
      directori[i].operations = &ops[i];
    }

  directori[0].nom = "keyboard";
  directori[0].mode_acces_valid = O_RDONLY;
  directori[0].operations->sys_read_dev = sys_read_keyboard;
  directori[0].first_block = NULL;
  directori[0].size = NULL;
  directori[0].n_blocs = NULL;
  directori[0].n_refs = 0;
 
  directori[1].nom = "console";
  directori[1].mode_acces_valid = O_WRONLY;
  directori[1].operations->sys_write_dev = sys_write_console;
  directori[1].first_block = NULL;
  directori[1].size = NULL;
  directori[1].n_blocs = NULL;
  directori[1].n_refs = 0;
}

void init_fat()
{
  int i;
  bloc_de_la_llibertat=0;
  
  for(i=0; i<MAX_BLOCKS-1; i++) 
    fat[i]=i+1;
  
  fat[MAX_BLOCKS-1] = -1;
}

void init_filesystem()
{
  init_directori();
  init_tfo();
  init_disk();
  init_fat();
}
