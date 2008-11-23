#include <libc.h>
#include <types.h>
#include <stdio.h>
#include <jocs.h>
#include <sf.h>

void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  printf ("\nBenvinguts a ZeOS!");  
 
  char buffer[10];
  read(0,buffer,10);
  write(1,buffer,10);

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

  /* JOCS DE PROVES DE LA PRIMERA ENTREGA */

  //  runjp (); //Hi ha que posar NUM_PAG_DATA = 8

  // int fd = open("patata",O_WRONLY|O_CREAT);

  //run2_jp();
  struct dir_ent buffer;
  //char buffer3[512];
  char buffer2[512];
  int fd;
  
  fd = open ("patata",O_RDWR|O_CREAT);

  write(fd,"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123FXP789012345678901234567890",280);
  write(fd,"HOLA",4);

  close(fd);
  fd = open("patata",O_RDWR);
  
  readdir(&buffer,2);
  printf("\nnom: ");
  printf(buffer.nom);
  printf("\nsize: ");
  printint(buffer.size);

  read(fd,buffer2,284);
  write(1,buffer2,284);

  while (1);
  return 0;
}
