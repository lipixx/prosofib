#include <libc.h>
#include <stdio.h>

void run2_jp ();
void runjp ();

int __attribute__ ((__section__ (".text.main"))) main (void)
{

  printf ("\nBenvinguts a ZeOS!");

  int i = fork ();
  printf("\nContador pid actual: ");
  printint(i);

  if (i!=0)
    {
      printf("\nSoc el pare i el meu fill te PID: ");
      printint(i);
      printf("\nJo tenc PID:");
    }
  else printf("\nSoc el fill i tenc PID: ");

  i = getpid();
  printint(i);
  
//Jocs de proves:
  // runjp(); 
  // run2_jp();

  while (1);
  return 0;
}

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
