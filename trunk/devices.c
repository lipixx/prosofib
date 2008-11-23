#include <io.h>
#include <devices.h>
#include <sched.h>
#include <errno.h>
#include <sf.h>
#include <keyboard.h>
#include <utils.h>
#include <list.h>
#include <segment.h>
#include <mm.h>
#include <mm_address.h>

#define EAX KERNEL_STACK_SIZE - 10


void init_devices()
{
  circ_chars=0;
  pos=0;
  inici=0;
}

int
sys_write_console (int fd, char *buffer, int size)
{
  int i;
  for (i = 0; i < size; i++)
    printc (buffer[i]);
  return i;
}

int
sys_write_file (int fd, char * buffer, int size)
{
  int bloc0,destBloc,destByte,i;

  struct task_struct * proces = current();
  struct fitxers_oberts * opened_file = proces->taula_canals[fd];
  struct file * fitxer = proces->taula_canals[fd]->opened_file;

  destByte = opened_file->lseek % BLOCK_SIZE;
  bloc0 = fitxer->first_block;

  //Comensarem a escriure a l'ultim bloc del fitxer
  destBloc = last_block(fitxer->first_block);

  for (i=0; i<size; i++)
    {
      //Si es necessari, incrementem nblocks
      if (fitxer->size % BLOCK_SIZE == 0 && fitxer->size != 0)
	{
	  bloc0 = balloc(1);
	  if (bloc0 == -1) return i;
	  //Afegim bloc al fitxer, de l'actual destBloc a bloc0
	  add_block(destBloc,bloc0);
	  destBloc = bloc0;
	  destByte = 0;
	  fitxer->n_blocs++;
	}
      //Copiem dades
      disk[destBloc][destByte] = buffer[i];
      fitxer->size++;
      opened_file->lseek++;
      destByte++;
    }

  return i;
}

int
sys_read_keyboard(int fd, char * buffer, int size)
{
  union task_union *proces_seguent;
  int escriure, pid;
  struct task_struct *actual;
  actual=current();
  actualitzar_pendents(size);
  pid = actual->pid;
  
  if((circ_chars==size || actual->chars_pendents<= 0 )&& list_empty(&keyboard_queue))
    {
      /* Hi ha suficients caracters al buffer i no hi ha cap proces esperant dades */    
      
    if(circ_chars==size) escriure = size; /* Si esta ple ho escrivim tot */
    else escriure = actual->chars_pendents; /* Si ja tenim tots els caracters que voliem els escrivim */
    /* Copiam les dades del buffer circular al buffer on hem de guardar els caracters llegits */
    anar_al_circ(actual,escriure);    
    //    copy_to_user(&buffer_circular[inici],buffer,escriure);    
    avanzar();
    circ_chars=0;   /* Actualtizam el nombre de caracters valids que conte el buffer circular */
    
    return escriure;    /* Retornam el nombre de caracters que hem llegit del teclat */
    
    }
  else 
    if(pid != 0)
      { /* El proces 0 no es pot bloquejar */ 
	
	inici=pos; /* Actualitzem la posicio inicial, indicant on es troben els caracters valids */
	
	/* Bloquegem el proces i apliquem la politica de planificacio per
	   passar a executar el proces que toqui: */
	
	/* Inserir el nou proces a la llista de bloquejats: keyboard_queue: */
	bloquejar_teclat((struct task_struct *) actual);
	
	
	/* Actualitzam les dades necessaries */
	//list_last(keyboard_queue.queue)->chars_pendets=size-circ_chars;
	list_head_to_task_struct(list_first(&keyboard_queue))->size=size;
	//	list_head_to_task_struct(list_first(&keyboard_queue))->buffer=buffer;
	
	/* Actualitzem el retorn de la crida del read */ 
	((union task_union *)(list_first(&keyboard_queue)))->stack[EAX] = size;
	
	
	/* Passam a executar el seguent proces de la runqueue mitjançant la politica de planificacio */
	proces_seguent =(union task_union *) (list_head_to_task_struct (runqueue.next));
	call_from_int = 1;
	task_switch (proces_seguent);
	
	
	return size;    
	
      }
  
  return -EPERM; /*Error si el proces 0 es vol bloquejar */
}

