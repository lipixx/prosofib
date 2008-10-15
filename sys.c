/*
 * sys.c - Syscalls implementation
 */

#include <devices.h>
#include <segment.h>
#include <utils.h>
#include <errno.h>
#include <sched.h>

#define LECTURA 0
#define ESCRIPTURA 1

int comprova_fd(int fd, int operacio);

int sys_write(int fd,char *buffer, int size)
{
  /* Params: %ebx, %ecx, %edx */
  
  int fd_cmp = comprova_fd(fd,ESCRIPTURA); 
  if (fd_cmp != 0) return -EBADF;
  if (size < 0) return -EINVAL;
  if (size > (NUM_PAG_DATA*PAGE_SIZE)-KERNEL_STACK_SIZE) return -EFBIG;

  /* COMPROVACIO PUNTER USUARI -
     Desde el final del codi de l'usuari, fins a
     l'inici de la pila d'usuari:
     Si la &buffer < Inici_Data_Usuari 
     || &buffer+mida a escriure >= Fi_Data_Usuari
     (que es el mateix que on comença l'USER_ESP)
     hi ha que retornar un error.
  */

  /*  if (buffer < (char*)(int*)(L_USER_START+(NUM_PAG_CODE*4096))) 
    return -EFAULT;
  if (buffer+(size*4) >= (char*)(int*)USER_ESP)
    return -EFAULT;
  */

  char buff_aux[256];
  int ncPrinted = 0, tempSize=size, writeSize=0;
  while (tempSize > 0){
    if (tempSize >= 256 ) writeSize=256;
    else writeSize=tempSize;
    
    copy_from_user(buffer,buff_aux,writeSize);
    ncPrinted += sys_write_console(buff_aux,writeSize);
    
    buffer +=tempSize;
    tempSize -= writeSize;
  }
  if (ncPrinted == size) return size;
  return -EIO;
  
  /*Altre manera...
  while (size > 0)
    {
      if (size >= 256)
	{
	  copy_from_user(buffer,buff_aux,256);
	  ncPrinted = sys_write_console(buff_aux,256);
	}
      else 
	{
	  copy_from_user(buffer,buff_aux,size);
	  ncPrinted = sys_write_console(buff_aux,size);
	}
      
      buffer += ncPrinted;
      size -= ncPrinted;
      result += ncPrinted;
    }
  return result;
  }*/
}

int comprova_fd(int fd,int operacio)
{
  if (fd == operacio) 
    return 0;

  return -EBADR;	/* Invalid request descriptor */
}

int sys_getpid(){
	struct task_struct * p=current();
	return p->pid;
}

int search_free_task(){
int i;
	for(i=0; i< NR_TASKS; i++){
		if(task[i].task.pid<0) return i;
	}
	return -1;
}
int sys_fork(){

	int t = search_free_task();
	if (t==-1) return -EAGAIN;
	
	copy_data();
	struct task_union fill = 
}
