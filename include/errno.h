//http://es.wikipedia.org/wiki/Errno.h

#ifndef _ASM_GENERIC_ERRNO_H
#define _ASM_GENERIC_ERRNO_H

#define EPERM            1	/* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define	ESRCH		 3	/* No such process */
#define	EIO		 5	/* I/O error */
#define	EBADF		 9	/* Bad file number */
#define	EAGAIN		11	/* Try again */
#define	EFAULT		14	/* Bad address */
#define	EBUSY		16	/* Device or resource busy */
#define ENODEV          19      /* No such device */
#define	EINVAL		22	/* Invalid argument */
#define ENFILE          23      /* Too many open files in system */
#define EMFILE          24      /* Too many open files by the process */
#define	EFBIG		27	/* File too large */
#define	ENOSYS		38	/* Function not implemented */
#define	EBADR		53	/* Invalid request descriptor */
#define	EBADFD		77	/* File descriptor in bad state */
#endif
