#this file is a shell script used to automating the device removal process

sudo rm dev/clock_device #remove the device

sudo rmmod my_driver || true #remove the module

make clean #remove compiled files


