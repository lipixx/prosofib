/*
 * mm.h - Capçalera del mòdul de gestió de memòria
 */

#ifndef __MM_H__
#define __MM_H__

#include <types.h>
#include <mm_address.h>


#define FREE_FRAME 0
#define USED_FRAME 1
#define PH_ADDR_TO_FRAME(address) return address >> 12

#define MAX_BLOCKS 100
#define BLOCK_SIZE 256

/* Bytemap to mark the free physical pages */
Byte phys_mem[TOTAL_PAGES];

/* Sistema de fitxers */
int fat[MAX_BLOCKS];
char disk[MAX_BLOCKS][BLOCK_SIZE];

int init_frames (void);
int alloc_frames (int nframes);
void free_frames (unsigned int frame, int nframes);
int initialize_P0_frames (void);


extern Descriptor *gdt;

extern TSS tss;

int init_mm ();
void set_ss_pag (unsigned page, unsigned frame);
void set_cr3 ();

void setGdt ();

void setTSS ();
void del_ss_pag (unsigned pagina_logica);

#endif /* __MM_H__ */
