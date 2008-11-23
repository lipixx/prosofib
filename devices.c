#include <io.h>
#include <devices.h>
#include <sched.h>
#include <errno.h>

<<<<<<< devices.c
#define EAX KERNEL_STACK_SIZE - 10


void init_devices()
{
  circ_chars=0;
  pos=0;
  inici=0;
}

=======

>>>>>>> ./devices.c.r217
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
  //escriure al fitxer
  
  /* Incrementam la posicio de lseek per cada caracter escrit*/
  return 0;
}

int
sys_read_keyboard(int fd, char * buffer, int size)
{
  return 0;
}

int sys_open_file(const char *path, int mode_acces)
{
<<<<<<< devices.c
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


int sys_open_file(const char *path, int mode_acces){
  /*
int sys_open_file(const char *path, int mode_acces)
{
=======
>>>>>>> ./devices.c.r217
  if(path<0) return -EINVAL;

  // int i;
  //  for(i=0; i<MAX_FILES && path != directori[i]; i++);

  return 0;
}

int sys_close_file(int fd){
  /*
  struct task_struct* actual = current();
  struct fitxers_oberts* tfo = actual->taula_canals[fd];
  
  tfo->refs--;
  actual->taula_canals[fd]=-1;
  */
  return 0;
}

int sys_read_file(int fd, char* buffer, int size){
  return 0;
}

int sys_close_console(int fd)
{
  return 0;
}

int sys_close_keyboard(int fd)
{
  return 0;
}
