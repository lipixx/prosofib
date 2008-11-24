#include <libc.h>
#include <stdio.h>
#include <jocs.h>
#include <list.h>
#include <sched.h>
#include <interrupt.h>
#include <sf.h>

void
provar_open ()
{
  int i, k;

  k = fork ();
  if (k == 0)
    {
      open ("keyboard", O_RDONLY);

      open ("keyboard", O_WRONLY);
      printf ("\nResult == EPERM?:");
      perror ();

      open ("console", O_RDWR);
      printf ("\nResult == EPERM?:");
      perror ();

      for (i = 0; i < NUM_CANALS; i++)
	{
	  open ("keyboard", O_RDONLY);
	}
      printf ("\nMAX_OP_FILES REACHED?:");
      perror ();
    }
  else
    {
      printf ("\nProvem a crear 3 nous fitxers, error?:");
      open ("patata", O_RDWR | O_CREAT);
      open ("comino", O_RDWR | O_CREAT);
      open ("patata", O_RDWR | O_CREAT);
      perror ();
    }
}

void
provar_dup ()
{

  int i;
  dup (0);
  dup (1);
  dup (2);
  printf ("\nNo error al dup 0 1 i 2:");
  perror ();
  for (i = 0; i < 20; i++)
    {
      dup (i);
    }
  printf ("\nMassa canals: ");
  perror ();
}

void
provar_close ()
{
  int fd;
  fd = 0;
  fd = open ("patata", O_RDWR | O_CREAT);
  close (fd);
  printf ("\nObrim nou fitxer i el tanquem (no error..):");
  perror ();
  close (0);
  printf ("\nTanquem el kbd (prova d'escriure..) -> no error:");
  perror ();
  close (5);
  printf ("\nIntent de tancar canal no obert: ");
  perror ();
  close (100);
  printf ("\nIntent de tancar canal out of range:");
  perror ();
}

void
provar_write_disp ()
{
  int fd, fd2;
  struct dir_ent buffer;

  printf("Prova escriptura kbd\n");
  write (0,"Hola",4);
  perror();
  printf("\nProva escriptura fitxer\n");
  fd = open ("patata", O_RDWR | O_CREAT);

  printf("\nLs primer: ");
  readdir(&buffer,2);
  printf(buffer.nom);
  printf(" ");
  printint(buffer.size);

  write (fd, "CADENA", 6);
  write (fd, "Kontinuacio", 11);
  write (fd, "12345678901234567890", 20);
  fd2 = open ("comino", O_RDWR | O_CREAT);
  write (fd2,
	 "INICI678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678900FINAL",
	 256);
  write (fd2,
	 "INICI67890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890FINAL",
	 255);
  write (fd2,
	 "INICI67890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890FINALTOTAL",
	 260);
  close (fd);
  close (fd2);
  printf("\nAra escrivim.....OK!\n");
  printf("\nProva sys_write_console:\n");
  
  printf("\nProva escriptura sense APPEND");
  fd = open("patata", O_RDWR);

  printf("\nLs intermedi: ");
  readdir(&buffer,2);
  printf(buffer.nom);
  printf(" ");
  printint(buffer.size);

  printf("\nEscrivim 5 chars");
  write(fd,"12345",5);
  printf("\nLs ultim: ");
  readdir(&buffer,2);
  printf(buffer.nom);
  printf(" ");
  printint(buffer.size);  
}

void
provar_read_disp ()
{
  printf ("/nHan de sortir 3 inicis 2 finals i 1 final total\n");
  int fd;
  char buffer[256 + 255 + 260];
  provar_write_disp ();
  fd = open ("comino", O_RDONLY);
  read (fd, buffer, 256);
  read (fd, buffer + 256, 255);
  read (fd, buffer + 256 + 255, 260);
  write (1, buffer, 256 + 255 + 260);
  close (fd);
}

void
provar_unlink ()
{
  int fd;
  printf ("\nProvam write i read per provar UNLINK\n");
  printf ("\n-------------------------------------\n");
  provar_write_disp ();
  provar_read_disp ();
  fd = open ("comino", O_RDWR);
  unlink ("comino");
  unlink ("patata");
  printf ("\n-------------------------------------\n");
  printf ("\nError BUSY:");
  perror ();
  printf ("\nProva OK!");
  close (fd);

}

