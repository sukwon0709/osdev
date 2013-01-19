#ifndef COMMON_H_
#define COMMON_H_

typedef unsigned int	u32int;
typedef 	 int	s32int;
typedef unsigned short	u16int;
typedef 	 short  s16int;
typedef unsigned char   u8int;
typedef          char   s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

void* memset(void* ptr, int ch, u32int len);
void* memcpy(void* dst, const void* src, u32int len);
int strcmp(const char* a, const char* b);
u32int strlen(const char* s);

#endif // COMMON_H_
