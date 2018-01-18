#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>
#include <signal.h>
#include <string.h>
#include <setjmp.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#define PATH_TO_MICE_FILE "/dev/input/event4"	//This is the path for the mouse event

void signal_handler(int signo);

jmp_buf jump_buffer;

void *mouse_click(void *ptr)	//Function to the thread for detecting the right click
{
	int MouseEventFd;
	struct input_event MouseEvent;
	struct timeval first_click,second_click;
	unsigned char Click1 = 0;
	long seconds,useconds,mtime;
	if((MouseEventFd = open(PATH_TO_MICE_FILE, O_RDONLY)) < 0)
	{
		perror("Mouse Open Failed\n");
		return NULL;
	}
	while(read(MouseEventFd, &MouseEvent, sizeof(struct input_event)))
	{
		if((MouseEvent.type == EV_KEY) && (MouseEvent.value == 0) && (MouseEvent.code == BTN_RIGHT))
		{
			if (0 == Click1)
			{
				gettimeofday(&first_click,NULL);	//First right click detected
				Click1 = 1;
				printf("Single right click detected\n");
			}
			else
			{
				gettimeofday(&second_click,NULL);	//Secind right click
				seconds  = second_click.tv_sec  - first_click.tv_sec;
				useconds = second_click.tv_usec - first_click.tv_usec;
			    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
			    if (mtime < 500)	//Checking the time betweent he two clicks for the detection of double click
			    {
					printf("Double right click detected\n");
					raise(SIGUSR1);	//Signal raised which causes longjump to occur
				}
				else	//case where the time interval between two clicks is large so that it get's detected as a new click
				{
					first_click = second_click;
					printf("Single right click detected\n");
				}
			}
		}
	}
	printf("\nExiting the MouseHandlerThread");
return 0;
}

void *computation_function(void *data)	//Function for the thread for the imprecise computation
{
	struct sigaction double_click;
	int i=0;
	/* Establish the signal handler.  */
	memset(&double_click, 0, sizeof(double_click));
	double_click.sa_handler = signal_handler;	//Assigning the singal handler function
	sigaction(SIGUSR1, &double_click, NULL);
	if(setjmp(jump_buffer) != 0)
	{
		printf("Existing Result = %d\n",i);
	}
	else
	{
		while(1)
		{
			i++;
			usleep(10000);
		}
	}
	pthread_exit(0);
}


int main()
{
	pthread_t thread_mouse, thread_computation;
	
	printf("This program is based on the detection of a right click from the mouse, double click terminates the program\n");
	pthread_create(&thread_mouse, NULL, mouse_click, NULL);
	pthread_create(&thread_computation, NULL,computation_function, NULL);

	
	pthread_join(thread_mouse, NULL);
	pthread_join(thread_computation, NULL);
	
	return 0;
}

void signal_handler(int signo)	//Function for the signal handler.
{
	printf("Signal Handler called\n");
	longjmp(jump_buffer, -1);
}

