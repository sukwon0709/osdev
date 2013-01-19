#ifndef MONITOR_H_
#define MONITOR_H_

#include "common.h"

// Write a single character out to the screen.
void monitor_put(char c);

// Clear screen to all black.
void monitor_clear();

// Output null-terminated ASCII string.
void monitor_write(char* c);

void monitor_write_dec(u32int n);

void advance_cursor();

#endif // MONITOR_H_
