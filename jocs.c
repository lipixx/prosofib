#include <libc.h>
#include <stdio.h>
#include <jocs.h>

/*
int
joc (void)
{

  int provar_fork ();

  while (1);
  return 0;
}
*/

int
provar_fork (void)
{
  int p;
  int fill = fork ();
  if (fill = 0)
    {
      printf ("\nSoc el pare, i el meu pid es:");
      p = getpid ();
      printint (p);
    }
  else
    {
      printf ("\nSoc el fill, i el meu pid es:");
      p = getpid ();
      printint (p);
      exit ();
    }
  return 0;
}

int
provar_get_stats (void)
{
  int tics, mipid = getpid ();

  get_stats (-1, &tics);
  printf ("\nPID incorrecte: -1");
  printint (tics);
  get_stats (mipid, (int *) 0);
  printf ("\n@ invalida: @tics=0");
  printint (tics);
  get_stats (mipid, (int *) 1000);
  printf ("\n@tics=1000");
  printint (tics);
  get_stats (mipid, &tics);
  printf ("\nEl meu pid es:");
  printint (tics);

}

int
provar_exit ()
{
  int p;
  int fill = fork ();
  if (fill = 0)
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
  return 0;
}
