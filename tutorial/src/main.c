#include "monitor.h"

int main(struct multiboot* mboot_ptr)
{
	init_descriptor_tables();

	monitor_clear();
	monitor_write("Welcome to SOS!");

	asm volatile ("int $0x3");
	asm volatile ("int $0x4");

	return 0xDEADBABA;
}
