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
  
  int error;
  
  error = check_address(start, size);
  
  if (error != 0) return error;
  
  DWord *p = start, *q = dest;
  while (size > 0)
    {
      *q++ = *p++;
      size -= 4;
    }
  return 0;
}

int
check_address(void *buffer, int size)
{
  unsigned int min_usr_address, max_usr_address, bytes_to_copy, max_bytes;
  
  max_bytes = (NUM_PAG_CODE + NUM_PAG_DATA) * PAGE_SIZE;
  min_usr_address = L_USER_START;
  max_usr_address = L_USER_START + max_bytes;
  bytes_to_copy = (unsigned int) size;
  
  if ((unsigned int) buffer < min_usr_address
      || (unsigned int) buffer > max_usr_address || bytes_to_copy > max_bytes)
    return -EFAULT;

  return 0;
}

/* Copia de espacio de kernel a espacio de usuario, devuelve 0 si ok y -1 si error*/
int
copy_to_user (void *start, void *dest, int size)
{

  int error;

  if (size < 0)
    return -EINVAL;

  error = check_address(dest, size);
  
  if (error != 0) return error;

  DWord *p = start, *q = dest;
  while (size > 0)
    {
      *q++ = *p++;
      size -= 4;
    }
  return 0;
}

/*Strcmp implementation, thanks to:
 * P.J. Plauger, The Standard C Library, 1992
 */
int
strcmp (const char *s1, const char *s2)
{
  for (; *s1 == *s2; ++s1, ++s2)
    if (*s1 == 0)
      return 0;
  return *(unsigned char *) s1 < *(unsigned char *) s2 ? -1 : 1;
}

int
strlen (const char *s1)
{
  int i = 0;
  for (; s1[i] != 0; i++);
  return i;
}

void
itoa (int n, char *buffer)
{
  int ndigits = n;
  int i = 0;

  while (ndigits > 0)
    {
      i++;
      ndigits /= 10;
    }

  while (i > 0)
    {
      buffer[i - 1] = (n % 10) + '0';
      n /= 10;
      i--;
    }

}
