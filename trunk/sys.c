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

int comprova_fd (int fd, int operacio);

int
sys_write (int fd, char *buffer, int size)
{
  /* Params: %ebx, %ecx, %edx */

  int fd_cmp = comprova_fd (fd, ESCRIPTURA);
  if (fd_cmp != 0)
    return -EBADF;
  if (size < 0)
    return -EINVAL;
  if (size > (NUM_PAG_DATA * PAGE_SIZE) - KERNEL_STACK_SIZE)
    return -EFBIG;

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
  int ncPrinted = 0, tempSize = size, writeSize = 0;
  while (tempSize > 0)
    {
      if (tempSize >= 256)
	writeSize = 256;
      else
	writeSize = tempSize;

      copy_from_user (buffer, buff_aux, writeSize);
      ncPrinted += sys_write_console (buff_aux, writeSize);

      buffer += tempSize;
      tempSize -= writeSize;
    }
  if (ncPrinted == size)
    return size;
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
     } */
}

int
comprova_fd (int fd, int operacio)
{
  if (fd == operacio)
    return 0;

  return -EBADR;		/* Invalid request descriptor */
}

int
sys_nice(int quantum)
{
  int old = -EPERM;
  struct task_struct * current_task = current();
  if (quantum > 0)
    {
      old = current_task->quantum;
      current_task->quantum = quantum;
    }
  return old; 
}

int
sys_getpid ()
{
  struct task_struct *p = current ();
  return p->pid;
}


int
sys_fork ()
{
  
  int i,j;
  int frames[NUM_PAG_DATA];
  int fill = search_free_task ();
  if (fill == -1)
    return -EAGAIN;
  
  
  /* Copiar el task_union del pare al fill */
  copy_data (current (), &task[fill], 4096);
  
  /* Herencia dades usuari, mirem que hi hagi prou frames lliures */
  for (j = 0; j<NUM_PAG_DATA && current()->pagines_fisiques[j]!=-1; j++)
    {
      frames[j] = alloc_frames(1);
      if (frames[j] == -1)
	return -EAGAIN;
    }
  
  for (i = 0; i<NUM_PAG_DATA && i<=j; i++)
    {
      
      /* Associa sa pagina logica del pare amb sa pagina fisica o 'frame' 
	 del fill(nomes de forma temporal) */
      set_ss_pag (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA+i, frames[i]);
      
      /* Copiam el frame del pare al frame del fill, el qual acabam 
	 d'associar amb la pagina logica del nou proces */
      copy_data ((void *) (PAGE_SIZE * (PAG_LOG_INIT_DATA_P0 + i)),
		 (void *) (PAGE_SIZE * (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA+i)),
		 PAGE_SIZE);
      
      /* Guardar la informació sobre els nous marcs de pagines al task_struct del fill */
      task[fill].task.pagines_fisiques[i] = frames[i];

      /*Alliberem les pagines fisiques temporals*/
      del_ss_pag(PAG_LOG_INIT_DATA_P0+NUM_PAG_DATA+i);
    }
  set_cr3();  
 

  /* Modifiquem el 'PID' del fill mitjancant l'eax, que sera el valor que retornara 
     quan el proces restauri el seu context. -10 perque quan entrem al sistema 
     s'apila ss,esp,eflags,cs,eip i llavors tots els registres de la macro SAVE_ALL 
     (entry.S). 
  */
  task[fill].stack[KERNEL_STACK_SIZE - 10] = 0;	
  
  
  /* Inicialitzar els camps del task_struct no comuns al fill */
  task[fill].task.pid = pid++;
  task[fill].task.quantum = 60;	/* Tots els processos tindran el mateix quantum */
  task[fill].task.tics_cpu = 0;

  /* Inserir el nou proces a la llista de preparats: runqueue */
  list_add_tail (&(task[fill].task.run_list), &runqueue);

  
  return pid-1;
}

void sys_exit()
{
	int i;
	struct task_struct* proces_actual = current();
	
	/* Seguent es el punter al task_union seguent de la runqueue */
	union task_union *seguent=(union task_union*) list_head_to_task_struct(proces_actual->run_list.next);
	
	if(proces_actual->pid!=0){	/* Mai podem matar el proces 0 */
	
		/* Alliberar les estructures del proces */
	
		proces_actual->pid=-1;	/* Marcam la posició del vector task com a lliure */
	
		list_del(&proces_actual->run_list);	/* Eliminem el proces de la runqueue */

		for(i=0; i < NUM_PAG_DATA; i++)	/* Alliberam les pagines fisiques */
			free_frames(proces_actual->pagines_fisiques[i],1);
		
		/* Posar el seguent element de la runqueue*/
		task_switch(seguent);
	}


}

int sys_get_stats(int pid, int *tics)
{
	int i;

	/* Cercam el proces amb PID=pid */
	for(i=0; i<NR_TASKS && task[i].task.pid!=pid; i++)
	
	if(task[i].task.pid!=pid) return -ESRCH;	/* No such process */
	
	/* Si hi ha el proces amb PID=pid */
	
	if (copy_to_user(&task[i].task.tics_cpu,&tics,4) < 0) return -EFAULT;	/* Bad address */ 
	return 0;
	 
}
