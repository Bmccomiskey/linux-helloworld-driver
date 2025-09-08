#This line tells the kernel build system to build the my_driver.ko file (Kernel Object)
obj-m += my_drier.o

# The 'all' target is the default. This command is the standard way to build external modules
# -C: changes directory to the kernel build directory
# M=$(PWD): specifies the location of our external module source code
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

# The 'clean' target is for removing compiled files
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
