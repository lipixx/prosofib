#include <libc.h>
#include <stdio.h>
#include <jocs.h>


void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{

  printf ("\nBenvinguts a ZeOS!");

  //provar_fork (9); //No sobrepassar NR_TASKS

  //provar_get_stats ();

  //provar_exit ();

  //provar_nice ();

  //provar_switch ();

  //provar_semafors ();

  /* JOCS DE PROVES DE LA PRIMERA ENTREGA */

  runjp (); //Hi ha que posar NUM_PAG_DATA = 8

  //run2_jp();

  while (1);
  return 0;
}
