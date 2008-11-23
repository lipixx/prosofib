#ifndef DEVICES_H__
#define  DEVICES_H__

#include <sched.h>

void init_devices();
int sys_write_console (int fd, char *buffer, int size);
int sys_read_keyboard (int fd, char *buffer, int size);
int sys_read_file(int fd, char* buffer, int size);
int sys_write_file(int fd, char* buffer, int size);
int sys_release_file (int dir_entry);
void actualitzar_pendents(int size);
void avanzar();
void bloquejar_teclat(struct task_struct *task);
void desbloquejar_teclat(struct task_struct *task );
void anar_al_circ(struct task_struct *bloq, int size);
#endif /* DEVICES_H__ */
