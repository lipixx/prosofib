/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <list.h>
#include <utils.h>
#include <mm.h>
#include <errno.h>
#include <segment.h>
#include <interrupt.h>
#include <types.h>
#include <sf.h>
#include <keyboard.h>

union task_union task[NR_TASKS] __attribute__ ((__section__ (".data.task")));

void
init_sched ()
{
  int i;
  INIT_LIST_HEAD (&runqueue);
  INIT_LIST_HEAD (&keyboard_queue);
  pid = 0;
  call_from_int = 0;
  for (i = 0; i < NR_TASKS; i++)
    task[i].task.pid = -1;

  for (i = 0; i < SEM_VALUE_MAX; i++)
    sem[i].init = 0;
}

void
init_task0 (int first_ph)
{
  int i = 0;

  task[0].task.pid = pid++;
  task[0].task.quantum = 60;
  task[0].task.tics_cpu = 0;
  vida = task[0].task.quantum;
  task[0].task.chars_pendents = 0;
  task[0].task.size = 0;
  task[0].task.buffer = 0;

  for (i = 0; i < NUM_PAG_DATA; i++)
    task[0].task.pagines_fisiques[i] = first_ph + NUM_PAG_CODE + i;

  //Init de la Taula de Canals
  for (i = 3; i < NUM_CANALS; i++)
    {
      task[0].task.taula_canals[i] = NULL;
    }

  //Init de stdin, stdout, stderr (com fer un open)
  //stdin = 0, stdout = 1, stderr = 2
  //directori[0] = keyboard, directori[1] = console
  //Fem que stderr vaigi a console.

  taula_fitxers_oberts[0].refs = 1;
  taula_fitxers_oberts[0].mode_acces = O_RDONLY;
  taula_fitxers_oberts[0].lseek = 0;
  taula_fitxers_oberts[0].opened_file = &directori[0];
  taula_fitxers_oberts[0].opened_file->n_refs = 1;

  taula_fitxers_oberts[1].refs = 1;
  taula_fitxers_oberts[1].mode_acces = O_WRONLY;
  taula_fitxers_oberts[1].lseek = 0;
  taula_fitxers_oberts[1].opened_file = &directori[1];
  taula_fitxers_oberts[1].opened_file->n_refs = 1;

  task[0].task.taula_canals[0] = &taula_fitxers_oberts[0];
  task[0].task.taula_canals[1] = &taula_fitxers_oberts[1];
  task[0].task.taula_canals[2] = &taula_fitxers_oberts[1];
  //Fi init de la Taula de Canals

  list_add (&(task[0].task.run_list), &runqueue);
}

union task_union *
get_task_union (struct task_struct *n_task)
{
  return (union task_union *) n_task;
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

  /* Actualitzar el quantum del nou proces */
  vida = t->task.quantum;

  /* Agafem la @ de la pila de la union t i actualitzem TSS
     Es un stack[KERNEL_STACK_SIZE] i no un stack, perque la stack
     esta definida com a vector.
   */
  tss.esp0 = (DWord) & (t->stack[KERNEL_STACK_SIZE]);

  /*Si permetessim agafar un num variable de pagines fisiques,
     hauriem d'invalidar les sobrants de la taula, residuals d'altres processos
   */
  for (i = 0; i < NUM_PAG_DATA && t->task.pagines_fisiques[i] != -1; i++)
    set_ss_pag (PAG_LOG_INIT_DATA_P0 + i,
		(unsigned) (t->task.pagines_fisiques[i]));

  set_cr3 ();

  /*Hem de moure la @ del primer valor de la pila de t, dins %esp. */
  __asm__ __volatile__
    ("movl %0,%%esp\n"::"g" ((DWord) & (t->stack[KERNEL_STACK_SIZE - 16])));


  /* Haurem de fer un EOI nomes si venim de una interrupcio,
     ja que sino no podrem rebre futures interrupcions. */

  if (call_from_int == 1)
    {
      __asm__ __volatile__ ("movb $0x20,%al\n" "outb %al, $0x20\n");
      call_from_int = 0;
    }

  /*Restaurar els registres per sortir del sistema i tornar
     al fork */
  __asm__ __volatile__ ("popl %ebx\n"
			"popl %ecx\n"
			"popl %edx\n"
			"popl %esi\n"
			"popl %edi\n"
			"popl %ebp\n"
			"popl %eax\n"
			"popl %ds\n"
			"popl %es\n" "popl %fs\n" "popl %gs\n" "iret\n");
}
