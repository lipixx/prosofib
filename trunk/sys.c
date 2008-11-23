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
#include <io.h>
#include <sf.h>

int
sys_ni_syscall()
{
  return -ENOSYS;
}

int
sys_write (int fd, char *buffer, int size)
{
  struct task_struct * current_task;
  struct fitxers_oberts * fitxer_obert;
  char buff_aux[256];
  int return_write, ncWritten, writeSize;

  current_task = current();
  fitxer_obert = (current_task->taula_canals[fd]);
  
  if (fd < 0 || fd >= NUM_CANALS)
    return -ECHRNG;
  if (current_task->taula_canals[fd] == NULL)
    return -EBADF;

  if (fitxer_obert->mode_acces == O_RDONLY)
    return -EROFS;

  if (size < 0)
    return -EINVAL;

  if (size > ((NUM_PAG_DATA * PAGE_SIZE) - KERNEL_STACK_SIZE))
    return -EFBIG;

  
  ncWritten = 0;
  writeSize = 0;
  while (size > 0)
    {
      if (size >= 256)
	writeSize = 256;
      else
	writeSize = size;

      if (copy_from_user (buffer, buff_aux, writeSize)!=0) return -EFAULT;

      return_write = (*((fitxer_obert->opened_file)->operations->sys_write_dev))(fd,buff_aux,writeSize);
      if (return_write < 0) return return_write;
      
      ncWritten += return_write;

      if (return_write != writeSize) return ncWritten;

      buffer += writeSize;
      size -= writeSize;
    }

  return ncWritten;
}

int sys_read(int fd, char * buffer, int size)
{
  struct task_struct * current_task;
  struct fitxers_oberts * fitxer_obert;
  char buff_aux[256];
  int ncRead, return_read, k, i;

   current_task = current();

 if (fd < 0 || fd >= NUM_CANALS)
    return -ECHRNG;
  if (current_task->taula_canals[fd] == NULL)
    return -EBADF;

  if (size < 0)
    return -EINVAL;

  if (size > (NUM_PAG_DATA * PAGE_SIZE) - KERNEL_STACK_SIZE)
    return -EFBIG;

  fitxer_obert = (current_task->taula_canals[fd]);

  if (fitxer_obert->mode_acces == O_WRONLY)
    return -EPERM;

  ncRead = 0;
  current_task->buffer=buffer;
  
  while (size != 0)
    {
      if (size < 256)
	{
	  return_read = (*(fitxer_obert->opened_file->operations->sys_read_dev))(fd,buff_aux,size);
	  size = 0;
	}
      else
	{
	  return_read = (*(fitxer_obert->opened_file->operations->sys_read_dev))(fd,buff_aux,256);
	  size -= 256;
	}
      	  
      //Farem que si hi ha error de i/o acabi la transaccio
      if (return_read == -1) return -EIO;
      
      for (k = ncRead, i=0; k < ncRead+return_read; k++, i++)
	buffer[k] = buff_aux[i];

      ncRead += return_read;
    }
  
  //Sempre hem de retornar ncRead, encara que s'hagin llegit
  //0 bytes, ja que pot ser que haguem arribat a l'EOF
  return ncRead; 

}

/*
 * Note that the flag value (low two bits) for sys_open means:
 *	00 - read-only
 *	01 - write-only
 *	10 - read-write
 */
