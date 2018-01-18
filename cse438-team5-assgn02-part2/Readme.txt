CSE 438:Embedded System Programming 

Assignment 2-Part 1

Team 5

Rama Kumar Kana Sundara
ASU ID:1213347614

Sharath Renjit Naik
ASU ID:1213340750

Files present in the folder:
1) main.c
This is a program to test the driver implemented. This contains a code to blink the led in a particular pattern.
2) RGBLed.c
This is the driver implemented to create the RGBLed device.
3) RGBLed.h
This contains some values and data that is common to both the test program and the driver file.
3) Makefile
4) RGBLed.sh
The bash script file which executes the blink pattern

Execution for the kernel level program:

1) In the Makefile change the "IOT_HOME" and "PATH" line to the directory where the i586-poky-linux-gcc is placed in host machine.
2) In "main.c" the value of pin numbers and duty cycle can be changed by adjusting the initialisation of the variable 'value'.Save upon changing.
3) Use make command to obtain the executable file, "main" and the kernel object RGBLed.ko.
4) Using scp command transfer the "RGBLed.ko" and "main" file to target device one at a time.
5) Use the insmod command "insmod RGBLed.ko" to insert the module into the kernel
6) Run the excecutable using "./main" 

Execution for the bashscript:

1) The value of the pin numbers and the duty cycle can be changed with varuables 'r','b','g' and 'duty_cycle'
2) Using scp command to transfer the "RGBLed.sh" file into the target device.
3) Run the bashscript file using "./RGBLed.sh"
