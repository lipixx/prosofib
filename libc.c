/*
 * libc.c 
 */

#include <libc.h>

int errno = 0;

/* Wrapper of  write system call*/
int
write (int fd, char *buffer, int size)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n" "movl 12(%%ebp),%%ecx\n" "movl 16(%%ebp),%%edx\n" "movl $4,%%eax\n" "int $0x80\n" "movl %%eax, %0\n":"=g" (val)	/*bind %0 */
			:	/*bind %1,%2,%3 si hi fossin.. */
			:"%ebx"	/*announce to the compiler that ebx is dirty */
			);

  return check_errno (val);
}

int
read (int fd, char *buffer, int size)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n" 
			"movl 12(%%ebp),%%ecx\n" 
			"movl 16(%%ebp),%%edx\n" 
			"movl $3,%%eax\n" 
			"int $0x80\n" "movl %%eax, %0\n"
			:"=g" (val)	/*bind %0 */
			:
			:"%ebx"	/*announce to the compiler that ebx is dirty */
			);
  return check_errno (val);
}

int
getpid (void)
{
  int pid;
  __asm__ __volatile__ ("movl $20,%%eax\n"
			"int $0x80\n" "movl %%eax, %0\n":"=g" (pid)::"%ebx");
  return pid;
}





/* FORK

RETURN VALUES

     Upon successful completion, fork() returns a value of 0 to the child
     process and returns the process ID of the child process to the parent
     process.  Otherwise, a value of -1 is returned to the parent process, no
     child process is created, and the global variable errno is set to indi-
     cate the error.

ERRORS

     Fork() will fail and no child process will be created if:

     [EAGAIN]           The system-imposed limit on the total number of pro-
                        cesses under execution would be exceeded.  This limit
                        is configuration-dependent.

     [EAGAIN]           The system-imposed limit MAXUPRC (<sys/param.h>) on
                        the total number of processes under execution by a
                        single user would be exceeded.

     [ENOMEM]           There is insufficient swap space for the new process.

*/

int
fork (void)
{
  int val = -1;
  __asm__ __volatile__ ("movl $2,%%eax\n"
			"int $0x80\n" "movl %%eax, %0\n":"=g" (val)::"%ebx");

  return check_errno (val);
}

void
exit ()
{
  __asm__ __volatile__ ("movl $1,%%eax\n" "int $0x80\n"	/* Feim la crida al sistema que no retornara mai */
			::);

}

int
get_stats (int pid, int *tics)
{
  int res = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"	/* Primer parametre */
			"movl 12(%%ebp),%%ecx\n"	/* Segon parametre */
			"movl $35,%%eax\n" "int $0x80\n"	/* TRAP */
			"movl %%eax, %0\n":"=g" (res)	/* resultat a res */
			::"%ebx");

  return check_errno (res);
}

int
nice (int quantum)
{
  /*Retorna el val del quantum anterior i -1 si no ha anat be */
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $34,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}


int
sem_init (int n_sem, unsigned int value)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl 12(%%ebp),%%ecx\n"
			"movl $21,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
sem_wait (int n_sem)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $22,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
sem_signal (int n_sem)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $23,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
sem_destroy (int n_sem)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $24,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
check_errno (val)
{
  if (val < 0)
    {
      errno = 0 - val;
      return -1;
    }
  else
    return val;
}

void
perror ()
{

  switch (errno)
    {
    case 1:
      write (1, "EPERM: Operation not permitted\n", 31);
      break;
    case 3:
      write (1, "ESRCH: No such process \n", 24);
      break;
    case 5:
      write (1, "EIO: I/O error \n", 16);
      break;
    case 9:
      write (1, "EBADF: Bad file number \n", 24);
      break;
    case 11:
      write (1, "EAGAIN: Try again \n", 19);
      break;
    case 14:
      write (1, "EFAULT: Bad address \n", 21);
      break;
    case 22:
      write (1, "EINVAL: Invalid argument \n", 26);
      break;
    case 23:
      write (1, "ENFILE: Too many open files in system \n",39);
      break;
    case 27:
      write (1, "EFBIG: File too large \n", 23);
      break;
    case 38:
      write (1, "ENOSYS: Function not implemented \n", 34);
      break;
    case 53:
      write (1, "EBADR: Invalid request descriptor \n", 35);
      break;
    case 77:
      write (1, "EBADFD: File descriptor in bad state \n", 38);
      break;
    default:
      write (1, "Error desconocido\n", 18);
      break;
    }
}
