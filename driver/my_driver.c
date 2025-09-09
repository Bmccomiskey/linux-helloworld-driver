#include <linux/init.h>    //Contains the __init and __exit macros
#include <linux/module.h>  //Contains headers for loading modules into the kerel
#include <linux/kernel.h>  //Contains Kernel services, like printk
#include <linux/fs.h>      //Contains character device functions

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Braylon McComiskey");
MODULE_DESCRIPTION("A Hello World kernel module");


//********** PLACEHOLDER FUNCTIONS FOR FOPS **********

//reading device
static ssize_t device_read(struct file *flip, char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "my_driver: Reading from device.\n");
	return 0;
}

//writing to device
static ssize_t device_write(struct file *flip, const char __user *buf, size_t len, loff_t *off) {
	printk(KERN_INFO "my_driver: Writing to device.\n");
	return len;
}

//opening device
static int device_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "my_driver: Device opened.\n");
	return 0;
}

//releaseing device
static int device_release(struct inode *inode, struct file *file) {
	printk(KERN_INFO "my_driver: Device released.\n");
	return 0;
}


// ********** GLOBAL VARIABLE DECLARATIONS **********

//File operations struct that points to declared functions above with operations for my device
static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
};

static int major_number;  //declares major number globally


// ********** INIT AND EXIT FUNCTIONS **********

//This function is called when the module is loaded. The __init macro tells the kernel this is an init function.
//This allows the kernel to free memory after it runs.
static int __init hello(void) {
	printk(KERN_INFO "Hello Kernel! I am a character device driver!\n");
	


	//Creates a major number that acts like a telephone number for the device to communicate from device to driver
	//A character device is special in that it sends information in characters rather than in chunks
	//register_chrdev() parameters:
	// 1. Major Number: 0 means dynamically allocate one
	// 2. Name: The name that will appear in /proc/devices
	// 3. File Operations: Tells the kernel what to do for operations with the device
	major_number = register_chrdev(0, "my_device", &fops);

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
