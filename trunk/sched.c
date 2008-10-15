/*
 * sched.c - initializes struct for task 0
 */

#include <sched.h>
#include <list.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));


void init_sched()
{
  INIT_LIST_HEAD(&runqueue);
  pid = 1;
}

void init_task0(int first_ph)
{
  struct task_struct task0;
  task0.pid = pid;
  pid++;
  task0.quantum = 0;
  task0.tics_cpu = 0;
  task0.pagines_fisiques[0] = first_ph;
  list_add(&(task0.p_rqueue),&runqueue);
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
  return list_entry(l,struct task_struct,p_rqueue);
}