int sys_open(const char *path, int flags)
{
  int fd,file_entry,tfo_entry;
  struct task_struct * proces = current();
  struct file * file;

   if (strlen(path) > MAX_NAME_LENGTH)
    return -ENAMETOOLONG;

  //Comprovem que podem obrir mes fitxers
  for (tfo_entry = 0; tfo_entry < MAX_OPEN_FILES &&
	 taula_fitxers_oberts[tfo_entry].refs > 0; tfo_entry++);
  if (tfo_entry == MAX_OPEN_FILES)
    return -ENFILE;

  //Comprovem que ens queden canals
  for (fd = 0; fd < NUM_CANALS && proces->taula_canals[fd] != NULL; fd++);
    if (fd == NUM_CANALS) return -EMFILE;
  
  //Obtenim el fitxer del directori
    for (file_entry = 0; file_entry < MAX_FILES && (strcmp(directori[file_entry].nom,path)); file_entry++);
  if (file_entry == MAX_FILES)
    if (flags < O_CREAT) return -ENOENT;
    else
      {
	//Crear nou file:
	file = create_file(path);
	if (file < 0) return (int) file;
	flags -= O_CREAT;
      }
  else
    file = &directori[file_entry];

  //Nomes open depenent al qui ho tingui implementat
  if (file->operations->sys_open_dev != NULL)
    (*(file->operations->sys_open_dev))(path,flags);

  //Obrim el dispositiu/fitxer!

  //Comprovem que tenim permis d'acces
  if (file->mode_acces_valid != O_RDWR && flags != file->mode_acces_valid)
    return -EPERM;
  
  //Nova entrada a la TFO i punter al canal
  taula_fitxers_oberts[tfo_entry].refs = 1;
  taula_fitxers_oberts[tfo_entry].mode_acces = flags;
  taula_fitxers_oberts[tfo_entry].lseek = 0;
  taula_fitxers_oberts[tfo_entry].opened_file = file;
  file->n_refs++;

  proces->taula_canals[fd] = &taula_fitxers_oberts[tfo_entry];

  return fd;
}

int sys_close(int fd)
{
 struct task_struct * current_task;
 
 current_task = current();

  if (fd < 0 || fd >= NUM_CANALS)
    return -ECHRNG;
  if (current_task->taula_canals[fd] == NULL)
    return -EBADF;

 //Close dependent
 if (current_task->taula_canals[fd]->opened_file->operations->sys_close_dev != NULL)
   (current_task->taula_canals[fd]->opened_file->operations->sys_close_dev)(fd);
 
 //Close generic
 current_task->taula_canals[fd]->refs--;
 current_task->taula_canals[fd]->opened_file->n_refs--;
 current_task->taula_canals[fd] = NULL;
 
 return 0;
}

int sys_dup(int fd)
{
  int new_fd;
  struct task_struct * proces = current();

  if (fd < 0 || fd >= NUM_CANALS)
    return -ECHRNG;
  if (proces->taula_canals[fd] == NULL)
    return -EBADF;
  
  for (new_fd = 0; new_fd < NUM_CANALS && proces->taula_canals[new_fd] != NULL; new_fd++);
    if (new_fd == NUM_CANALS) return -EMFILE;
 
  proces->taula_canals[new_fd] = proces->taula_canals[fd];
  proces->taula_canals[new_fd]->refs++;
  proces->taula_canals[new_fd]->opened_file->n_refs++;

 return new_fd;
}

int
sys_unlink(const char * path)
{
  struct file * fitxer;
  int file_entry;
  
  if (strlen(path) > MAX_NAME_LENGTH)
    return -ENAMETOOLONG;

  for (file_entry = 0; file_entry < MAX_FILES && (strcmp(directori[file_entry].nom,path)); file_entry++);
  if (file_entry == MAX_FILES) return -ENOENT;

  fitxer = &directori[file_entry];
  
  if (fitxer->n_refs == 0)
    {
      fitxer->n_refs = -1;
      if (fitxer->operations->sys_release_dev != NULL)
	(fitxer->operations->sys_release_dev) (file_entry);

      fitxer->operations->sys_open_dev = NULL;
      fitxer->operations->sys_close_dev = NULL;
      fitxer->operations->sys_read_dev = NULL;
      fitxer->operations->sys_write_dev = NULL;
      fitxer->operations->sys_release_dev = NULL;
      fitxer->n_refs = -1;
      fitxer->nom[0] = 0;
    }
  return -EBUSY;
  
}

int 
sys_readdir(struct dir_ent *buffer, int offset)
{
  int size = 0;
  char * s1 = directori[offset].nom;
 
  if (offset < 0 || offset >= MAX_FILES)
    return -ENOENT;
  if(directori[offset].n_refs == -1)
    return -ENOENT;
  
  for (; *s1 != 0; ++s1, ++size);
  copy_to_user(directori[offset].nom,buffer,size);

  buffer->size = directori[offset].size;
  
  return 0;
}

