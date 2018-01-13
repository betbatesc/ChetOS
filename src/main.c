#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "multiboot.h"
#include "fs.h"
#include "initrd.h"
#include "task.h"

#define CHECK_FLAG(flags,bit)	((flags)&(1 << (bit)))

extern u32int placement_address;
u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack) {
	
	initial_esp = initial_stack;

	// Initialise all the ISRs and segmentation
	init_descriptor_tables();   
	
	// Initialise the screen (by clearing it)
	monitor_clear();	

	// Write out a sample string
	monitor_write("ChetOS - A simple x86 Operating System\n");

	asm volatile("sti");
	init_timer(50);

	// Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    // Create a new process in a new address space which is a clone of this.
    int ret = fork();

    monitor_write("fork() returned ");
    monitor_write_hex(ret);
    monitor_write(", and getpid() returned ");
    monitor_write_hex(getpid());
    monitor_write("\n============================================================================\n");

    // The next section of code is not reentrant so make sure we aren't interrupted during.
    asm volatile("cli");
    // list the contents of /
    int i = 0;
    struct dirent *node = 0;
    while ( (node = readdir_fs(fs_root, i)) != 0)
    {
        monitor_write("Found file ");
        monitor_write(node->name);
        fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        if ((fsnode->flags&0x7) == FS_DIRECTORY)
        {
            monitor_write("\n\t(directory)\n");
        }
        else
        {
            monitor_write("\n\t contents: \"");
            char buf[256];
            u32int sz = read_fs(fsnode, 0, 256, buf);
            int j;
            for (j = 0; j < sz; j++)
                monitor_put(buf[j]);
            
            monitor_write("\"\n");
        }
        i++;
    }
    monitor_write("\n");

    asm volatile("sti");
	
	/*
	monitor_write("\nFlags: ");
	monitor_write_hex(mboot_ptr->flags);

	monitor_write("\nmem_lower: ");
	monitor_write_hex(mboot_ptr->mem_lower);

	monitor_write("\nmem_upper: ");
	monitor_write_hex(mboot_ptr->mem_upper);

	monitor_write("\nboot_device: ");
	monitor_write_hex(mboot_ptr->boot_device);

	monitor_write("\ncmdline: ");
	monitor_write_hex(mboot_ptr->cmdline);

	monitor_write("\nmods_count: ");
	monitor_write_hex(mboot_ptr->mods_count);

	monitor_write("\nmods_addr: ");
	monitor_write_hex(mboot_ptr->mods_addr);	

	monitor_write("\n");

	// Are mods_* valid?
	if (CHECK_FLAG (mboot_ptr->flags, 3))
		monitor_write("Mods valid...\n");

	// Find the location of our initial ramdisk
	ASSERT(mboot_ptr->mods_count > 0);	
	u32int initrd_location = *((u32int *)mboot_ptr->mods_addr);
	u32int initrd_end = *(u32int *)(mboot_ptr->mods_addr+4);

	// Don't trample our module with placement accesses, please!
	placement_address = initrd_end;

	// Start paging.
	initialise_paging();

	// Initialise the initial ramdisk, and set it as the filesystem root.
	fs_root = initialise_initrd(initrd_location);
	
	// list the contents of
	int i = 0;
	struct dirent *node = 0;
	while ((node = readdir_fs(fs_root, i)) != 0) {
		monitor_write("Found file ");
		monitor_write(node->name);
		fs_node_t *fsnode = finddir_fs(fs_root, node->name);
		if ((fsnode->flags&0x7) == FS_DIRECTORY) {
			monitor_write("\n\t(directory)\n");
		} else {
			monitor_write("\n\t contents: \"");
			char buf[256];
			u32int sz = read_fs(fsnode, 0, 256, buf);
			int j;
			for (j = 0; j < sz; j++)
				monitor_put(buf[j]);
			monitor_write("\"\n");
		}
		i++;
	}

	//

	//u32int a = kmalloc(8);

	//initialise_paging();
	*/

	/*
	u32int b = kmalloc(8);
	u32int c = kmalloc(8);
	
	monitor_write("a: ");
	monitor_write_hex(a);	
	monitor_write(", b: ");
	monitor_write_hex(b);
	monitor_write("\nc: ");
	monitor_write_hex(c);

	kfree(c);
	kfree(b);
	u32int d = kmalloc(12);
	monitor_write(", d: ");
	monitor_write_hex(d);
	*/

	//asm volatile("int $0x3");
	//asm volatile("int $0x4");

	//asm volatile("sti");
	//init_timer(50);
	
	//u32int *ptr = (u32int *)0xA0000000;
	//u32int do_page_fault = *ptr;

	return 0;
}
