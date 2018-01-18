
#include <stdlib.h>

pthread_mutex_t mutex;
static unsigned int register_counter = 0;

void (*function[10])();
pthread_t pt_global[10];

#define MICE_DEVICE "/dev/input/mice"


void register_my_thread(pthread_t pt, void *fn)
{
	//int i=0;
	pthread_mutex_lock(&mutex);
	if(register_counter < 10)
	{
		function[register_counter] = fn;
		//printf("register_my_thread : %lu\n",(long unsigned int)pt);
		pt_global[register_counter] = pt;
		register_counter++;
	}
	pthread_mutex_unlock(&mutex);
}

void signal_handler(int signo)
{
	int i;
	for(i=0;i<register_counter;i++)
	{
		function[i]();
		//pthread_kill(pt_global[register_counter],SIGUSR1);
	}
}


void *thread_capture_function(void *data)
{
	//int i = 0;
	int fd_mice;
	struct sigaction usr_action;
	long flag;
	int iRetValue = 0;
	
	fd_mice = open(MICE_DEVICE, O_RDONLY);
	if(fd_mice < 0)
	{
		printf("Failed to Open Mice Device\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Mouse Device Opened Successfully!!!\n");
	}
	
	iRetValue = pthread_mutex_init(&mutex, NULL);
	if(iRetValue != 0)
	{
		printf("Mutex Initialization Failed\n");
	}
	else
	{
		//printf("Mutex Initialized Successfully\n");
	}
	
	//specify a process as the owner of the file
	fcntl(fd_mice, F_SETOWN, getpid());
	flag = fcntl(fd_mice, F_GETFL);
	fcntl(fd_mice, F_SETFL, flag | FASYNC);
	
	memset(&usr_action, 0, sizeof(usr_action));
	usr_action.sa_handler = signal_handler;
	usr_action.sa_flags = SA_SIGINFO;
	sigaction(SIGIO, &usr_action, NULL);

	while(1)
	{
		sleep(5);
	}
	pthread_mutex_destroy(&mutex);
	pthread_exit(0);
}
