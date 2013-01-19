#include "monitor.h"

int main(struct multiboot* mboot_ptr)
{
	monitor_clear();
	monitor_write("Welcome to SOS!");
	return 0xDEADBABA;
}
