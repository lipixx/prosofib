#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__


#define CIRCULAR_SIZE 5

#include <list.h>

/*
struct keyq {
  struct list_head *queue;   
  int chars_pendents;
  int buffer;
  int size;
} keyboard_queue;  
*/
struct list_head keyboard_queue;

 
/* keyboard_queue es la cua on tindrem bloquejats els procesos pendents de llegir algun caracter per teclat */
/* Fins que un proces no hagi acabat de fer la lectura de tots els caracters no podra sortir de la cua keyboard_queue */

char buffer_circular[CIRCULAR_SIZE];
int pos;
int inici;
int circ_chars; /* Indica el nombre de caracters que hi ha dins el buffer circular */



#endif /* __KEYBOARD_H__ */
