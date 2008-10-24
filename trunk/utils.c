#include <utils.h>
#include <types.h>
#include <errno.h>
#include <segment.h>

void
copy_data (void *start, void *dest, int size)
{
  DWord *p = start, *q = dest;
  while (size > 0)
    {
      *q++ = *p++;
      size -= 4;
    }
}

/* Copia de espacio de usuario a espacio de kernel, devuelve 0 si ok y -1 si error*/
int
copy_from_user (void *start, void *dest, int size)
{
  unsigned int min_usr_address, max_usr_address, bytes_to_copy, max_bytes;
  
  max_bytes = (NUM_PAG_CODE+NUM_PAG_DATA) * PAGE_SIZE; 
  min_usr_address = L_USER_START;
  max_usr_address =  L_USER_START + max_bytes;
  bytes_to_copy = (unsigned int) size;
  
  if ((unsigned int) start < min_usr_address
      || (unsigned int) start > max_usr_address
      || bytes_to_copy > max_bytes)
    return -EFAULT;

  DWord *p = start, *q = dest;
  while (size > 0)
    {
      *q++ = *p++;
      size -= 4;
    }
  return 0;
}

/* Copia de espacio de kernel a espacio de usuario, devuelve 0 si ok y -1 si error*/
int
copy_to_user (void *start, void *dest, int size)
{

  unsigned int min_usr_address, max_usr_address, bytes_to_copy, max_bytes;

  max_bytes = (NUM_PAG_CODE+NUM_PAG_DATA) * PAGE_SIZE; 
  min_usr_address = L_USER_START;
  max_usr_address =  L_USER_START + max_bytes;
  bytes_to_copy = (unsigned int) size;


  if (size < 0) return -EINVAL;

  if ((unsigned int) dest < min_usr_address
     || (unsigned int) dest > max_usr_address
     || bytes_to_copy > max_bytes)
        return -EFAULT;

  DWord *p = start, *q = dest;
  while (size > 0)
    {
      *q++ = *p++;
      size -= 4;
    }
  return 0;
}
