#include <libc.h>
#include <types.h>
#include <stdio.h>
#include <jocs.h>
#include <sf.h>

void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  printf("\n      ################################################");
  printf("\n                   Benvinguts a ZeOS 1.3b"); 
  printf("\n      ################################################");

  printf("\n       - A partir d'aquest punt ja has entrat al sistema!");
  printf("\n       Si estas llegint aquest missatge potser t'agradaria");
  printf("\n       modificar el fitxer user.c descomentant una prova");
  printf("\n       dels jocs de proves disponibles per veure les");
  printf("\n       funcionalitats que t'ofereix ZeOS!\n\n\n\n.");


  /* Prova KBD -
  char buffer[512];

  int f, llegits;

  f = fork ();

  if (f == 0)
    {
      llegits = read (0, buffer,300);
      printf ("\nRead ens retorna: ");
      printint (llegits);
      printf("\n\n");
      write (1, buffer, 300);
      exit ();
    }
  

  */
  //JOCS DE PROVES DE LA SEGONA ENTREGA:
  //provar_fork (9); //No sobrepassar NR_TASKS
  //provar_get_stats ();
  //provar_exit ();
  //provar_nice ();
  //provar_switch ();
  //provar_semafors ();

  /*JOCS DE PROVES DE LA TERCERA ENTREGA. DESCOMENTAR EL
     QUE INTERESSI: */
  //provar_open();
  //provar_dup();
  //provar_close();
  //provar_write_disp();
  //provar_read_disp();
  //provar_unlink();
  //provar_readdir();

  /* JOCS DE PROVES DE LA PRIMERA ENTREGA */

  //runjp (); //Hi ha que posar NUM_PAG_DATA = 8

  // int fd = open("patata",O_WRONLY|O_CREAT);

  //run2_jp();

  while (1);
  return 0;
}
