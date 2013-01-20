#include "monitor.h"
#include "paging.h"

// for bochs internal debugging
void debug()
{
	asm volatile ("xchg %bx, %bx");
}

int main(struct multiboot* mboot_ptr)
{
	init_descriptor_tables();

	monitor_clear();
	initialize_paging();
	monitor_write("Welcome to SOS!\n");

	u32int* ptr = (u32int *) 0xA0000000;
	u32int do_page_fault = *ptr;

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	asm volatile ("sti");	// enable interrupt

	init_timer(50);		// 50 Hz

	return 0;
}
