#include <utils.h>
#include <types.h>
#include <errno.h>
#include <segment.h>

void copy_data(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  while(size > 0) {
    *q++ = *p++;
    size -= 4;
  }
}
/* Copia de espacio de usuario a espacio de kernel, devuelve 0 si ok y -1 si error*/
int copy_from_user(void *start, void *dest, int size)
{
  if ((char*)start < (char*)(int*)(L_USER_START+(NUM_PAG_CODE*4096))) 
    return -EFAULT;
  if ((char*)start+(size*4) >= (char*)(int*)USER_ESP)
    return -EFAULT;
  
  DWord *p = start, *q = dest;
  while(size > 0) {
    *q++ = *p++;
    size -= 4;
  }
  return 0;
}
/* Copia de espacio de kernel a espacio de usuario, devuelve 0 si ok y -1 si error*/
int copy_to_user(void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  while(size > 0) {
    *q++ = *p++;
    size -= 4;
  }
  return 0;
}

