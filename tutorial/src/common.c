#include "common.h"

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port)
{
	u8int ret;
	asm volatile ("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int inw(u16int port)
{
	u16int ret;
	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

void* memset(void* ptr, int ch, u32int len)
{
	char* p = ptr;
	u32int i;

	for (i = 0; i < len; i++) {
		p[i] = ch;
	}

	return ptr;
}

void* memcpy(void* dst, const void* src, u32int len)
{
	u32int i;

	char* d = dst;
	const char* s = src;

	for (i = 0; i < len; i++) {
		d[i] = s[i];
	}

	return dst;
}

int strcmp(const char* a, const char* b)
{
	u32int i;

	for (i = 0; a[i] != 0 && a[i] == b[i]; i++);

	if (a[i] > b[i]) {
		return 1;
	}
	else if (a[i] == b[i]) {
		return 0;
	}
	return -1;
}

u32int strlen(const char* s)
{
	u32int ret = 0;
	while (s[ret]) {
		ret++;
	}
	return ret;
}

extern void panic(const char* message, const char* file, u32int line)
{
	asm volatile ("cli");

	monitor_write("PANIC(");
	monitor_write(message);
	monitor_write(") at ");
	monitor_write(file);
	monitor_write(":");
	monitor_write_dec(line);
	monitor_write("\n");

	for (;;);
}

extern void panic_assert(const char* file, u32int line, const char* desc)
{
	asm volatile ("cli");

	monitor_write("ASSERTION-FAILED(");
	monitor_write(desc);
	monitor_write(") at ");
	monitor_write(file);
	monitor_write(":");
	monitor_write_dec(line);
	monitor_write("\n");

	for (;;);
}
