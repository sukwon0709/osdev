#include "timer.h"
#include "isr.h"
#include "monitor.h"

static u32int tick = 0;
static int TIMER_COMMAND = 0x43;
static int TIMER_DATA1 = 0x40;
static int TIMER_DATA2 = 0x41;
static int TIMER_DATA3 = 0x42;

static void timer_callback(registers_t regs)
{
	tick++;
	monitor_write("Tick: ");
	monitor_write_dec(tick);
	monitor_put('\n');
}

void init_timer(u32int frequency)
{
	register_interrupt_handler(IRQ0, &timer_callback);

	u32int divisor = 1193180 / frequency;

	outb(TIMER_COMMAND, 0x36);

	// has to be sent by byte
	u8int l = (u8int) (divisor & 0xFF);
	u8int h = (u8int) ((divisor >> 8) & 0xFF);

	outb(TIMER_DATA1, l);
	outb(TIMER_DATA1, h);
}
