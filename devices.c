#include <io.h>
#include <devices.h>
#include <sched.h>
#include <errno.h>

int
sys_write_console (char *buffer, int size)
{
  int i;
  for (i = 0; i < size; i++)
    printc (buffer[i]);
  return i;
}

int sys_open_file(const char *path, int mode_acces){

  if(path<0) return -EINVAL;

  int i;
  for(i=0; i<MAX_FILES && path!=directori[i]; i++);

}

int sys_close_file(int fd){
  
  struct task_struct* actual = current();
  struct fitxers_oberts* tfo = actual->taula_canals[fd];
  
  tfo->refs--;
  actual->taula_canals[fd]=-1;
  
}

int sys_read_file(int fd, char* buffer, int size){
  
}
