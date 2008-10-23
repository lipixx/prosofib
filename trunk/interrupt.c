/*
 * interrupt.c -
 */
#include <types.h>
#include <interrupt.h>
#include <segment.h>
#include <hardware.h>
#include <io.h>
#include <entry.h>
#include <sched.h>


Gate idt[IDT_ENTRIES];
Register idtR;
int temps;


char char_map[] = {
  '\0', '\0', '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '\'', '¡', '\0', '\0',
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
  'o', 'p', '`', '+', '\0', '\0', 'a', 's',
  'd', 'f', 'g', 'h', 'j', 'k', 'l', 'ñ',
  '\0', 'º', '\0', 'ç', 'z', 'x', 'c', 'v',
  'b', 'n', 'm', ',', '.', '-', '\0', '*',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',
  '8', '9', '-', '4', '5', '6', '+', '1',
  '2', '3', '0', '\0', '\0', '\0', '<', '\0',
  '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
  '\0', '\0'
};

void
setInterruptHandler (int vector, void (*handler) (), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE INTERRUPTION GATE FLAGS:                          R1: pg. 5-11  */
  /* ***************************                                         */
  /* flags = x xx 0x110 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word) (maxAccessibleFromPL << 13);
  flags |= 0x8E00;		/* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset = lowWord ((DWord) handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags = flags;
  idt[vector].highOffset = highWord ((DWord) handler);
}

void
setTrapHandler (int vector, void (*handler) (), int maxAccessibleFromPL)
{
  /***********************************************************************/
  /* THE TRAP GATE FLAGS:                                  R1: pg. 5-11  */
  /* ********************                                                */
  /* flags = x xx 0x111 000 ?????                                        */
  /*         |  |  |                                                     */
  /*         |  |   \ D = Size of gate: 1 = 32 bits; 0 = 16 bits         */
  /*         |   \ DPL = Num. higher PL from which it is accessible      */
  /*          \ P = Segment Present bit                                  */
  /***********************************************************************/
  Word flags = (Word) (maxAccessibleFromPL << 13);

  //flags |= 0x8F00;    /* P = 1, D = 1, Type = 1111 (Trap Gate) */
  /* Changed to 0x8e00 to convert it to an 'interrupt gate' and so
     the system calls will be thread-safe. */
  flags |= 0x8E00;		/* P = 1, D = 1, Type = 1110 (Interrupt Gate) */

  idt[vector].lowOffset = lowWord ((DWord) handler);
  idt[vector].segmentSelector = __KERNEL_CS;
  idt[vector].flags = flags;
  idt[vector].highOffset = highWord ((DWord) handler);
}


void
setIdt ()
{
  /* Program interrups/exception service routines */
  idtR.base = (DWord) idt;
  idtR.limit = IDT_ENTRIES * sizeof (Gate) - 1;

  /* Inicialitzar excepcions IDT */
  setInterruptHandler (0, divide_error_handler, 0);
  setInterruptHandler (1, debug_handler, 3);
  setInterruptHandler (2, nmi_handler, 0);
  setInterruptHandler (3, breakpoint_handler, 3);
  setInterruptHandler (4, overflow_handler, 0);
  setInterruptHandler (5, bounds_check_handler, 0);
  setInterruptHandler (6, invalid_opcode_handler, 0);
  setInterruptHandler (7, device_not_available_handler, 0);
  setInterruptHandler (8, double_fault_handler, 0);
  setInterruptHandler (9, coprocessor_segment_overrun_handler, 0);
  setInterruptHandler (10, invalid_tss_handler, 0);
  setInterruptHandler (11, segment_not_present_handler, 0);
  setInterruptHandler (12, stack_exception_handler, 0);
  setInterruptHandler (13, general_protection_handler, 0);
  setInterruptHandler (14, page_fault_handler, 0);
  setInterruptHandler (16, floating_point_error_handler, 0);
  setInterruptHandler (17, alignment_check_handler, 0);
  setInterruptHandler (32, clock_handler, 0);
  setInterruptHandler (33, keyboard_handler, 0);
  setTrapHandler (128, system_call_handler, 3);

/* ADD INITIALIZATION CODE FOR INTERRUPT VECTOR */
  set_idt_reg (&idtR);

}

void
iniTemps ()
{
  temps = -1;
}

/* IDT # 0 */
void
divide_error_routine ()
{
  printk ("DIVIDE ERROR EXCEPTION");
  while (1);

}

	/* IDT # 1 */
void
debug_routine ()
{
  printk ("DEBUG");
  while (1);

}

	/* IDT # 2 */
void
nmi_routine ()
{
  printk ("NON MASCABLE INTERRUPT");
  while (1);

}

	/* IDT # 3 */
void
breakpoint_routine ()
{
  printk ("BREAKPOINT");
  while (1);

}

	/* IDT # 4 */
void
overflow_routine ()
{
  printk ("OVERFLOW");
  while (1);

}

	/* IDT # 5 */
