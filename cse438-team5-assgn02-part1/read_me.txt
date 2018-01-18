CSE 438:Embedded System Programming 
Assignment 2-Part 1
Team 5
Rama Kumar Kana Sundara
ASU ID:1213347614
Sharath Renjit Naik
ASU ID:1213340750
Files in folder:
1.RGBled.c -is the main program.
2.Gpio_func.c-is the source program for gpio operations.
3.Gpio_func.h
4.Makefile


Steps for execution:

In the Makefile change the "IOT_HOME" and "PATH" line to the directory where the i586-poky-linux-gcc is placed in host machine.
In "RGBled.c" the value of pin numbers and duty cycle can be changed accordingly.Save upon changing.
Use make command to obtain the executable file, "blink".
Using scp command transfer the blink file to target device.
Use "chmod +x blink" to ensure "blink" is excecutable.
Run the excecutable using ./blink 
 
  