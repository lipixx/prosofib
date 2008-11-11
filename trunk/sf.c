/*
 * sf.c - Sistem de Fitxers: 
 */

#include <sf.h>

void init_fat()
{
  int i;
  bloc_de_la_llibertat=0;
  
  for(i=0; i<MAX_BLOCKS-1; i++) fat[i]=i+1;
  
  fat[MAX_BLOCKS-1]=-1;
}

