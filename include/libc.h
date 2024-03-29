/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */

#ifndef __LIBC_H__
#define __LIBC_H__

#include <sf.h>

int write (int fd, char *buffer, int size);
int read (int fd, char *buffer, int size);
int getpid (void);
int fork (void);
int nice (int quantum);
int get_stats (int pid, int *tics);
int open (const char * path, int flags);
int close (int fd);
int dup (int fd);
int unlink (const char * path);
int readdir (struct dir_ent * buffer, int offset);
int sem_init (int n_sem, unsigned int value);
int sem_wait (int n_sem);
int sem_signal (int n_sem);
int sem_destroy (int n_sem);
void exit ();

void perror ();
int check_errno (int i);
#endif /* __LIBC_H__ */
