/*
 * sf.c - Sistem de Fitxers: 
 */

#include <sf.h>
#include <types.h>
#include <sched.h>
#include <devices.h>

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

  for (i=2; i<MAX_FILES; i++)
    {
      directori[i].n_refs = -1;
      directori[i].operations->sys_open_dev = NULL;
      directori[i].operations->sys_close_dev = NULL;
      directori[i].operations->sys_read_dev = NULL;
      directori[i].operations->sys_write_dev = NULL;
    }

  directori[0].nom = "keyboard";
  directori[0].mode_acces_valid = O_RDONLY;
  directori[0].operations->sys_open_dev = NULL;
  directori[0].operations->sys_close_dev = NULL;
  directori[0].operations->sys_read_dev = sys_read_keyboard;
  directori[0].operations->sys_write_dev = NULL;
  directori[0].first_block = NULL;
  directori[0].size = NULL;
  directori[0].n_blocs = NULL;
  directori[0].n_refs = 0;
 
  directori[1].nom = "console";
  directori[1].mode_acces_valid = O_WRONLY;
  directori[1].operations->sys_open_dev = NULL;
  directori[1].operations->sys_close_dev = NULL;
  directori[1].operations->sys_read_dev = NULL;
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
