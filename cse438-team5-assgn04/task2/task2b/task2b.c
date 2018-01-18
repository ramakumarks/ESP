#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <linux/input.h>
#include <unistd.h>
#include <semaphore.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define timeRun 2

void *thread_signal_generator(void *data);

int signalHandlerFlag = 0;
sem_t mutex;

static inline unsigned long long rdtsc()
{
        unsigned long long d;
        __asm__ __volatile__ ("rdtsc" : "=A" (d) );
        return d;
}

void signal_handler(int signo)
{
	printf("Time : %lld : Signal Handler called\n",rdtsc());
	signalHandlerFlag = 1;
	return;
}

int main()
{
	pthread_t thread_sg;
	//int iret_sg;
	int iRetValue = 0;
	struct sigaction usr_action;
	sigset_t block_mask;
	//pid_t child_id;
    time_t endTime;
	
	/* Establish the signal handler.  */
	memset(&usr_action, 0, sizeof(usr_action));
	sigfillset(&block_mask);
	usr_action.sa_handler = signal_handler;
	usr_action.sa_mask = block_mask;
	//usr_action.sa_flags = 0;
	sigaction(SIGUSR1, &usr_action, NULL);
	
	
		
		
			/* Semaphore Mutex Initialization*/
			if(sem_init(&mutex, 0, 1) != 0)
			{
				printf("Semaphore Initialization Failed!!!\n");
			}
			sem_wait(&mutex);       /* down semaphore */
			
			pthread_create(&thread_sg, NULL, &thread_signal_generator, NULL);
		
			endTime = time(NULL) + timeRun;
			while(time(NULL) < endTime)
			{
				/* down semaphore */
				iRetValue = sem_wait(&mutex);
				if(iRetValue < 0)
				{
					printf("Time : %lld : Semaphore Error\n",rdtsc());
					perror("Semaphore error : ");
				}
				/* up semaphore */
				sem_post(&mutex);
			}
		
			/* destroy semaphore */
			sem_destroy(&mutex);
			pthread_join(thread_sg, NULL);
		
		
	return 0;
}

void *thread_signal_generator(void *data)
{
	
	//Generate a call to Signal Handler
	printf("Time : %lld : Signal Generated\n",rdtsc());
	//iRetValue = raise(SIGUSR1);
	kill(0,SIGUSR1);
	pthread_exit(0);
}

