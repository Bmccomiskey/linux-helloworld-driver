# This file is a shell script that automates the commands needed for testing the device.


# Sets to exit if any command fails
set -e

#remove existing devices or modules
# "|| true" is used to ignore errors with files not existing
sudo rm dev/clock_device || true
sudo rmmod my_driver || true

sudo insmod my_driver.ko

MAJOR_NUM = $(cat /proc/devices | grep "clock_device" | awk  '{print $1}')

if [ -z "$MAJOR_NUM" ]; then 
	echo "Error: could not find major number"
	exit 1
fi

sudo mknod /dev/clock_device c $MAJOR_NUM 0
sudo chmod 666 /dev/clock_device

cat /dev/clock_device

sudo rm /dev/clock_device

sudo rmmod my_driver

