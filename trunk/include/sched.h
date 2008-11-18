/*
 * sched.h - Estructures i macros pel tractament de processos
 */

#ifndef __SCHED_H__
#define __SCHED_H__

#define NR_TASKS      10
#define KERNEL_STACK_SIZE	1024
#define SEM_VALUE_MAX 25
#define NUM_CANALS 10

#include <list.h>
#include <mm_address.h>
#include <sf.h>

struct task_struct
{
  int pid;
  int quantum;
  int tics_cpu;
  int pagines_fisiques[NUM_PAG_DATA];
  struct list_head run_list;
  struct fitxers_oberts* taula_canals[NUM_CANALS];
};

struct list_head runqueue;
int pid;
char call_from_int;

union task_union
{
  struct task_struct task;
  unsigned long stack[KERNEL_STACK_SIZE];	/* pila de sistema, per proces */
};

struct semafor
{
  char init;
  int count;
  struct list_head queue;
};

struct semafor sem[SEM_VALUE_MAX];

extern union task_union task[NR_TASKS];

/* Inicialitza les dades del proces inicial */
void init_task0 ();
void init_sched ();
union task_union * get_task_union(struct task_struct * n_task);
int search_free_task ();
void task_switch (union task_union *t);
struct task_struct *current ();
struct task_struct *list_head_to_task_struct (struct list_head *l);

#endif /* __SCHED_H__ */
