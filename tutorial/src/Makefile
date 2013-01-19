SOURCES=boot.o main.o common.o monitor.o isr.o descriptor_tables.o interrupt.o gdt_flush.o idt_flush.o timer.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link

clean:
	rm -f *.o kernel

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<
