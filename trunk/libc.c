/*
 * libc.c 
 */

#include <libc.h>
#include <sf.h>

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
unlink (const char * path)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $10,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
open (const char * path, int flags)
{
  int res = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl 12(%%ebp),%%ecx\n"
			"movl $5,%%eax\n" "int $0x80\n"
			"movl %%eax, %0\n":"=g" (res)
			::"%ebx");
  return check_errno (res);
}

int
close (int fd)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $6,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
dup (int fd)
{
  int val = -1;
  __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl $41,%%eax\n"
			"int $0x80\n" "movl %%eax,%0\n":"=g" (val)::"%ebx");
  return check_errno (val);
}

int
readdir (struct dir_ent * buffer, int offset)
{
  int res = -1;
    __asm__ __volatile__ ("movl 8(%%ebp),%%ebx\n"
			"movl 12(%%ebp),%%ecx\n"
			"movl $141,%%eax\n" "int $0x80\n"
			"movl %%eax, %0\n":"=g" (res)
			::"%ebx");
  return check_errno (res);
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
    case 2:
      write (1, "ENOENT: No such file or directory\n",34);
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
    case 16:
      write (1, "EBUSY: Device or resource busy\n",31);
      break;
    case 19:
      write (1, "ENODEV: No such device\n",23);
      break;
    case 22:
      write (1, "EINVAL: Invalid argument \n", 26);
      break;
    case 23:
      write (1, "ENFILE: Too many open files in system \n",39);
      break;
    case 24:
      write (1, "EMFILE: Too many open files by the proces \n",44);
      break;
    case 27:
      write (1, "EFBIG: File too large \n", 23);
      break;
    case 28:
       write (1, "ENOSPC: No space left on device\n",32);
      break;
    case 29:
      write (1, "ESPIPE: Illegal Seek\n", 21);
      break;
    case 30:
      write (1, "EROFS: RO File System\n", 22);
      break;
    case 31:
      write (1, "EMLINK: Too many links\n", 23);
      break;
    case 36:
      write (1, "ENAMETOOLONG: File name too long\n", 33);
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
      write (1, "Unknown Error\n", 14);
      break;
    }
}
