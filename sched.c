/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <list.h>

/* prova inicialitzacio de pagines */
#include <utils.h>
#include <mm.h>
#include <errno.h>
#include <segment.h>

union task_union task[NR_TASKS] __attribute__ ((__section__ (".data.task")));

void
init_sched ()
{
  int i;
  INIT_LIST_HEAD (&runqueue);
  pid = 1;
  for(i = 0; i < NR_TASKS; i++)
    task[i].task.pid = -1;
}

void
init_task0 (int first_ph)
{
  int i = 0;
  task[0].task.pid = pid++;
  task[0].task.quantum = 0;
  task[0].task.tics_cpu = 0;
  task[0].task.pagines_fisiques[0] = first_ph;

  /*for (i=0;i<NUM_PAG_DATA;i++)
     task[0].task.pagines_fisiques[i] = -1; */
  for (i = 0; i < NUM_PAG_DATA; i++)
    {
      /* Mirem que hi hagi prou frames lliures */
      task[0].task.pagines_fisiques[i] = alloc_frames (1);
      //if (task[0].task.pagines_fisiques[i]==-1) return - EAGAIN;
    }

  list_add (&(task[0].task.run_list), &runqueue);
}

struct task_struct *
current ()
{
  struct task_struct *p;

  __asm__ __volatile__ ("movl $0xfffff000, %%ecx\n"
			"andl %%esp, %%ecx\n"
			"movl %%ecx, %0\n":"=g" (p)::"%ecx");
  return p;
}

struct task_struct *
list_head_to_task_struct (struct list_head *l)
{
  return list_entry (l, struct task_struct, run_list);
}

int
search_free_task ()
{
  int i;
  for (i = 0; i < NR_TASKS; i++)
    {
      if (task[i].task.pid < 0)
	return i;
    }
  return -1;
}

/* TASK_SWITCH. *t es el punter al task que es passara a executar */
void
task_switch (union task_union *t)
{
  int i;
  /*
    a.  Actualitzar la TSS perque apunti a la pila de sistema de t.
    b. Actualitzar la taula de pagines perque les pagines de dades+pila
    d'usuari de t siguin accessibles.
    c.  Canviar a la pila de sistema del nou proces
    d.  Restaurar els registres.
    e.  Cal fer EOI? --> Si venim de una int si!!
    f.  IRET
  */
  
  /* Agafem la @ de la pila de la union t i actualitzem TSS
     Es un stack[KERNEL_STACK_SIZE] i no un stack, perque la stack
     esta definida com a vector.
  */
  tss.esp0 = (DWord) &(t->stack[KERNEL_STACK_SIZE]);

  /*Si permetessim agafar un num variable de pagines fisiques,
    hauriem de modificar aquesta condicio i invalidar les sobrants de la
    taula, residuals d'altres processos
  */
  for (i=0; i<NUM_PAG_DATA && t->task.pagines_fisiques[i]!=-1;i++)
    set_ss_pag(PAG_LOG_INIT_DATA_P0+i, t->task.pagines_fisiques[i]);
  
  set_cr3();

  /*Hem de moure el primer valor de la pila de t, dins %esp.
   */
  __asm__ __volatile__
    (
     "movl %0,%%esp\n"
     :
     : "g" ((DWord) &(t->stack[KERNEL_STACK_SIZE-16]))
     );
  

  
  //FALTA FER QUE ES COMPROVI SI S'HA ENTRAT AQUI PER UNA INT O PER UN KILL
  /* Haurem de fer un EOI nomes si venim de una interrupcio,
     ja que sino no podrem rebre futures interrupcions.*/

  __asm__ __volatile__(
		       "movb $0x20,%al\n"
		       "outb %al, $0x20\n"
		       );

 /*Restaurar els registres per sortir del sistema i tornar
  al fork*/
  __asm__ __volatile__(
		      
		       "popl %ebx\n"
		       "popl %ecx\n"
		       "popl %edx\n"
		       "popl %esi\n"
		       "popl %edi\n"
		       "popl %ebp\n"
		       "popl %eax\n"
		       "popl %ds\n"
		       "popl %es\n"
		       "popl %fs\n"
		       "popl %gs\n"
		       "iret\n"
		       );
}
