#include "monitor.h"
#include "descriptor_tables.h"

int main(struct multiboot *mboot_ptr) {
	// All out initialisation calls will go in here.
	// Initialise all the ISRs and segmentation
	init_descriptor_tables();   
	// Initialise the screen (by clearing it)
	monitor_clear();
	// Write out a sample string
	monitor_write("ChetOS - A simple x86 Operating System\n");
	asm volatile("int $0x3");
	asm volatile("int $0x4");
	return 0;
}
