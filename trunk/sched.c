/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <list.h>

/* prova inicialitzacio de pagines */
#include <utils.h>
#include <mm.h>
#include <errno.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


void init_sched()
{
  INIT_LIST_HEAD(&runqueue);
  pid = 1;
}

void init_task0(int first_ph)
{
  int i = 0;
  task[0].task.pid = pid++;
  task[0].task.quantum = 0;
  task[0].task.tics_cpu = 0;
  task[0].task.pagines_fisiques[0] = first_ph;
  
  /*for (i=0;i<NUM_PAG_DATA;i++)
    task[0].task.pagines_fisiques[i] = -1;*/
    for (i=0; i< NUM_PAG_DATA; i++){
	/* Mirem que hi hagi prou frames lliures */
		task[0].task.pagines_fisiques[i]=alloc_frames(1);
		if (task[0].task.pagines_fisiques[i]==-1) return - EAGAIN;
	}
  
  list_add(&(task[0].task.run_list),&runqueue);
}

struct task_struct * current()
{
  struct task_struct *p;

  __asm__ __volatile__(
		       "movl $0xfffff000, %%ecx\n"
		       "andl %%esp, %%ecx\n"
		       "movl %%ecx, %0\n"
		       : "=g" (p)
		       :
		       : "%ecx"
		       );
  return p;
}

struct task_struct * list_head_to_task_struct(struct list_head * l)
{
  return list_entry(l,struct task_struct,run_list);
}

