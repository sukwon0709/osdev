#include "paging.h"
#include "monitor.h"

u32int* frames;
u32int nframes;

extern u32int placement_address;

page_directory_t* kernel_directory = 0;
page_directory_t* current_directory = 0;

#define INDEX_FROM_BIT(a) (a/32)
#define OFFSET_FROM_BIT(a) (a%32)

static void set_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (1 << off);
}

static void clear_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(1 << off);
}

static u32int test_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (1 << off));
}

static u32int first_frame()
{
	u32int i, j;
	for (i = 0; i <	INDEX_FROM_BIT(nframes); i++) {
		if (frames[i] != 0xFFFFFFFF) {
			for (j = 0; j < 32; j++) {
				u32int toTest = 1 << j;
				if (!(frames[i] & toTest)) {
					return i*32 + j;
				}
			}
		}
	}
	return -1;
}

void alloc_frame(page_t* page, int is_kernel, int is_writeable)
{
	if (page->frame != 0) return;
	else {
		u32int idx = first_frame();
		if (idx == (u32int) -1) {
			PANIC("No free frames!");
		}
		set_frame(idx * 0x1000);
		page->present = 1;
		page->rw = (is_writeable)?1:0;
		page->user = (is_kernel)?0:1;
		page->frame = idx;
	}
}

void free_frame(page_t* page)
{
	u32int frame;
	if (!(frame = page->frame)) return;
	else {
		clear_frame(frame);
		page->frame = 0;
	}
}

void initialize_paging()
{
	u32int mem_end_page = 0x1000000;	// 16mb for now

	nframes = mem_end_page / 0x1000;
	frames = (u32int *) kmalloc(INDEX_FROM_BIT(nframes), 0, 0);

	memset(frames, 0, INDEX_FROM_BIT(nframes));

	kernel_directory = (page_directory_t *) kmalloc_a(sizeof(page_directory_t));
	current_directory = kernel_directory;

	int i = 0;
	while (i < placement_address) {
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}

	register_interrupt_handler(14, page_fault);
	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t* dir)
{
	current_directory = dir;
	asm volatile ("mov %0, %%cr3" :: "r"(&dir->tablesPhysical));
	u32int cr0;
	asm volatile ("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;	// enable paging
	asm volatile ("mov %0, %%cr0" :: "r"(cr0));
}

page_t* get_page(u32int address, int make, page_directory_t* dir)
{
	address /= 0x1000;

	u32int table_idx = address / 1024;
	if (dir->tables[table_idx]) {
		return &dir->tables[table_idx]->pages[address%1024];
	} else if (make) {
		u32int tmp;
		dir->tables[table_idx] = (page_table_t *)kmalloc_ap(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; // present, rw, user
		return &dir->tables[table_idx]->pages[address%1024];
	} else {
		return 0;
	}
}

void page_fault(registers_t regs)
{
	u32int faulting_address;
	asm volatile ("mov %%cr2, %0" : "=r" (faulting_address));

	int present = !(regs.err_code & 0x1);		// page not present
	int rw = regs.err_code & 0x2;			// write operation?
	int user = regs.err_code & 0x4;			// user mode?
	int reserved = regs.err_code & 0x8;
	int id = regs.err_code & 0x10;			// from instr fetch?

	monitor_write("Page fault! (");
	if (present) { monitor_write("present "); }
	if (rw) { monitor_write("read-only "); }
	if (user) { monitor_write("user-mode "); }
	if (reserved) { monitor_write("reserved "); }
	monitor_write(") at 0x");
	monitor_write_hex(faulting_address);
	monitor_write("\n");
	PANIC("Page fault");
}
