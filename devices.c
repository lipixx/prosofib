#include <io.h>
#include <devices.h>
#include <sched.h>
#include <errno.h>


int
sys_write_console (int fd, char *buffer, int size)
{
  int i;
  struct task_struct * proces_actual = current();

  /* Comprovam que tenim els permisos necessaris: O_WRONLY */
  if( proces_actual->taula_canals[fd]->mode_acces!=O_WRONLY)
    return -EPERM;

  /* Incrementam la posicio de lseek a la posicio del buffer */
  buffer+=proces_actual->taula_canals[fd]->lseek;

  for (i = 0; i < size; i++)
    printc (buffer[i]);
  return i;
}

int
sys_read_keyboard(int fd, char * buffer, int size)
{
  return 0;
}

int sys_open_file(const char *path, int mode_acces){
  /*
  if(path<0) return -EINVAL;

  int i;
  for(i=0; i<MAX_FILES && path!=directori[i]; i++);
  */
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
