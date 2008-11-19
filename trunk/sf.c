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
  struct file console;
  struct file keyboard;

  for (i=2; i<MAX_FILES; i++)
    {
      directori[i]->mode_acces_valid = -1;
    }
  
  console.nom[0] = 'c';
  console.nom[1] = 'o';
  console.nom[2] = 'n';
  console.nom[3] = 's';
  console.nom[4] = 'o';
  console.nom[5] = 'l';
  console.nom[6] = 'e';
  console.mode_acces_valid = O_WRONLY;
  console.operations->sys_open_dev = NULL;
  console.operations->sys_close_dev = NULL;
  console.operations->sys_read_dev = NULL;
  console.operations->sys_write_dev = sys_write_console;
  console.first_block = NULL;
  console.size = NULL;
  console.n_blocs = NULL;
  
  directori[1] = &console;
  
  keyboard.nom[0] = 'k';
  keyboard.nom[1] = 'e';
  keyboard.nom[2] = 'y';
  keyboard.nom[3] = 'b';
  keyboard.nom[4] = 'o';
  keyboard.nom[5] = 'a';
  keyboard.nom[6] = 'r';
  keyboard.nom[7] = 'd';
  keyboard.mode_acces_valid = O_RDONLY;
  keyboard.operations->sys_open_dev = NULL;
  keyboard.operations->sys_close_dev = NULL;
  keyboard.operations->sys_read_dev = sys_read_keyboard;
  keyboard.operations->sys_write_dev = NULL;
  keyboard.first_block = NULL;
  keyboard.size = NULL;
  keyboard.n_blocs = NULL;
  
  directori[0] = &keyboard;
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