void actualitzar_pendents(int size){
  struct task_struct * task = current();
  task->chars_pendents = size - circ_chars;
}


void avanzar(){
  /* Funcio per simular el comportament d'una cua FIFO */

  if (pos==CIRCULAR_SIZE-1) pos=0;
  else pos++;
}

void bloquejar_teclat(struct task_struct *task){
  
/* Inserir el nou proces a la llista de bloquejats: keyboard_queue: */
    list_del (&(task->run_list));   /* Eliminam el proces de la runqueue */
    list_add_tail (&(task->run_list), &keyboard_queue); 
/* Sera desbloquejat per la rutina d’atencio a la interrupcio de teclat.*/

}

void desbloquejar_teclat(struct task_struct *task ){
  
  list_del (&(task->run_list));   /* Eliminam el proces de la cua de bloquejats pel teclat */
  list_add_tail (&(task->run_list), &runqueue); /* Inserim el proces de nou a la runqueue.*/
  
}

void llegir_al_circ_es_molt_divertit(char * origen, char * desti,int size)
{
  int i;
  for(i=0;size>0;size--){
    desti[inici]=origen[(inici+i)%CIRCULAR_SIZE];
  }
}

void anar_al_circ(struct task_struct *bloq, int size){
  /* Copia les dades */

  int i;
  char buff[CIRCULAR_SIZE];

  for (i = 0; i < NUM_PAG_DATA; i++)
    { /* Associa la pagina logica del proces bloquejat amb la seva pagina fisica o 'frame' 
	 damunt les @logiques del proces actual */
      set_ss_pag (PAG_LOG_INIT_DATA_P0 + i, bloq->pagines_fisiques[i]);
    }
  set_cr3 ();
  
  llegir_al_circ_es_molt_divertit(buffer_circular,bloq->buffer,size);
  
  /* Copiam els caracters del proces actual al bloquejat */ 
  copy_to_user(buff,bloq->buffer, size);
  
  
  for(i=0; i < NUM_PAG_DATA; i++){
    
    /* Restaurem la taula de pagines del proces actual */
    set_ss_pag (PAG_LOG_INIT_DATA_P0 + i, current()->pagines_fisiques[i]);
  }
  set_cr3(); 
}

int sys_read_file(int fd, char * buffer, int size)
{
  int bloc0,byte0,i,lseekBloc;

  struct task_struct * proces = current();
  struct fitxers_oberts * opened_file = proces->taula_canals[fd];
  struct file * fitxer = proces->taula_canals[fd]->opened_file;

  bloc0 = fitxer->first_block;
  lseekBloc = opened_file->lseek / BLOCK_SIZE;
  byte0 = opened_file->lseek % BLOCK_SIZE;
  
  //Avansem n blocs
  for (i = 0; i < lseekBloc; i++)
    bloc0 = fat[bloc0];

  //Llegirem size bytes a partir del bloc0
   for (i=0; i<size; i++)
    {
      //Si es necessari, incrementem nblocks
      if (byte0 % BLOCK_SIZE == 0 && byte0 != 0 && bloc0 != fitxer->first_block)
	{
	  if (opened_file->lseek == size) return i;
	  //Seguim a partir del seguent bloc
	  if (fat[bloc0] != -1)
	      bloc0 = fat[bloc0];
	  byte0 = 0;
	}
      //Copiem dades
      buffer[i] = disk[bloc0][byte0];
      opened_file->lseek++;
      byte0++;
    }

  return i;
}

int sys_release_file(int dir_entry)
{
  freeb(directori[dir_entry].first_block);
  directori[dir_entry].n_blocs = 0;
  directori[dir_entry].size = 0;
 
  return 0;
}
