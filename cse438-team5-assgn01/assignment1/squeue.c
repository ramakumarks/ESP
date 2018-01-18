#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<fcntl.h>
#include<stdio.h>

#define queue_length 10

//Time Stamp Counter

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

// Message Structure 

typedef struct msg_details 
{
   int sender_id;               // Source Id 
   int message_id;              // Message Id 
   int enqueue_time;        // Enqueue time stamp
   int accum_time;        // accumulated time in queue
//   char msg_buffer[80];   // user define msg buff
   double value;
} msg;

typedef struct squeue_dev {
	msg *msg_pointer[queue_length];
	int Head_Pointer;					// Pointer Represnting Head
	int Tail_Pointer;					// Pointer Representing Tail
	int Data_Count;						// Data present in Queue
	pthread_mutex_t mulock;
} squeue_t;

struct class *squeue_dev_class;          /* Tie with the device model */

squeue_t * sq_create()
{
	squeue_t *newq = malloc(sizeof(squeue_t));	//Allocate memory for a new queue
	newq->Head_Pointer = 0;				
	newq->Tail_Pointer = 0;
	newq->Data_Count = 0;
	pthread_mutex_init(&(newq->mulock), NULL);	//Initialise mutex of new queue
	return newq;
}
int sq_write (msg *m, squeue_t *queue)
{
	pthread_mutex_lock(&(queue->mulock));		//Enables Mutex lock so other processes cannot access it
	if(queue-> Head_Pointer == queue-> Tail_Pointer && queue->Data_Count)
	{
		pthread_mutex_unlock(&(queue->mulock));
		return -1;
	}
	int index = queue->Head_Pointer;
	queue->msg_pointer[index] = m;			//Message is transferred from user
	queue->msg_pointer[index]->enqueue_time = rdtsc();//Store enqueue time
	queue->Head_Pointer++;				//Increase the pointer that points to the starting of the queue
	queue->Head_Pointer %= queue_length;		//Increasing the length of the queue
	if(queue->Head_Pointer == queue->Tail_Pointer)
	{
		queue->Data_Count = 1;			//Set if the queue if full
	}
	pthread_mutex_unlock(&(queue->mulock));		//Release the Mutex Lock
	return 0;
}

int sq_read (msg **m, squeue_t *squeue)			//Use double pointer so that the read message pointer can be returned
{
	pthread_mutex_lock(&(squeue->mulock));
	if(squeue-> Head_Pointer == squeue-> Tail_Pointer && !squeue->Data_Count)		//Checking if the queue is empty
	{
		*m=NULL;
		pthread_mutex_unlock(&(squeue->mulock));
		return -1;
	}
	int i=squeue->Tail_Pointer;
	squeue->msg_pointer[i]->accum_time += (rdtsc() - (squeue->msg_pointer[i]->enqueue_time));	//Calculating and storing the accumulated time of that message in the queue
	*m = squeue->msg_pointer[i];			//Message Transferred
	squeue->Tail_Pointer++;
	squeue->Tail_Pointer %= queue_length;				//Make queue circular
	if(squeue-> Head_Pointer == squeue -> Tail_Pointer)
	{
		squeue->Data_Count = 0;
	}
	pthread_mutex_unlock(&(squeue->mulock));
	return 0;
}

void sq_delete(squeue_t *squeue)
{
	free(squeue);
}