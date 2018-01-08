#include "monitor.h"

int main(struct multiboot *mboot_ptr) {
	// All out initialisation calls will go in here.
	// return 0xDEADBABA;   
	// Initialise the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("ChetOS - A simple x86 Operating System\n");
	return 0;
}
