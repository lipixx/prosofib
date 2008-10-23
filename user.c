#include <libc.h>
#include <stdio.h>

void run2_jp ();
void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{

  int ret_fork1, ret_fork2, ret_fork3, aux1;

  printf ("\nBenvinguts a ZeOS!");

  ret_fork1 = fork ();
  switch (ret_fork1)
    {
    case 0:
      printf ("\nTASK1> fill de TASK0, return fork hauria de ser 0 -> ");
      printint (ret_fork1);
      printf (",tinc pid:");
      printint (getpid ());
      /*Codi de T1-AF */
      /*Fi Codi T1-AF */
      ret_fork2 = fork ();
      switch (ret_fork2)
	{
	case 0:
	  printf ("\nTASK2> fill de TASK1, return fork hauria de ser 0 -> ");
	  printint (ret_fork2);
	  printf (",tinc pid:");
	  printint (getpid ());
	  /*Codi de T2 */
	  nice (20);
	  while (1)
	    printf ("T2");
	  break;
	  /*Fi Codi T2 */

	default:
	  printf ("\nTASK1> nou fill amb pid:");
	  printint (ret_fork2);
	  printf (", el meu pid es:");
	  printint (getpid ());

	  ret_fork3 = fork ();
	  switch (ret_fork3)
	    {
	    case 0:
	      printf
		("\nTASK3> fill de TASK1, return fork hauria de ser 0 -> ");
	      printint (ret_fork3);
	      printf (",tinc pid:");
	      printint (getpid ());
	      /*Codi de T3 */
	      nice (30);
	      while (1)
		printf ("T3");
	      break;
	      /*Fi Codi T3 */

	    default:
	      printf ("\nTASK1> nou fill amb pid:");
	      printint (ret_fork3);
	      printf (", el meu pid es:");
	      printint (getpid ());
	      /*Codi de T1 */
	      nice (10);
	      while (1)
		printf ("T1");
	      break;
	      /*Fi Codi T1 */
	    }

	  break;
	}
      break;

    default:
      printf ("\nTASK0> nou fill amb pid:");
      printint (ret_fork1);
      printf (", el meu pid es:");
      printint (getpid ());
      /*Codi de T0 */
      nice (5);
      while (1)
	{
	  get_stats (getpid (), &aux1);
	  printf (" T0> tics:");
	  printint (aux1);
	}
      break;
      /*Fi Codi T0 */
    }
  return 0;
}

/*
  Arbre de processos actual:
              T0
               |
	       T1---->T2
	           |
                   T3
 */

void
run2_jp ()
{
  //int k=0;

   /*EXCEPCIONS*/
    /*GENERAL PROTECTION *//* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
    /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
    /*DIVIDE ERROR */
    //__asm__ __volatile__ ("div %eax");
    /*PAGE FAULT */
    //__asm__ __volatile__ ("movl $0, 5(%esp)");
    /*GENERACIO DE INTERRUPCIONS DES DE CODI D'USUARI */
    /*Acces a una interrupcio de sistema desde usuari (p.ex floating point error)
     *Substituir el nombre de interrupcio per provar-ne d'altres
     */
    //__asm__ __volatile__("int $16");
    /*Acces a una int de usuari (p.ex divide error) */
    //__asm__ __volatile__("int $0");
    /*CRIDES A SISTEMA */
    // Es pot descomentar el perror() del final per veure com queda
    // l'ERRNO.
    // write(1,"Prova d'un write",256);
    //  write(1,"",100);
    //  write(-10,"Hola",10);
    //k = write(1,"Hola",-10);
    //La seguent prova es surt de la consola pero ho fa be
    /*    int k = write(1,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
       "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb\n"
       "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccc\n"
       "dddddddddddddddDDDDDdddDDDdddDDDdddddddDDDDDddDDDdddDDDdd\n"
       "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee\n"
       "fffffffffffffffffffffffffffffffffffffffffffffffffffffffff\n"
       "ggggggggggggggggggggggggggggggggggggggggggggggggggggggggg\n"
       "hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n"
       "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii\n"
       "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj\n"
       "kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk\n"
       "En aquest bloc de text hi ha 683 caracters",1024);

       printf("\n");
       printint(k);

       perror();
     */
    //Bad ADDRESS
    //write(1,k,200);
    //perror();
    //  printint(k);
    //   write(1,"hola",0);
    /*PROVA DE LA FUNCIO PRINTF i PRINTI
       char *text1 = "p";
       int k = printf(text1);
       printf("\nMida bytes escrits: ");
       printint(k);
     */
}
