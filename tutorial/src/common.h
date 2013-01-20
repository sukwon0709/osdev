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

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void) 0 : panic_assert(__FILE__, __LINE__, #b));

extern void panic(const char* message, const char* file, u32int line);
extern void panic_assert(const char* file, u32int line, const char* desc);

void* memset(void* ptr, int ch, u32int len);
void* memcpy(void* dst, const void* src, u32int len);
int strcmp(const char* a, const char* b);
u32int strlen(const char* s);

#endif // COMMON_H_
