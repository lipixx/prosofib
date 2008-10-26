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
#include <interrupt.h>

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
sys_nice (int quantum)
{
  int old = -EPERM;
  struct task_struct *current_task = current ();
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

  int i, j;
  int frames[NUM_PAG_DATA];
  int fill = search_free_task ();
  if (fill == -1)
    return -EAGAIN;


  /* Copiar el task_union del pare al fill */
  copy_data (current (), &task[fill], 4096);

  /* Herencia dades usuari, mirem que hi hagi prou frames lliures */
  for (j = 0; j < NUM_PAG_DATA && current ()->pagines_fisiques[j] != -1; j++)
    {
      frames[j] = alloc_frames (1);
      if (frames[j] == -1)
	return -EAGAIN;
    }

  for (i = 0; i < NUM_PAG_DATA && i <= j; i++)
    {

      /* Associa sa pagina logica del pare amb sa pagina fisica o 'frame' 
         del fill(nomes de forma temporal) */
      set_ss_pag (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i, frames[i]);

      /* Copiam el frame del pare al frame del fill, el qual acabam 
         d'associar amb la pagina logica del nou proces */
      copy_data ((void *) (PAGE_SIZE * (PAG_LOG_INIT_DATA_P0 + i)),
		 (void *) (PAGE_SIZE *
			   (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i)),
		 PAGE_SIZE);

      /* Guardar la informació sobre els nous marcs de pagines al task_struct del fill */
      task[fill].task.pagines_fisiques[i] = frames[i];

      /*Alliberem les pagines fisiques temporals */
      del_ss_pag (PAG_LOG_INIT_DATA_P0 + NUM_PAG_DATA + i);
    }
  set_cr3 ();


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


  return pid - 1;
}

void
sys_exit ()
{
  int i;
  struct task_struct *proces_actual = current ();

  /* Seguent es el punter al task_union seguent de la runqueue */
  union task_union *seguent =
    (union task_union *) list_head_to_task_struct (proces_actual->run_list.
						   next);

  if (proces_actual->pid != 0)
    {
      /* Mai podem matar el proces 0 */
      /* Alliberar les estructures del proces */

      proces_actual->pid = -1;	/* Marcam la posició del vector task com a lliure */

      list_del (&proces_actual->run_list);	/* Eliminem el proces de la runqueue */

      for (i = 0; i < NUM_PAG_DATA; i++)	/* Alliberam les pagines fisiques */
	free_frames (proces_actual->pagines_fisiques[i], 1);

      /* Posar el seguent element de la runqueue */
      task_switch (seguent);
    }else printk("\nERROR: El pare no es pot suicidar!");


}

int
sys_get_stats (int spid, int *tics)
{
  int i = 0;
	
if(spid<0 || spid > NR_TASKS) return -ESRCH;		/* No such process */

  /* Cercam el proces amb PID=spid */
  for (i = 0; i < NR_TASKS && task[i].task.pid != spid; i++);

  if (task[i].task.pid != spid)
    return -ESRCH;		/* No such process */

  /* Si hi ha el proces amb PID=spid */

  return copy_to_user (&(task[i].task.tics_cpu), tics, 4);
 
}

int
sys_sem_init (int n_sem, unsigned int value)
{
  /* inicialitzam el comptador del semafor n_sem a value */
  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX)
    return -EINVAL;		/* Error si l'identificador m_sem es invalid */
  if (sem[n_sem].init == 1)
    return -EBUSY;		/* Error si el semafor n_sem ja esta inicialitzat -> Device or resource busy */
    				/* init=0 => NO inicialitzat, init=1 => SI inicialitzat */

  sem[n_sem].count = value;
  sem[n_sem].init = 1;
  INIT_LIST_HEAD (&(sem[n_sem].queue));

  return 0;
}

int
sys_sem_wait (int n_sem)
{
	/* Si el comptador del semafor n_sem es mes petit o igual que zero,
	aquesta crida bloqueja en aquest semafor el proces que la ha invocat. 
	En cas que el comptador sigui mes gran que zero, aquesta crida decrementa 
	el valor del semafor. El proces 0 no es pot bloquejar en cap cas, per tant
	s’ha de considerar un error la utilització d’aquesta crida pel procés 0.*/
  
  struct task_struct *old_task;
  union task_union *new_task;

  old_task = current ();
  int p = sys_getpid();

//  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX || sem[n_sem].init == 0)
    if (n_sem < 0 ){ printk("1"); return -EINVAL; }
    else if (n_sem >= SEM_VALUE_MAX ){ printk("2"); return -EINVAL; }
    else if(sem[n_sem].init == 0){ printk("3"); return -EINVAL; }

  if (old_task->pid == 0)
    return -EPERM;

  if (sem[n_sem].count <= 0)
    {
      /*Eliminam de la runqueue i el posem a la cua de bloqued del semafor */
      list_del (&(old_task->run_list));
      list_add_tail (&(old_task->run_list), &(sem[n_sem].queue));

      /*Fem un task_switch a una nova tasca */
      new_task =
	(union task_union *) (list_head_to_task_struct (runqueue.next));
      vida = new_task->task.quantum;
      call_from_int = 0;
      task_switch (new_task);
    }
    else sem[n_sem].count--; 

  return 0;
}

int
sys_sem_signal (int n_sem)
{

	/* Si no hi ha cap proces bloquejat en el semafor n_sem aleshores 
	aquesta crida incrementa el comptador del semafor n_sem. En el cas 
	que hi hagi un o mes processos bloquejats sobre n_sem, aquesta crida
	desbloqueja el primer proces.*/
	
  struct list_head *blocked_task;

  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX || sem[n_sem].init == 0)
    return -EINVAL;

  if (!list_empty (&(sem[n_sem].queue)))
    {
      blocked_task = sem[n_sem].queue.next;
      list_del (blocked_task);
      list_add (blocked_task, &runqueue);

      /*Hi ha que mirar que decidim; que es fa amb el proces que
         desbloquejem? El deixem nomes a run_queue o l'executem
         instantaniament? */
      //Per ara el posem nomes a run_queue
      // JOSEP: Evidentment a sa run_queue
    }else sem[n_sem].count++;

  return 0;
}

int
sys_sem_destroy (int n_sem)
{
  /* destrueim el semafor n_sem si aquest esta inicialitzat */
  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX)
    return -EINVAL;		/* Error si l'identificador m_sem es invalid */
  if (sem[n_sem].init == 0 || !list_empty (&sem[n_sem].queue))
    return -EPERM;		/* Error si el semafor no esta inicialitzat,
				   Error si encara hi ha processos bloquejats a la cua */
  sem[n_sem].init = 0;
  return 0;
}
