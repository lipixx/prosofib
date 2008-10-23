/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

int write(int fd, char *buffer, int size);
int getpid(void);
int fork(void);
int nice(int quantum);
void exit();
int get_stats(int pid, int *tics);

void perror();
int check_errno(int i);
#endif  /* __LIBC_H__ */
