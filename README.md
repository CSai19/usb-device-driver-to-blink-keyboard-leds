# usb-device-driver-to-blink-keyboard-leds

The existing usb module is to be removed and our module is to be inserted into kernel .When a usb is plugged the keyboard leds 
start blinking until the usb is plugged out.The detailed procedure is given below:

1)Device driver code "stick.c" has been written.

2)Makefile is made with

      obj-m += stick.o

3)The existing usb_storage is removed by

       sudo rmmod uas usb_storage

4)The new module is inserted into the kernel by

       sudo insmod stick.ko

5)The inserted module can be removed from the kernel by

       sudo rmmod stick.ko

6)We can view the messages printed to log file by
       
      dmesg