void
provar_readdir ()
{
  struct dir_ent buffer;
  int fd, fd2, i;

  fd = open ("patata", O_RDWR | O_CREAT);
  fd2 = open ("cuminu", O_RDWR | O_CREAT);
  printf
    ("\nAtencio, les repeticions son normals! nomes mirar noms diferents");
  printf ("\n\nDIRECTORI amb 4 fitxers");
  for (i = 0; i < MAX_FILES; i++)
    {
      readdir (&buffer, i);
      printf ("\nfile:   ");
      printf (buffer.nom);
      printf ("      ");
      printint (buffer.size);
    }
  close (fd);

  printf ("\nTancat patata, pero no eliminat");
  for (i = 0; i < MAX_FILES; i++)
    {
      readdir (&buffer, i);
      printf ("\nFile/Dev: ");
      printf (buffer.nom);
      printf ("      ");
      printint (buffer.size);
    }
  unlink ("patata");

  printf ("\n\nEliminat patata");
  for (i = 0; i < MAX_FILES; i++)
    {
      readdir (&buffer, i);
      printf ("\nFile/Dev: ");
      printf (buffer.nom);
      printf ("      ");
      printint (buffer.size);
    }
  printf ("\n\nEscrivim 20b a cuminu:");
  write (fd2, "12345678901234567890", 20);
  for (i = 0; i < MAX_FILES; i++)
    {
      readdir (&buffer, i);
      printf ("\nFile/Dev: ");
      printf (buffer.nom);
      printf ("      ");
      printint (buffer.size);
    }

  printf ("\n\n PROVA OK!");
}

void
provar_fork (int num_fills)
{
  int i, p, fill[num_fills];
  for (i = 0; i < num_fills; i++)
    {
      fill[i] = fork ();
      printf ("\nReturn del fork: ");
      printint (fill[i]);

      if (fill[i] == 0)
	{
	  printf (" -> Soc el fill, tinc PID: ");
	  p = getpid ();
	  printint (p);
	  exit ();
	}
      else
	{
	  printf (" -> Soc el pare, tinc PID: ");
	  p = getpid ();
	  printint (p);
	}
    }
  printf ("\nTotal fills creats: ");
  printint (i);
  printf ("\n");
  while (1);
}

void
provar_get_stats (void)
{
  int fill, tics = 0;
  int mipid = getpid ();

  printf ("\nPID incorrecte (-1): ");
  get_stats (-1, &tics);
  printint (tics);		/* Si es incorrecte imprimim un 0 per pantalla, sino el numero de tics que el proces ha gastat de cpu */

  printf ("\nNo existeix el proces 30: ");
  get_stats (30, &tics);
  printint (tics);		/* Si es incorrecte imprimim un 0 per pantalla, sino el numero de tics que el proces ha gastat de cpu */

  printf ("\n@tics invalida: @tics=0: ");
  get_stats (mipid, (int *) 0);
  printint (tics);		/* Si es incorrecte imprimim un 0 per pantalla, sino el numero de tics que el proces ha gastat de cpu */

  printf ("\n@tics invalida: @tics=1000: ");
  get_stats (mipid, (int *) 1000);
  printint (tics);

  fill = fork ();
  if (fill == 0)
    {
      printf ("\nNumero total de tics del proces 0: ");
      get_stats (mipid, &tics);
      printint (tics);
      printf ("\n");
    }
  else
    {

      printf ("\nNumero total de tics del FILL: ");
      get_stats (getpid (), &tics);
      printint (tics);
      printf ("\n");
    }

  while (1);
}

void
provar_exit ()
{
  int p;
  int fill = fork ();

  if (fill == 0)
    {
      printf ("\nSoc el fill, i el meu pid es: ");
      p = getpid ();
      printint (p);
      printf ("\nJo el fill, em suicidare\n");
      exit ();
    }
  else
    {
      printf ("\nSoc el pare, i el meu pid es: ");
      p = getpid ();
      printint (p);
      printf ("\nAra em disposo a suicidar-me, cosa que no podria fer...");
      exit ();
    }
  while (1);
}