int
sys_nice (int quantum)
{
  int old = -EINVAL; /* Invalid argument */

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
  for (j = 0; j < NUM_PAG_DATA && current()->pagines_fisiques[j] != -1; j++)
    {
      frames[j] = alloc_frames (1);
      if (frames[j] == -1)
	{
	  for (i = j-1; i>=0; i--)
	    {
	      free_frames(frames[i],1);
	    }
	  task[fill].task.pid = -1;
	  return -EAGAIN;
	}
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

      /* Guardar la informacio sobre els nous marcs de pagines al task_struct del fill */
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
  task[fill].task.quantum = current()->quantum;	/* Tots els processos tindran el mateix quantum */
  task[fill].task.tics_cpu = 0;
  
  /* Incrementem referencies de la TFO i dels Files */
  for (i = 0; i<NUM_CANALS; i++)
    {
      if (task[fill].task.taula_canals[i] != NULL)
	{
	  task[fill].task.taula_canals[i]->refs++;
	  task[fill].task.taula_canals[i]->opened_file->n_refs++;
	}
    }

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
    (union task_union *) list_head_to_task_struct (proces_actual->
						   run_list.next);

  if (proces_actual->pid != 0)
    {
      /* Mai podem matar el proces 0 */
      /* Alliberar les estructures del proces */

      proces_actual->pid = -1;	/* Marcam la posicio del vector task com a lliure */

      /* Alliberem taula de canals */
      for (i = 0; i<NUM_CANALS; i++)
	sys_close(i);
      
      list_del (&proces_actual->run_list);	/* Eliminem el proces de la runqueue */

      for (i = 0; i < NUM_PAG_DATA; i++)	/* Alliberam les pagines fisiques */
	free_frames (proces_actual->pagines_fisiques[i], 1);

      /* Posar el seguent element de la runqueue */
      task_switch (seguent);
    }
}

int
sys_get_stats (int spid, int *tics)
{
  int i = 0;

  if (spid <0 ) return -EINVAL;    /* Invalid argument */
  if (spid > NR_TASKS) return -ESRCH;  /* No such process */		

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
  if (n_sem < 0)  return -EINVAL;		/* Error si l'identificador n_sem es invalid */
  else if (n_sem >= SEM_VALUE_MAX) return -EINVAL;

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
     s'ha de considerar un error la utilitzacio d'aquesta crida pel proces 0. */

  struct task_struct *old_task;
  union task_union *new_task;

  old_task = current ();

  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX || sem[n_sem].init == 0)
    return -EINVAL;


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
      call_from_int = 0;
      task_switch (new_task);
    }
  else
    sem[n_sem].count--;

  return 0;
}

int
sys_sem_signal (int n_sem)
{

  /* Si no hi ha cap proces bloquejat en el semafor n_sem aleshores 
     aquesta crida incrementa el comptador del semafor n_sem. En el cas 
     que hi hagi un o mes processos bloquejats sobre n_sem, aquesta crida
     desbloqueja el primer proces. */

  struct list_head *blocked_task;
  union task_union *blocked_task_union;

  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX || sem[n_sem].init == 0)
    return -EINVAL;

  if (!list_empty (&(sem[n_sem].queue)))
    {
      blocked_task = sem[n_sem].queue.next;
      list_del (blocked_task);
      
      //Hem de fer que l'eax tingui el valor del return del sem_wait
      blocked_task_union = get_task_union(list_head_to_task_struct(blocked_task));
      if ((int) blocked_task_union == NULL)
	return -ESRCH;
      blocked_task_union->stack[KERNEL_STACK_SIZE - 10] = 0;

      list_add (blocked_task, &runqueue);
    }
  else
    sem[n_sem].count++;

  return 0;
}

int
sys_sem_destroy (int n_sem)
{
  /* destrueim el semafor n_sem si aquest esta inicialitzat */
  if (n_sem < 0 || n_sem >= SEM_VALUE_MAX)
    return -EINVAL;		/* Error si l'identificador n_sem es invalid */

  if (sem[n_sem].init==0) return -EINVAL;             		/* Error si el semafor no esta inicialitzat*/
  else if (!list_empty (&sem[n_sem].queue)) return -EBUSY;	/* Error si encara hi ha processos bloquejats a la cua */
  
  sem[n_sem].init = 0;
  return 0;
}

