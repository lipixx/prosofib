/*
 * sf.c - Sistem de Fitxers: 
 */

#include <sf.h>
#include <types.h>
#include <sched.h>

void init_filesystem()
{
  init_directori();
  init_tfo();
  init_disk();
  init_fat();
}

inline void init_tfo()
{
  int i;
  for (i=0; i<NUM_CANALS*NR_TASKS; i++)
    taula_fitxers_oberts[i].refs = 0;
}

inline void init_disk()
{
  int i,j;
   for (i=0; i<MAX_BLOCS; i++)
     for (j=0; j<BLOCK_SIZE; j++)
     disk[i][j] = 0;
}

inline void init_directori()
{
  int i;
  file console, keyboard;

  for (i=2; i<MAX_FILES; i++)
    {
      directori[i]->mode_acces_valid = -1;
    }

  console.nom = "console";
  console.mode_acces_valid = O_WRONLY;
  console.operations->sys_open_dev = NULL;
  console.operations->sys_close_dev = NULL;
  console.operations->sys_read_dev = NULL;
  console.operations->sys_write_dev = &sys_write_console;;
  console.first_block = NULL;
  console.size = NULL;
  console.n_blocs = NULL;

  keyboard.nom = "keyboard";
  keyboard.mode_acces_valid = O_RDONLY;
  keyboard.operations->sys_open_dev = NULL;
  keyboard.operations->sys_close_dev = NULL;
  keyboard.operations->sys_read_dev = &sys_read_keyboard;
  keyboard.operations->sys_write_dev = NULL;
  console.first_block = NULL;
  console.size = NULL;
  console.n_blocs = NULL;
  
  directori[1] = console;
  directori[0] = keyboard;
}

inline void init_fat()
{
  int i;
  bloc_de_la_llibertat=0;
  
  for(i=0; i<MAX_BLOCKS-1; i++) 
    fat[i]=i+1;
  
  fat[MAX_BLOCKS-1]=-1;
}

