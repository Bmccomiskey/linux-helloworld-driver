# This file is a shell script that automates the commands needed for starting the device.

# Sets to exit if any command fails
set -e

bear -- make #compile (bear means record .h files paths)

sudo insmod my_driver.ko #insert module

sleep 1 #rest before finding major num (if too fast can cause errors)

#get major number (associated with device named clock_device in proc/devices)
MAJOR_NUM=$(cat /proc/devices | grep "clock_device" | awk '{print $1}')

#error checking to make sure major number was found
if [ -z "$MAJOR_NUM" ]; then 
	echo "Error: could not find major number"
	
	sudo rmmod my_driver

	exit 1
fi

sudo mknod /dev/clock_device c $MAJOR_NUM 0 #creates the device from the module

sudo chmod 666 /dev/clock_device #sets device permissions 
