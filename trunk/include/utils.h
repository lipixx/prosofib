#ifndef UTILS_H
#define UTILS_H

void copy_data (void *start, void *dest, int size);
int copy_from_user (void *start, void *dest, int size);
int copy_to_user (void *start, void *dest, int size);
int strcmp(const char * s1, const char * s2);
void itoa (int n, char *buffer);

#endif
