/*
 * libc.c 
 */

#include <libc.h>

int errno = 0;

/* Wrapper of  write system call*/
int write(int fd,char *buffer,int size)
{
  int val = -1;
  __asm__ __volatile__(
     "movl 8(%%ebp),%%ebx\n"
     "movl 12(%%ebp),%%ecx\n"
     "movl 16(%%ebp),%%edx\n"
     "movl $4,%%eax\n"
     "int $0x80\n"
     "movl %%eax, %0\n"
     : "=g" (val)  /*bind %0*/
     :             /*bind %1,%2,%3 si hi fossin..*/
     : "%ebx"      /*announce to the compiler that ebx is dirty*/
     
);

  if (val < 0){
    errno = 0-val;
    perror();
    return -1;
  }
  else return val;

}

int getpid(void)
{
  int pid;
  __asm__ __volatile__(
		      "movl $20,%%eax\n"
		      "int $0x80\n"
		      "movl %%eax, %0\n"
		      : "=g"(pid)
		      :
		      : "%ebx"
		       );
}


void perror()
{
  
  switch(errno)
    {
    case 5: write(1,"EIO: I/O error \n",16); break;
    case 9: write(1,"EBADF: Bad file number \n",24); break;
    case 14: write(1,"EFAULT: Bad address \n",21); break;
    case 22: write(1,"EINVAL: Invalid argument \n",26);break;
    case 27: write(1,"EFBIG: File too large \n",23);break;
    case 38: write(1,"ENOSYS: Function not implemented \n",34);break;
    case 53: write(1,"EBADR: Invalid request descriptor \n",35);break;
    case 77: write(1,"EBADFD: File descriptor in bad state \n",38);break;
    default: write(2,"Error desconocido\n",18);break;
    }
}