void
bounds_check_routine ()
{
  printk ("BOUNDS CHECK EXCEPTION");
  while (1);

}

	/* IDT # 6 */
void
invalid_opcode_routine ()
{
  printk ("INVALID OPCODE");
  while (1);

}

	/* IDT # 7 */
void
device_not_available_routine ()
{
  printk ("DEVICE NOT AVAILABLE");
  while (1);

}

	/* IDT # 8 amb parametre de 4 bytes */
void
double_fault_routine ()
{
  printk ("DOUBLE FAULT");
  while (1);

}

	/* IDT # 9 */
void
coprocessor_segment_overrun_routine ()
{
  printk ("COPROCESSOR SEGMENT EXCEPTION");
  while (1);

}


	/* IDT # 10 amb parametre de 4 bytes */
void
invalid_tss_routine ()
{
  printk ("INVALID TSS");
  while (1);

}

	/* IDT # 11 amb parametre de 4 bytes */
void
segment_not_present_routine ()
{
  printk ("SEGMENT NOT PRESENT");
  while (1);

}

	/* IDT # 12 amb parametre de 4 bytes */
void
stack_exception_routine ()
{
  printk ("STACK EXCEPTION");
  while (1);

}

	/* IDT # 13 amb parametre de 4 bytes */
void
general_protection_routine ()
{
  printk ("GENERAL PROTECTION EXCEPTION");
  while (1);

}

	/* IDT # 14 amb parametre de 4 bytes */
void
page_fault_routine ()
{
  int pid_fault=current()->pid;
  
  printk ("PAGE FAULT:\nHa fallat el proces amb PID=");
  //printk ("Ha fallat el proces amb PID=");
  printc ('0' + pid_fault);
  
  if (pid_fault!=0) sys_exit();
  else while (1);

}

	/* IDT # 16 */
void
floating_point_error_routine ()
{
  printk ("FLOATING POINT ERROR");
  while (1);

}

	/* IDT # 17 amb parametre de 4 bytes */
void
alignment_check_routine ()
{
  printk ("ALIGNMENT CHECK EXCEPTION");
  while (1);
}

void
clock_routine ()
{
  int minuts;
  int segons;
  char hora[6] = { '0', '0', ':', '0', '0', '\0' };
  struct task_struct * old_task;
  union task_union * new_task;

  /* Aqui hauria de ser un 18 tics/segon, pero els IPS del fitxer
   * de configuracio del .bochsrc que esta a 600000 fa que el rellotge
   * vaigi massa rapid. Per aixo ho hem modificat a 650 tics/segon,
   * mesura aproximada. Una maquina real va com podem veure a uns 227t/s:
   *  cat /proc/interrupts 
   *              CPU0       
   *    0:        227   IO-APIC-edge      timer
   *
   */
  
  vida--;
  old_task = current();
  old_task->tics_cpu++;
  if (vida == 0)
    {
      list_del(&(old_task->run_list));
      list_add_tail(&(old_task->run_list),&runqueue);
      new_task = (union task_union*) (list_head_to_task_struct(runqueue.next));
      vida = new_task->task.quantum;
      call_from_int = 1;
      task_switch(new_task);
    }
  


  /*
  if (pid > 3) //aprox cada mig segon
    {
      debug = current()->pid;

      switch (debug)
	{
      case 1:
      task_switch(&task[1]);
      break;
      case 2:
	task_switch(&task[2]);
	break;
      case 3: 
	task_switch(&task[0]);
	break;
	default: break;
	}
    }*/

  if (temps % 18 == 0)
    {
      segons = temps / 18;
      minuts = segons / 60;
      segons = segons % 60;

      /* Si fa mes de 100 minuts, reiniciarem el comptador de temps.
       * Hi ha altres alternatives com tenir una variable especifica
       * per el rellotge, pero d'aquesta manera evitam que si es te la
       * maquina engegada molt de temps es desbordi el contador. El mes
       * adient seria posar diverses variables, o una estructura de dades
       * per el clock del sistema que guardes els minuts, segons, hores,
       * dies, etc.en diferents llocs. Com que aquest sistema es de prova
       * ho hem trobat necessari implementar-ho.
       */
      if (minuts > 99)
	{
	  minuts = 0;
	  temps = 0;
	}

      itoa (minuts / 10, &hora[0]);
      itoa (minuts % 10, &hora[1]);
      hora[2] = ':';
      itoa (segons / 10, &hora[3]);
      itoa (segons % 10, &hora[4]);
      printk_xy (70, 10, hora);
    }
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

void
keyboard_routine ()
{
  Byte b = inb (0x60);
  char c;

  if (!(b & 0x80))		//Make
    {
      c = char_map[b & 0x7f];

      if (char_map[b] == '\0')
	printk_xy (70, 17, "Control");
      else
	{
	  printk_xy (70, 17, "       ");
	  printc_xy (70, 17, c);
	}
    }
}
