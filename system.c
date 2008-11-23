/*
 * system.c - 
 */

#include <segment.h>
#include <types.h>
#include <interrupt.h>
#include <hardware.h>
#include <system.h>
#include <sched.h>
#include <mm.h>
#include <io.h>
#include <utils.h>
#include <libc.h>
#include <sf.h>
#include <devices.h>

int (*usr_main) (void) = (void *) PH_USER_START;
unsigned int *p_sys_size = (unsigned int *) KERNEL_START;
unsigned int *p_usr_size = (unsigned int *) KERNEL_START + 1;

/************************/
/** Auxiliar functions **/
/************************/
/**************************
 ** setSegmentRegisters ***
 **************************
 * Set properly all the registers, used
 * at initialization code.
 *   DS, ES, FS, GS <- DS
 *   SS:ESP <- DS:DATA_SEGMENT_SIZE
 *         (the stacks grows towards 0)
 *
 * cld -> gcc2 wants DF (Direction Flag (eFlags.df))
 *        always clear.
 */

/*
 * This function MUST be 'inline' because it modifies the %esp 
 */
inline void
set_seg_regs (Word data_sel, Word stack_sel, DWord esp)
{
  esp = esp - 5 * sizeof (DWord);	/* To avoid overwriting task 1 */
  __asm__ __volatile__ ("cld\n\t" "mov %0,%%ds\n\t" "mov %0,%%es\n\t" "mov %0,%%fs\n\t" "mov %0,%%gs\n\t" "mov %1,%%ss\n\t" "mov %2,%%esp":	/* no output */
			:"r" (data_sel), "r" (stack_sel), "g" (esp));

}

/*
 *   Main entry point to ZEOS Operatin System 
 */

int __attribute__ ((__section__ (".text.main"))) main (void)
{
  int first_ph;

  set_eflags ();

  /* Define the kernel segment registers */
  set_seg_regs (__KERNEL_DS, __KERNEL_DS, KERNEL_ESP);

  /* Move user code now, because the following code can overwrite the data section */
  copy_data ((void *) KERNEL_START + *p_sys_size, usr_main, *p_usr_size);

  printk ("Kernel Carregat!...\n\n");

  /* Initialize hardware data */
  setGdt ();			/* Definicio de la taula de segments de memoria */
  setIdt ();			/* Definicio del vector de interrupcions */
  setTSS ();			/* Definicio de la TSS */

  /* Initialize Memory */
  first_ph = init_mm ();
  iniTemps ();

  /* Inicialitzem les variables i estructures del scheduler */
  init_sched ();

  /* Inicialitzem els dispositius */
  init_devices ();

  /* Inicialitzar tot el sistema de fitxers */
  init_filesystem ();

  /* Initialize task 0 data */
  init_task0 (first_ph);

  /* Activem les interrupcions */
  enable_int ();

  printk ("Entrant en mode usuari...\n");

  /*
   * We return from a 'theorical' call to a 'call gate' to reduce our privileges
   * and going to execute 'magically' at 'usr_main'...
   */
  return_gate (__USER_DS, __USER_DS, USER_ESP, __USER_CS, L_USER_START);

  /* The execution never arrives to this point */
  return 0;
}
