/*
 * sf.h - Sistem de Fitxers: 
 */
#include <types.h>
#include <sched.h>

#ifndef __SF_H__
#define __SF_H__

/* From Linux Kernel 2.4
 * include/asm-i386/fcntl.h

 * Note that the flag value (low two bits) for sys_open means:
 *	00 - read-only
 *	01 - write-only
 *	10 - read-write
 */
#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_CREAT		   0100	/* not fcntl */

#define MAX_BLOCKS 40	/* Tindrem un disc dur de 1 MBytes */
#define BLOCK_SIZE 256
#define MAX_FILES 10
#define MAX_NAME_LENGTH 10

void init_filesystem();

/* Sistema de fitxers */
int fat[MAX_BLOCKS];

//El bloc_de_la_llibertat sempre apunta 
//al primer bloc lliure de la taula fat.
int bloc_de_la_llibertat;


struct fitxers_oberts * taula_fitxers_oberts[NUM_CANALS*NR_TASKS];

/* Disc */
int disk[MAX_BLOCKS][BLOCK_SIZE];
struct file * directori[MAX_FILES]; 

struct file_operations{
  int (*sys_open_dev)(const char *, int);
  int (*sys_close_dev)(int);
  int (*sys_read_dev)(int, char*, int);
  int (*sys_write_dev)(int, char*, int);
};

//Si mode_acces_valid = -1 file no es valid
struct file{
  char nom[MAX_NAME_LENGTH];
  int mode_acces_valid;
  struct file_operations *operations;
  int first_block;
  int size;
  int n_blocs;
};

struct fitxers_oberts {
  int refs;
  int mode_acces;
  int lseek;
  struct file * opened_file;
};


#endif /* __SF_H__ */
