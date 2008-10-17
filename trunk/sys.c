/*
 * sys.c - Syscalls implementation
 */

#include <devices.h>
#include <segment.h>
#include <utils.h>
#include <errno.h>
#include <sched.h>
#include <mm_address.h>
#include <mm.h>

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
	struct task_struct *p=current();
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

	int fill = search_free_task();
	if (fill==-1) return -EAGAIN;
	
	/* Copiar el task_union del pare al fill */
	copy_data(current(), &task[fill], 4096);
	
	/* Herencia dades usuari*/
	int i;
	//struct task_struct *p=current();
	
	int frames[NUM_PAG_DATA];
	for (i=0; i< NUM_PAG_DATA; i++){
	/* Mirem que hi hagi prou frames lliures */
		frames[i]=alloc_frames(1);
		if (frames[i]==-1) return - EAGAIN;
	}
	
	for(i=0; i<NUM_PAG_DATA; i++){
		
		/* Associa sa pagina logica del pare amb sa pagina fisica o 'frame' del fill(nomes de forma temporal) */
		set_ss_pag(PAG_LOG_INIT_DATA_P0+NUM_PAG_DATA+i, frames[i]);
		
		/* Copiam la pagina del pare al frame del fill, el qual acabam d'associar amb la pagina logica del nou proces*/
		copy_data((char*)(PAGE_SIZE*(PAG_LOG_INIT_DATA_P0+i)), (char*)(PAGE_SIZE*(PAG_LOG_INIT_DATA_P0+NUM_PAG_DATA+i)), PAGE_SIZE);
		
		/* Allibarem les pagines fisiques*/
		del_ss_pag(frames[i]);
		
		/* Guardar la informació sobre els nous marcs de pagines al task_struct del fill */
		task[fill].task.pagines_fisiques[i]=frames[i];
		
		/* Modifiquem el PID del fill mitjançant l'eax, que sera el valor que retornara quan el proces restauri el seu context */
		task[fill].stack[KERNEL_STACK_SIZE-4]=0;
			
		
	}
	
	return 0;
}
