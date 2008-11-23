#include <libc.h>
#include <types.h>
#include <stdio.h>
#include <jocs.h>
#include <sf.h>

void runjp ();


int strlen(char * buffer)
{int i;
	for (i=0; buffer[i] != 0; i++);
	return i;
}

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  printf ("\nBenvinguts a ZeOS!");  
 

  //JOCS DE PROVES DE LA SEGONA ENTREGA:
  /* __asm__ __volatile__ ("movl $40,%%eax\n"
			"int $0x80\n"
			:
			:
			);
  */
  //provar_fork (9); //No sobrepassar NR_TASKS
  //provar_get_stats ();
  //provar_exit ();
  //provar_nice ();
  //provar_switch ();
  //provar_semafors ();

  /*JOCS DE PROVES DE LA TERCERA ENTREGA. DESCOMENTAR EL
    QUE INTERESSI:*/
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

