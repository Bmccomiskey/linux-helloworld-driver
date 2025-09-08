#include <linux/init.h>    //Contains the __init and __exit macros
#include <linux/module.h>  //Contains headers for loading modules into the kerel
#include <linux/kernel.h>  //Contains Kernel services, like printk


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Braylon McComiskey");
MODULE_DESCRIPTION("A Hello World kernel module");

//This function is called when the module is loaded. The __init macro tells the kernel this is an init function.
//This allows the kernel to free memory after it runs.
static int __init hello(void) {
	printk(KERN_INFO "Hello World!");
	return 0;
}

//This function is called when the module is unloaded. The __exit macro tells the kernel this is an exit function
static int __exit goodbye(void) {
	printk(KERN_INFO "Goodbye World!");
	return 0;
}

//These macros register our functions as the entry and exit positions
module_init(hello);
module_exit(goodbye);
