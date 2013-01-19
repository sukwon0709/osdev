#ifndef ISR_H_
#define ISR_H_

#include "common.h"

#define IRQ0	32
#define IRQ15	47

typedef struct registers
{
	u32int ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int int_no, err_code;
	u32int eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(u8int n, isr_t handler);

#endif // ISR_H_
