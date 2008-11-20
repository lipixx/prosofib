#include <io.h>
#include <devices.h>
#include <sched.h>
#include <errno.h>


int
sys_write_console (int fd, char *buffer, int size)
{
  int i;
  //josep, pensa a sumar vufer+elesic i a cunpruvar es moda d'atses balit.
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
