#ifndef PAGING_H_
#define PAGING_H_

#include "common.h"
#include "isr.h"

typedef struct page
{
	u32int present		: 1;	// page present in memory
	u32int rw		: 1;	// read-only if 0, rw else
	u32int user		: 1;	// supervisor if 0, user else
	u32int accessed		: 1;	// accessed since last refresh?
	u32int dirty		: 1;	// written since last refresh?
	u32int unused		: 7;
	u32int frame		: 20;	// frame address (shifted left 12 bits)
} page_t;

typedef struct page_table
{
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
	page_table_t* tables[1024];
	u32int tablesPhysical[1024];
	u32int physicalAddr;
} page_directory_t;

void initialize_paging();

// cause specified page directory to be loaded to CR3 register
void switch_page_directory(page_directory_t* new);

page_t* get_page(u32int address, int make, page_directory_t* dir);

void page_fault(registers_t regs);

#endif // PAGING_H_
