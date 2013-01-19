#ifndef DESCRIPTOR_TABLES_H_
#define DESCRIPTOR_TABLES_H_

#include "common.h"

// Access byte format
// P|DPL|DT|TYPE
// 1| 2 |1 | 4
// P - is segment present?
// DPL - descriptor priviledge level
// DT - descriptor type
// Type - segment type (code/data)

// Granularity byte format
// G|D|0|A|Segment len
// 1|1|1|1|	4
// G - granularity (0 = 1byte, 1 = 1kbyte)
// D - operand size(0 = 16bit, 1 = 32bit)
// A - available for system use (always 0)

struct gdt_entry_struct
{
	u16int limit_low;	// the lower 16bit of the limit
	u16int base_low;	// the lower 16bit of the base
	u8int  base_middle;	// next 8bits of the base
	u8int  access;		// determine which ring segment can be used
	u8int  granularity;
	u8int  base_high;	// last 8 bits of base
} __attribute__((packed));

typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
	u16int limit;		// size of table - 1
	u32int base;		// address of first gdt_entry_t struct
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

// Flags byte format
// P|DPL|FLAGS
// 1| 2 | 5
// P - present?
// DPL - priviledge level
// FLAGS - always 001100 (14)
struct idt_entry_struct
{
	u16int base_low;	// lower 16bit of address to jump to
	u16int sel;		// kernel segment selector
	u8int  always0;		// zero
	u8int  flags;		// flags
	u16int base_hi;		// upper 16bit of address to jump to
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
	u16int limit;
	u32int base;		// addr of first idt_entry element
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;

// GDT/IDT init
void init_descriptor_tables();

// ISR handles
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

#endif // DESCRIPTOR_TABLES_H_
