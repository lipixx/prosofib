#ifndef DEVICES_H__
#define  DEVICES_H__

int sys_write_console (char *buffer, int size);

int sys_open_file(const char *path, int mode_acces);
int sys_close_file(int fd);
int sys_read_file(int fd, char* buffer, int size);
int sys_write_file(int fd, char* buffer, int size);
int sys_unlink_file(const char *path);

int sys_read_keyboard (char *buffer, int size);

#endif /* DEVICES_H__ */
