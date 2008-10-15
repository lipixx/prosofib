/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024
#include <list.h>
#include <mm_address.h>

struct task_struct {
  int pid;
  int quantum;
  int tics_cpu;
  int pagines_fisiques[NUM_PAG_CODE+NUM_PAG_DATA];
  struct list_head p_rqueue;
};

struct list_head runqueue;
int pid;

union task_union {
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];    /* pila de sistema, per procés */
};

extern union task_union task[NR_TASKS];

/* Inicialitza les dades del proces inicial */
void init_task0();
void init_sched();
struct task_struct* current();
struct task_struct * list_head_to_task_struct(struct list_head * l);

#endif  /* __SCHED_H__ */


