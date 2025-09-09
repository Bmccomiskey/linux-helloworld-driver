#include <linux/init.h>    //Contains the __init and __exit macros
#include <linux/module.h>  //Contains headers for loading modules into the kerel
#include <linux/kernel.h>  //Contains Kernel services, like printk
#include <linux/fs.h>      //Contains character device functions

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Braylon McComiskey");
MODULE_DESCRIPTION("A Hello World kernel module");

static int major_number; //declares major number so it can be globally used in init and exit functions

//This function is called when the module is loaded. The __init macro tells the kernel this is an init function.
//This allows the kernel to free memory after it runs.
static int __init hello(void) {
	printk(KERN_INFO "Hello Kernel! I am a character device driver!\n");
	


	//Creates a major number that acts like a telephone number for the device to communicate from device to driver
	//A character device is special in that it sends information in characters rather than in chunks
	//register_chrdev() parameters:
	// 1. Major Number: 0 means dynamically allocate one
	// 2. Name: The name that will appear in /proc/devices
	// 3. File Opperations: Just a null pointer for now
	major_number = register_chrdev(0, "my_device", NULL);

	//test if the register_chrdev() passed an error (negative number)
	if (major_number < 0) {

		printk(KERN_ALERT "Failed to allocate a kernel number!\n");
		return major_number;  //major number would be negative which is an error code
	}
	
	printk(KERN_INFO "Registered Correctly with major number %d.\n", major_number);

	return 0;
}

//This function is called when the module is unloaded. The __exit macro tells the kernel this is an exit function
static void __exit goodbye(void) {

	printk(KERN_INFO "Goodbye Kernel!\n");
	unregister_chrdev(major_number, "my_device");

}

//These macros register our functions as the entry and exit positions
module_init(hello);
module_exit(goodbye);
