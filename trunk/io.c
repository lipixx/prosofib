/*
 * io.c - 
 */

#include <io.h>

/**************/
/** Screen  ***/
/**************/

#define NUM_COLUMNS 80
#define NUM_ROWS    25

Byte x, y=15;

/* Read a byte from 'port' */
Byte inb (unsigned short port)
{
  Byte v;

  __asm__ __volatile__ (
"inb %w1,%0"
:"=a" (v)
:"Nd" (port)
);
  return v;
}

void printc(char c)
{
  Word ch = (Word) (c & 0x00FF) | 0x0200;
  DWord screen = 0xb8000 + (y * NUM_COLUMNS + x) * 2;

  /*Implementem \n i \t*/
  if (c=='\n')
  {
     x=0;
     y++;
  }
  else
    {
      if (c=='\t')
	{
	  x+=8;
	  if (x >= NUM_COLUMNS)
	    {
	      x = 0;
	      if (++y >= NUM_ROWS)
		y = 0;
	    }
	}
      else
	{
	  if (++x >= NUM_COLUMNS)
	    {
	      x = 0;
	      if (++y >= NUM_ROWS)
		y = 0;
	    }
	  asm(
	      "movw %0, (%1)" 
	      : 
	      : "g"(ch), "g"(screen));
	}
    }
  //Sortida per consola
  __asm__ __volatile__ (
			"movb %0, %%al\n"
			"outb $0xe9"
			::"g"(c)
			); 
  
}

void printk(char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc(string[i]);
}

/**Funcions d'escriptura per pantalla amb coordenades que hem creat*/
void printk_xy(int x, int y, char *string)
{
  int i;
  for (i = 0; string[i]; i++)
    printc_xy(x+i, y, string[i]);
}

void printc_xy(int x, int y, char c)
{
  Word ch = (Word) (c & 0x00FF) | 0x0200;
  DWord screen = 0xb8000 + (y * NUM_COLUMNS + x)*2;
   asm(
  "movw %0, (%1)" 
  : 
  : "g"(ch), "g"(screen));
   
}
