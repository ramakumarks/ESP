Files present in the folder:
1) main.c
2) squeue.c
4) Makefile
5) Report.pdf

main.c is a program to test the driver that has been implemented. This file initiates 6 sender threads (2 aperiodic threads that get triggered on left and right clock respectively and 4 periodic threads) and 1 receiver thread.

Message to be sent from user space to kernel space has to be in the form of structure define below

typedef struct msg_details
{
	int sedner_id;
	int message_id;
	int enqueue_time;
	int accum_time;
	double value;
}msg;


squeue.c is the file which implements the driver. It implements two Queue devices.
dataqueue1 for one aperiodic and two periodic threads.
datatqueue2 for the reamining aperiodic thread and periodic threads 

Execution
1) In the terminal, navigate to the path where source files have been placed.
2) Run the command "make all", this generates the .o file for the driver.
3) Now run the command ./main.o to execute the program.
