
#include <linux/module.h>  //Contains headers for loading modules into the kerel
#include <linux/kernel.h>  //Contains Kernel services, like printk
#include <linux/fs.h>      //Contains character device functions
#include <linux/timekeeping.h> //Containts fuctions for interacting with kernel time
#include <linux/time.h>    //Contains function to convert from UNIX time to datetime 
#include <linux/jiffies.h> //COntains jiffies and HZ for incrementing timer

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Braylon McComiskey");
MODULE_DESCRIPTION("A Hello World kernel module");


// ********** GLOBAL VARIABLE DECLARATIONS **********

//struct that handles the timer operations from an existing library
static struct timer_list my_timer;

static int major_number;  //declares major number globally

// ********** HELPER FUNCTIONS  **********

//this function will be called when the timer expires
static void sound_alarm(struct timer_list *timer) {
    printk(KERN_ALERT "TIMES UP!!!!");
}


//********** FILE OPERATIONS **********

//Read function (runs when using <)
//Reads system time, formats to readable string, outputs to user
static ssize_t device_read(struct file *flip, char __user *buf, size_t len, loff_t *off) {
	
	char timestr[64];
	int timestr_len;
	struct tm datetime;

	//check to stop cat after the first read
	if (*off > 0) {
		return 0;  //end of file
	}

	//Read in system time from kernel
	struct timespec64 timestruct;
	ktime_get_real_ts64(&timestruct);
	
	//Converts UNIX time to datetime
	time64_to_tm(timestruct.tv_sec, 0, &datetime);
	
	//Subtract 5 to convert from UTC to EST
	int conv_hour = (datetime.tm_hour - 4);
	
	//conversion from army time to standard time
	if (conv_hour < 1) {
		conv_hour += 12;
	} else if (conv_hour > 12) {
		conv_hour -= 12;
	}
	
	//Copys the string onto timestr so it can be copied onto user space.
	//sprintf() also outputs the length of the string so we can increment the offset after we are done printing
	timestr_len = sprintf(timestr, "Time -  %02d:%02d:%02d\n", conv_hour, datetime.tm_min, datetime.tm_sec);

	//Copy the string to user space
	//this is important because "cat" command runs in user space and can only print out user space data
	//if it could access kernel space it would corrupt security of the system
	if (copy_to_user(buf, timestr, timestr_len)) {
		printk(KERN_ALERT, "Could not copy time to user space");
		return -EFAULT;  //bad address error
	}
	
	//increment the timestr_len to notify the device we are done reading
	*off += timestr_len;

	return timestr_len;
}

//Write function (runs when using >)
//gets time to increment from user, gets system time from kernel, sets a function to run after increment + system time
//when the user writes a "5" it will sound the alarm in 5 seconds 
static ssize_t device_write(struct file *flip, const char __user *buf, size_t len, loff_t *off) {
    
    char user_str[32];
    unsigned long delay_seconds;
    int err;

    printk(KERN_INFO "Copying increment from user space");

    //this will get the data from the user to use for imcrementation of the timer
    if (copy_from_user(user_str, buf, len)) {
        printk(KERN_ALERT "Could not copy increment from user space");
        return -EFAULT; //bad address error
    }
    
    printk(KERN_INFO "Converting user input into unsigned long");

    //converts the user input into unsigned long so it can be used in incrementation
    //kstrtoul returns 1 on error
    err = kstrtoul(user_str, 10, &delay_seconds);

    if (err) {
        printk(KERN_ALERT "Could not convert input to unsigned long");
        return err;
    }

    printk(KERN_INFO "Setting the alarm for %lu sconds from now", delay_seconds);
    
    //mod_timer increments the timer
    //jiffies + (delay_seconds * HZ) is the conversion from our seconds to an increment the kernel understands
    // - jiffies: the kernels inerntal time tick
    // - HZ: the amount of jiffies per second (kernel constant)
    mod_timer(&my_timer, (jiffies + (delay_seconds * HZ));

	return len; //returns the length of what we coppied
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

//File operations struct that points to declared functions above with operations for my device
static struct file_operations fops = {
    .owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release,
};


// ********** INIT AND EXIT FUNCTIONS **********

//This function is called when the module is loaded. The __init macro tells the kernel this is an init function.
//This allows the kernel to free memory after it runs.
static int __init hello(void) {
	printk(KERN_INFO "Hello Kernel! I am a character device driver!\n");
	


	//Creates a major number that acts like a telephone number for the device to communicate from device to driver
	//A character device is special in that it sends information in characters rather than in chunks
	//register_chrdev() parameters:
	// @ Major Number: 0 means dynamically allocate one
	// @ Name: The name that will appear in /proc/devices
	// @ File Operations: Tells the kernel what to do for operations with the device
	major_number = register_chrdev(0, "clock_device", &fops);

	//test if the register_chrdev() passed an error (negative number)
	if (major_number < 0) {

		printk(KERN_ALERT "Failed to allocate a kernel number!\n");
		return major_number;  //major number would be negative which is an error code
	}
	
	printk(KERN_INFO "Registered Correctly with major number %d.\n", major_number);
    
    //sets up the timer to be used by the user later
    timer_setup(&my_timer, sound_alarm, 0);

    printk(KERN_INFO "Registerd Timer");

	return 0;
}

//This function is called when the module is unloaded. The __exit macro tells the kernel this is an exit function
static void __exit goodbye(void) {
    
    printk(KERN_INFO "Deactivating pending timer");

    //this removes any pending timer to avoid a kernel crash
    //if module is removed before timer activates it will cause a kernel crash
    //the address for the callback function would not exist anymore
    del_timer_sync(&my_timer);

	printk(KERN_INFO "Goodbye Kernel!\n");
	unregister_chrdev(major_number, "clock_device");

}

//These macros register our functions as the entry and exit positions
module_init(hello);
module_exit(goodbye);
