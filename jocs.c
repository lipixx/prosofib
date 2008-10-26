#include <libc.h>
#include <stdio.h>
#include <jocs.h>


void
provar_fork (int num_fills)
{
  int i, p, fill=0;
  for (i = 0; i < num_fills; i++)
      fill = fork();

  if (fill == 0)
    {
      printf ("\nPID pare:");
      p = getpid ();
      printint (p);
    }
  else
    {
      printf ("\nPID fill:");
      p = getpid ();
      printint (p);
      exit ();
    }
  printf ("Total fills creats: ");
  printint(i);
}

void
provar_get_stats (void)
{
  int tics = 0;
  int mipid = getpid ();

  get_stats (-1, &tics);
  printf ("\nPID incorrecte (-1): ");
  printint (tics);

  get_stats (30, &tics);
  printf("\nNo existeix el proces 30: ");
  printint (tics);

  get_stats (mipid, (int *) 0);
  printf ("\n@tics invalida (0): ");
  printint (tics);

  /*  get_stats (mipid, (int *) 1000);
  printf ("\n@tics=1000");
  printint (tics);*/

  get_stats (mipid, &tics);
  printf ("\nEl meu pid es:");
  printint (tics);

}

void
provar_exit ()
{
  int p;
  int fill = fork ();
  if (fill == 0)
    {
      printf ("\nSoc el pare, i el meu pid es:");
      p = getpid ();
      printint (p);
      printf ("\nAra em disposo a suicidar-me, cosa que no podria fer...");
      exit ();
    }
  else
    {
      printf ("\nSoc el fill, i el meu pid es:");
      p = getpid ();
      printint (p);
      printf ("\nJo com que som un matat (mai tan ben dit...) em suicidare");
      exit ();
    }
}

void
provar_nice (void)
{
  
  printf ("\nProvar nice correcte = 12 ");
  nice (12);
  
  printf ("\nProvar nice incorrecte = -1");
  nice (-1);
  
  printf ("\nProvar nice incorrecte = 0");
  nice(0);
}

void
provar_switch (void)
{
  int ret_fork1, ret_fork2, ret_fork3, aux1;	
  ret_fork1 = fork ();
  switch (ret_fork1)
    {
    case 0:
      printf (" \nTASK1 > fill de TASK0, return fork hauria de ser 0->");
      printint (ret_fork1);
      printf (", tinc pid: ");
      printint (getpid ());
	/*Codi de T1-AF */
	/*Fi Codi T1-AF */
	
	ret_fork2 = fork ();
	switch (ret_fork2)
	  {
	case 0:
	  printf (" \nTASK2 > fill de TASK1, return fork hauria de ser 0->");
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
	    printf (" \nTASK1 > nou fill amb pid: ");
	    printint (ret_fork2);
	    printf (", el meu pid es: ");
	    printint (getpid ());
	    
	    ret_fork3 = fork ();
	    switch (ret_fork3)
	      {
	      case 0:
		printf
		  (" \nTASK3 > fill de TASK1, return fork hauria de ser 0->");
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
		printf (" \nTASK1 > nou fill amb pid: ");
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
      printf (" \nTASK0 > nou fill amb pid: ");
      printint (ret_fork1);
      printf (", el meu pid es: ");
      printint (getpid ());
	/*Codi de T0 */
	nice (5);
	while (1)
	  {
	    get_stats (getpid (), &aux1);
	    printf (" T0 > tics:");
	    printint (aux1);
	  }
	break;
	  /*Fi Codi T0 */
	  }  
}

void provar_semafors()
{
  int aux;	
  printf("\nCreacio,suma,resta,destruccio dels semafors: ");
  printf("\nCreant sem_init(-1,10):");
  sem_init(-1,10);
  printf("\nCreant sem_init(0,3)");
  sem_init(0,3);
  printf("\nFent sem_wait(-1): ");
  sem_wait(-1);
  printf("\nFent sem_signal(-1): ");
  sem_signal(-1);
  printf("\nFent sem_wait(0) (soc el task0):");
  sem_wait(0);
  printf("\nFent sem_signal(0)");
  sem_signal(0);
  printf("\nFent sem_destroy(0)");
  sem_destroy(0);
  printf("\nCreant sem_init(0,1)");
  sem_init(0,1);
  
  printf("\nFem un fork i sem_wait al fill");
  aux = fork();
  if (aux == 0)
    {
      printf("\nFill> Hauria de quedarme en un bucle imprimint");
      printf(" pero ara fare sem_wait");
      sem_wait(0);
      while(1) printf("F");
    }
  
  printf("\nEl fill esta parat");
  for(aux=0;aux<1000;aux++); //Esperem
  printf("\nIntento destruir el semafor: ");
  sem_destroy(0);
  printf("\nEl faix continuar amb sem_init, adeu.");
  sem_signal(0);
  sem_destroy(0);
  while(1) printf("P");
}
