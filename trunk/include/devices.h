#ifndef DEVICES_H__
#define  DEVICES_H__

int sys_write_console (int fd, char *buffer, int size);
int sys_read_keyboard (int fd, char *buffer, int size);
int sys_open_file(const char *path, int mode_acces);
int sys_close_file(int fd);
int sys_read_file(int fd, char* buffer, int size);
int sys_write_file(int fd, char* buffer, int size);
int sys_unlink_file(const char *path);
int sys_close_console(int fd);
int sys_close_keyboard(int fd);

#endif /* DEVICES_H__ */
