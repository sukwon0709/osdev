#ifndef KHEAPS_H_
#define KHEAPS_H_

#include "common.h"

u32int kmalloc_a(u32int sz);
u32int kmalloc_p(u32int sz, u32int* phys);
u32int kmalloc_ap(u32int sz, u32int* phys);
u32int kmalloc(u32int sz);

#endif // KHEAPS_H_
