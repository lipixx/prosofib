/*
 * sf.h - Sistem de Fitxers: 
 */
#include <types.h>

#ifndef __SF_H__
#define __SF_H__

#define O_RDONLY 4	/* Basat en el sistema de permissos de UNIX */
#define O_WRONLY 2
#define O_RDWR 6
#define O_CREAT 7
#define MAX_BLOCKS 40	/* Tindrem un disc dur de 1 MBytes */
#define BLOCK_SIZE 256
#define MAX_FILES 10
#define MAX_NAME_LENGTH 10

void init_fat();

/* Sistema de fitxers */
Byte fat[MAX_BLOCKS];
int bloc_de_la_llibertat;

/* Disc */
Byte disk[MAX_BLOCKS][BLOCK_SIZE];
int directori[MAX_FILES]; 

struct file_operations{
int (*sys_open_dev)(const char *, int);
int (*sys_close_dev)(int);
int (*sys_read_dev)(int, char*, int);
int (*sys_write_dev)(int, char*, int);
int a;
};

struct file{
char nom[MAX_NAME_LENGTH];
int mode_acces_valid;
struct file_operations *operations;
int first_block;
int size;
int n_blocs;
};

struct taula_fitxers_oberts {
int refs;
int mode_acces;
int lseek;
struct file* file;
};


#endif /* __SF_H__ */
