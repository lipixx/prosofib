#include <libc.h>
#include <stdio.h>

/* Funcio print que va escrivint el buffer mitjançant un write
   de 256 en 256 bytes fins que troba el caràcter \0. Retorna
   el numero de caracters escrits.*/
int printf(char *buffer)
{
  int i=0,written=0;

  while (buffer[i] != '\0')
    i++;
  
  /* buff_d = n de vegades que s'ha de fer un write de 256 bytes
     buff_r = n de bytes que queden al final del buffer < 256 bytes
  */
  int buff_d = i/256;
  int buff_r = i%256;
  
  for (i=0; i<buff_d; i++)
    {
      buffer = buffer+(i*256*sizeof(char));
      written = written + write(1,buffer,256);
    }

  buffer = buffer+(i*256*sizeof(char));
 
 if (buff_r != 0)
    written = written + write(1,buffer,buff_r);

 return written;
}

void printint(int i)
{
  char buffer[11] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'};
  itoa(i,buffer);
  printf(buffer);
}

/*int strlen(char *buffer)
{
  int i=0;
   while (buffer[i] != '\0')
    i++;
   return i;
   }*/

void itoa(int n, char *buffer)
{
  int ndigits = n;
  int i = 0;

  if (n ==0) buffer[0] = '0';

  /* Comptem el nombre de digits */
  while (ndigits > 0)
    {
      i++;
      ndigits /= 10;
    }

  
  while (i > 0)
    {		
      buffer[i-1] = (n%10) + '0';
      n /= 10;
      i--;
    }
	
}
