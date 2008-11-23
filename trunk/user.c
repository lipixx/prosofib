#include <libc.h>
#include <types.h>
#include <stdio.h>
#include <jocs.h>
#include <sf.h>

void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  printf ("\nBenvinguts a ZeOS!");  
 

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

  runjp (); //Hi ha que posar NUM_PAG_DATA = 8

  // int fd = open("patata",O_WRONLY|O_CREAT);

  //run2_jp();
  
  struct dir_ent buffer;
  char buffer2[512];
  int fd,fd2,i;
  /* 
  fd = open ("patata",O_RDWR|O_CREAT);
  fd2 = open ("cuminu",O_RDWR|O_CREAT);

  write(fd,"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123FXP789012345678901234567890",281);
  
  close (fd);
  fd = open ("patata",O_RDWR);

  write(fd,"soc_tubercul",12);
  write(fd,"AAA",3);*/
  /*
  write(fd2"soc_verdura",11);

  close(fd);
  
  fd = open("patata",O_RDWR);
  
  for (i = 0; i<MAX_FILES; i++)
    {
      readdir(&buffer,i);
      printf("\nNom: ");
      printf(buffer.nom);
      printf("\nSize: ");
      printint(buffer.size);
    }
  
  read(fd,buffer2,281);
  write(1,buffer2,281);
*/

  while (1);
  return 0;
}
