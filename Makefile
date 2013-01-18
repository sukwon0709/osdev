CFLAGS=-m32

all: os-image

run: all
		qemu-system-i386 -fda os-image

bochs: all
		bochs

os-image: boot_sect.bin kernel.bin
		cat $^ > os-image

kernel.bin: kernel_entry.o kernel.o
		ld -o kernel.bin -melf_i386 -Ttext 0x1000 $^ --oformat binary

kernel.o: kernel.c
		gcc ${CFLAGS} -ffreestanding -c $< -o $@

kernel_entry.o: kernel_entry.asm
		nasm $< -f elf32 -o $@

boot_sect.bin: boot_sect.asm
		nasm $< -f bin -o $@

clean:
		rm -fr *.bin *.dis *.o os-image *.map


kernel.dis: kernel.bin
		ndisasm -b 32 $< > $@
