/*
 * libc.h - macros per fer els traps amb diferents arguments
 *          definició de les crides a sistema
 */
 
#ifndef __LIBC_H__
#define __LIBC_H__

int write(int fd, char *buffer, int size);
int getpid(void);
void perror();

#endif  /* __LIBC_H__ */
