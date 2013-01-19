#include "monitor.h"

static u16int cursor_x = 0, cursor_y = 0;
static int COMMAND_PORT = 0x3D4;
static int DATA_PORT = 0x3D5;

static u16int* video_memory = (u16int *) 0xb8000;

// Updates hardware cursor
static void move_cursor()
{
	// The screen is 80 chars wide
	u16int cursorLocation = cursor_y * 80 + cursor_x;
	outb(COMMAND_PORT, 14);		// setting high cursor byte
	outb(DATA_PORT, cursorLocation >> 8);
	outb(COMMAND_PORT, 15);		// setting low cursor byte
	outb(DATA_PORT, cursorLocation);
}

static void scroll()
{
	u8int backColor = 1;
	u8int foreColor = 15;

	// get a space char with default attribute
	u8int attributeByte = ((backColor & 0x0F) << 4) | (foreColor & 0x0F);
	u16int blank = 0x20 | (attributeByte << 8);

	// row 25 is the end
	if (cursor_y >= 25) {
		int i;
		for (i = 0; i < 24 * 80; i++) {
			video_memory[i] = video_memory[i+80];
		}

		for (i = 24*80; i < 25*80; i++) {
			video_memory[i] = blank;
		}

		cursor_y = 24;
	}
}

// writes a single char
void monitor_put(char c)
{
	u8int backColor = 1;
	u8int foreColor = 15;

	u16int attr = (backColor << 12) | (foreColor << 8);
	u16int* location;

	// handle backspace by moving a cursor back one space
	if (c == 0x08 && cursor_x) {
		cursor_x--;
	}
	// handle tab
	else if (c == 0x09) {
		cursor_x = (cursor_x + 8) & ~7;
	}
	// handle carriage return
	else if (c == '\r') {
		cursor_x = 0;
	}
	// handle newline
	else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	}
	// handle other chars
	else if (c >= ' ') {
		location  = video_memory + (cursor_y * 80 + cursor_x);
		*location  = c | attr;
		cursor_x++;
	}
	
	// insert newline if reached end of screen
	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}

	// scroll if needed
	scroll();
	// move hardware cursor
	move_cursor();
}

// clears screen
void monitor_clear()
{
	u8int backColor = 1;
	u8int foreColor = 15;

	u8int attrByte = ((backColor & 0x0F) << 4) | (foreColor & 0x0F);
	u16int blank = 0x20 | (attrByte << 8);

	int i;
	for (i = 0; i < 80 * 25; i++) {
		video_memory[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;
	move_cursor();
}

// write null terminated string
void monitor_write(char* c)
{
	int i = 0;
	while (c[i]) {
		monitor_put(c[i++]);
	}
}

void advance_cursor()
{
	cursor_x++;
	if (cursor_x >= 80) {
		cursor_y++;
		cursor_x = 0;
	}
	scroll();
}
