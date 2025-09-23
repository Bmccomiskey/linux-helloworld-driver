
This is a device driver that interacts with the Linux kernel to display the current time and set a timer.

# Setup

  - Clone the repository
  - Run these commands in the driver directory
    - sudo chmod +x start.sh
    - sudo chmod +x end.sh
    - sudo chmod +x tests.sh
  - Run "./start.sh" to start the device
    - This is now when you can use cat and echo to interact with the device.
    - To make sure the program is working, run "./tests.sh".
  - Run "./end.sh" to clean the directory and remove the device.
      

# How to use

  - Display Time: cat /dev/clock_device

  - Set timer: echo "{timer length in seconds}" > /dev/clock_device


# Notes

  - If you create a timer and then run "./end.sh" before it goes off, the timer will not display and will be canceled.
  - Running "dmesg | tail" will allow you to see the device's process statements (opening device, closing device, etc.).
  - The time is displayed in EST.
