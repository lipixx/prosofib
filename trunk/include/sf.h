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
#define MAX_OPEN_FILES 15 //NUM_CANALS*NR_TASKS

void init_filesystem();
struct file * create_file(const char * path);
int freespace();
int balloc (int nblocks);
int freed (int ibloc0);
void add_block (int blocLast, int blocAdded);
int last_block (int bloc0);

struct dir_ent {
  char nom[MAX_NAME_LENGTH];
  short int size;
};

/* Fat */
int fat[MAX_BLOCKS];
//El bloc_de_la_llibertat sempre apunta 
//al primer bloc lliure de la taula fat.
int bloc_de_la_llibertat;

/* Disc */
Byte disk[MAX_BLOCKS][BLOCK_SIZE];


/* Estructures de la Mort */
struct file_operations{
  int (*sys_open_dev)(const char *, int);
  int (*sys_close_dev)(int);
  int (*sys_read_dev)(int, char*, int);
  int (*sys_write_dev)(int, char*, int);
} ops[MAX_FILES];

//Tenim un directori de files.
//Si n_refs = -1, file no es valid.
struct file{
  char * nom;
  int mode_acces_valid;
  struct file_operations * operations;
  int first_block;
  short int size;
  int n_blocs;
  int n_refs;
} directori[MAX_FILES];

//Tenim una taula_fitxers_oberts de fitxers_oberts
struct fitxers_oberts {
  int refs;
  int mode_acces;
  int lseek;
  struct file * opened_file;
} taula_fitxers_oberts[MAX_OPEN_FILES];


#endif /* __SF_H__ */
