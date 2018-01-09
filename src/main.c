#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"

int main(struct multiboot *mboot_ptr) {

	// All out initialisation calls will go in here.
	// Initialise all the ISRs and segmentation
	init_descriptor_tables();   
	
	// Initialise the screen (by clearing it)
	monitor_clear();

	initialise_paging();

	// Write out a sample string
	monitor_write("ChetOS - A simple x86 Operating System\n");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	//asm volatile("sti");
	//init_timer(50);
	
	u32int *ptr = (u32int *)0xA0000000;
	u32int do_page_fault = *ptr;

	return 0;
}