void
provar_nice (void)
{

  printf ("\nProvar nice correcte = 12 ");
  nice (12);

  printf ("\nProvar nice incorrecte = -1 ");
  nice (-1);

  printf ("\nProvar nice incorrecte = 0 ");
  nice (0);

  while (1);
}

void
provar_switch (void)
{
  int ret_fork1, ret_fork2, ret_fork3;
  ret_fork1 = fork ();
  switch (ret_fork1)
    {
    case 0:
      printf (" \nTASK1> fill de TASK0, return fork hauria de ser 0->");
      printint (ret_fork1);
      printf (", tinc pid: ");
      printint (getpid ());
      /*Codi de T1-AF */
      /*Fi Codi T1-AF */

      ret_fork2 = fork ();
      switch (ret_fork2)
	{
	case 0:
	  printf (" \nTASK2> fill de TASK1, return fork hauria de ser 0->");
	  printint (ret_fork2);
	  printf (", tinc pid: ");
	  printint (getpid ());
	  /*Codi de T2 */
	  nice (20);
	  while (1)
	    printf (" T2 ");
	  break;
	  /*Fi Codi T2 */

	default:
	  printf (" \nTASK1> nou fill amb pid: ");
	  printint (ret_fork2);
	  printf (", el meu pid es: ");
	  printint (getpid ());

	  ret_fork3 = fork ();
	  switch (ret_fork3)
	    {
	    case 0:
	      printf
		(" \nTASK3> fill de TASK1, return fork hauria de ser 0->");
	      printint (ret_fork3);
	      printf (", tinc pid: ");
	      printint (getpid ());
	      /*Codi de T3 */
	      nice (30);
	      while (1)
		printf (" T3 ");
	      break;
	      /*Fi Codi T3 */

	    default:
	      printf (" \nTASK1> nou fill amb pid: ");
	      printint (ret_fork3);
	      printf (", el meu pid es: ");
	      printint (getpid ());
	      /*Codi de T1 */
	      nice (10);
	      while (1)
		printf (" T1 ");
	      break;
	      /*Fi Codi T1 */
	    }

	  break;
	}
      break;

    default:
      printf (" \nTASK0> nou fill amb pid: ");
      printint (ret_fork1);
      printf (", el meu pid es: ");
      printint (getpid ());
      /*Codi de T0 */
      nice (5);
      while (1)
	printf (" T0 ");
      break;
      /*Fi Codi T0 */
    }
}

void
provar_semafors ()
{
  int aux;
  printf ("\nCreacio,suma,resta,destruccio dels semafors: ");
  printf ("\nCreant sem_init(-1,10):");
  sem_init (-1, 10);
  printf ("\nCreant sem_init(0,3)");
  sem_init (0, 3);
  printf ("\nFent sem_wait(-1): ");
  sem_wait (-1);
  printf ("\nFent sem_signal(-1): ");
  sem_signal (-1);
  printf ("\nFent sem_wait(0) (soc el task0):");
  sem_wait (0);
  printf ("\nFent sem_signal(0)");
  sem_signal (0);
  printf ("\nFent sem_destroy(0)");
  sem_destroy (0);
  printf ("\nCreant sem_init(1,0)");
  sem_init (1, 0);

  printf ("\nFem un fork i sem_wait al fill");
  aux = fork ();
  if (aux == 0)
    {
      printf ("\nFill> Hauria de quedarme en un bucle imprimint");
      printf (" pero ara fare sem_wait");
      sem_wait (1);
      while (1)
	printf ("F");
    }
  else
    {
      printf ("\nEl fill esta parat");
      printf ("\nIntento destruir el semafor: ");
      sem_destroy (0);
      for (aux = 0; aux < 2000000; aux++)
	printf ("");		/* Mentre el fill no estigui bloquejat no continuar */
      printf ("\nEl faix continuar amb sem_init, adeu.");
      sem_signal (0);
      sem_destroy (0);
      while (1)
	printf ("P");
    }
}


/*	JOCS DE PROVES DE LA PRIMERA ENTREGA		*/

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
    /*   int k = write(1,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n"
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
    while (1);
}
