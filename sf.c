/*
 * sf.c - Sistem de Fitxers: 
 */

#include <sf.h>
#include <types.h>
#include <sched.h>
#include <devices.h>

void init_tfo()
{
  //AQUESTA INICIALITZACIO ES CORRECTA?
  //BASTARIA FER taula_fitxers_oberts = NULL???
  int i;
  struct fitxers_oberts fitxers[NUM_CANALS*NR_TASKS];

  for (i=0; i<NUM_CANALS*NR_TASKS; i++)
    {
      fitxers[i].refs = 0;
      fitxers[i].lseek = 0;
      fitxers[i].mode_acces = 0;
      fitxers[i].opened_file = NULL;
      taula_fitxers_oberts[i] = &fitxers[i];
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

  struct file fitxer[MAX_FILES];
  struct file_operations ops[MAX_FILES];

  for (i=0; i<MAX_FILES; i++)
    {
      fitxer[i].operations = &ops[i];
      fitxer[i].nom = "";
      fitxer[i].mode_acces_valid = -1;
      fitxer[i].operations->sys_open_dev = NULL;
      fitxer[i].operations->sys_close_dev = NULL;
      fitxer[i].operations->sys_read_dev = NULL;
      fitxer[i].operations->sys_write_dev = NULL;
      fitxer[i].first_block = NULL;
      fitxer[i].size = NULL;
      fitxer[i].n_blocs = NULL;
      directori[i] = &fitxer[i];
    }

  directori[0]->nom = "keyboard";
  directori[0]->mode_acces_valid = O_WRONLY;
  directori[0]->operations->sys_open_dev = NULL;
  directori[0]->operations->sys_close_dev = NULL;
  directori[0]->operations->sys_read_dev = sys_read_keyboard;
  directori[0]->operations->sys_write_dev = NULL;
  directori[0]->first_block = NULL;
  directori[0]->size = NULL;
  directori[0]->n_blocs = NULL;
 
  directori[1]->nom = "console";
  directori[1]->mode_acces_valid = O_WRONLY;
  directori[1]->operations->sys_open_dev = NULL;
  directori[1]->operations->sys_close_dev = NULL;
  directori[1]->operations->sys_read_dev = NULL;
  directori[1]->operations->sys_write_dev = sys_write_console;
  directori[1]->first_block = NULL;
  directori[1]->size = NULL;
  directori[1]->n_blocs = NULL; 
}

void init_fat()
{
  int i;
  bloc_de_la_llibertat=0;
  
  for(i=0; i<MAX_BLOCKS-1; i++) 
    fat[i]=i+1;
  
  fat[MAX_BLOCKS-1]=-1;
}

void init_filesystem()
{
  init_directori();
  init_tfo();
  init_disk();
  init_fat();
}
