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

void *higher_priority_thread(void *data);
void *lower_priority_thread(void *data);
int signalHandlerFlag = 0;
sem_t mutex;
pthread_t global_tid;

int iRetValue = 0;
	

sigset_t block_mask;
	//pid_t child_id;
struct sched_param current_threadparams, new_threadparams;
int current_policy;
static inline unsigned long long rdtsc()
{
        unsigned long long d;
        __asm__ __volatile__ ("rdtsc" : "=A" (d) );
        return d;
}

void signal_handler(int signo)
{	int i;
	signalHandlerFlag = 1;
	for(i=0;i<15;i++){
	printf("Time : %lld : Signal Handler called\n",rdtsc());}
	
	return;
}

int main()
{
	pthread_t thread_h,thread_l;
	//int iret_sg;
			new_threadparams.sched_priority = sched_get_priority_max(SCHED_RR);
			
			pthread_create(&thread_h, NULL, &higher_priority_thread, NULL);
			pthread_create(&thread_l, NULL, &lower_priority_thread, NULL);		
			pthread_join(thread_h, NULL);
			pthread_join(thread_l, NULL);
		
	
	return 0;
}

void *lower_priority_thread(void *data)
{	int i;
	/* Establish the signal handler.  */
	struct sigaction usr_action;
	memset(&usr_action, 0, sizeof(usr_action));
	usr_action.sa_handler = signal_handler;
	//usr_action.sa_flags = 0;
	sigaction(SIGUSR1, &usr_action, NULL);
	
		
		
			
			
	while(1){		
	if(signalHandlerFlag == 1)
			{    printf("lower prioprity thread continues to excecute after higher priority thread ends\n");
				 for(i=0;i<5;i++){	
					printf("Time : %lld : Signal Interrupt Detected inside lower priority thread\n",rdtsc());}

					break;
				
			}}
   
	//Generate a call to Signal Handler
	
	pthread_exit(0);
}
void *higher_priority_thread(void *data)
{
	int i=0;
	//Generate a call to Signal Handler
			iRetValue = pthread_setschedparam(pthread_self(), SCHED_RR, &new_threadparams);
			if(iRetValue != 0)
			{
				printf("Realtime Priority Set Failed\n");
				return 0;
			}
			else
			{
				printf("higher Priority Set Successful = %d\n",new_threadparams.sched_priority);
			}
	printf("Time : %lld : Signal Generated\n",rdtsc());
	for(i=0;i<100;i++){
		if(i==50){
			raise(SIGUSR1);
		}
		printf("inside higher priority thread\n");	
		
	//kill(0,SIGUSR1);
		}
		//raise(SIGUSR1);
	pthread_exit(0);
}


