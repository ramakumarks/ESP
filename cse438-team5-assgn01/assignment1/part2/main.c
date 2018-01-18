#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include<sys/time.h>
#include<time.h>
#include<errno.h>
#include<math.h>
#include <stdbool.h>
#include <sched.h>

#define CPU_CLOCKS_PER_MILLISECOND 2500000
#define queue_length 10
typedef struct msg_details 
{
   int sender_id;               // Source Id 
   int message_id;              // Message Id 
   int enqueue_time;        // Enqueue time stamp
   int accum_time;        // accumulated time in queue
   double value;
} msg;

typedef struct squeue_dev
{
	msg *msg_pointer[queue_length];
	int Head_Pointer;					// Pointer Represnting Head
	int Tail_Pointer;					// Pointer Representing Tail
	int Full;						// Data present in Queue
} squeue_t;

void* sender_function(void* ptr);
void* receiver_function(void *ptr);
void* mousecl(void *ptr);

int count=0,tx_count=0,rx_count=0;
double  pi_value();
int stop_fwd=0,stop_rec,cexit=1;

const int p_period_multiplier[4] = {12, 32, 18, 28};  //Period Multiplier for the sender
const int r_period_multiplier = 40;              //Period multipler for the reciever
const int thread_priority [] = {0,1};
double queueing_time[2000];


pthread_mutex_t hold = PTHREAD_MUTEX_INITIALIZER;     //Initialise mutex lock
  
int main()
{
  int i,j,ret;
  pthread_t thread_ID[6];                             //6Tx and 1Rx thread
  pthread_attr_t attr1[2];                             //Thread attributes
  pthread_attr_t attr2[4];
  int s_id[4]={0,1,2,3};
  int sa_id[2]={0,1};
  struct sched_param param1[2];
  struct sched_param param2[4];
  const int* sid[4]={&s_id[0],&s_id[1],&s_id[2],&s_id[3]};  //Periodic Sender's IDs
  const int* said[2]={&sa_id[0],&sa_id[1]};//Aperiodic Senders IDs
  int r_id=0;
  const int* rid=&r_id;//Receiver's ID
  for(i=1,j=0;i<3;i++,j++)
  {                         //Initialising the aperiodic sender threads with default values
    ret=pthread_attr_init (&attr1[i]);
    ret=pthread_attr_getschedparam(&attr1[i],&param1[i]);
    param1->sched_priority=thread_priority[0];
    ret=pthread_attr_setschedparam(&attr1[i], &param1[i]);
    ret=pthread_create(&thread_ID[i],NULL,mousecl,(void*) said[j]);
  }
  for(i=1,j=0;i<5;i++,j++)
  {                         //Initialising the periodic sender threads with default values
    
    ret=pthread_attr_init (&attr2[i]);
    ret=pthread_attr_getschedparam(&attr2[i],&param2[i]);
    param2->sched_priority=thread_priority[1];
    ret=pthread_attr_setschedparam(&attr2[i],&param2[i]);
    ret=pthread_create(&thread_ID[i],NULL,sender_function,(void*) sid[j]);
  }
   
  pthread_create(&thread_ID[0],NULL,receiver_function,(void*) rid);  
  if(ret==0);
  for(i=0;i<6;i++){
  pthread_join(thread_ID[i],NULL);}
  queueing_time[rx_count]='\0';
  printf("Double Click detected\nGood Bye. Have a nice day.\n");
  exit(EXIT_SUCCESS);
}
void* sender_function(void *ptr)
{
  struct timespec next,period;    //Structure containing variables to hold seconds and nanoseconds
  clock_gettime(CLOCK_MONOTONIC,&next);  
  int* source_id;
  source_id=(int *)ptr;
  period.tv_sec =0;
   
  period.tv_nsec= p_period_multiplier[*source_id]*1000000;  //Setting the periodicity of each periodic thread
  int k,fd1,fd2;
  msg* m1;
  m1=(msg*)malloc(sizeof(msg));
  while(cexit)
  {
	fd1=open("/dev/dataqueue1",O_RDWR);
	fd2=open("/dev/dataqueue2",O_RDWR);
    
    m1->sender_id=*source_id;    //Set the values of the current message
    m1->enqueue_time=0;
    pthread_mutex_lock(&hold);
    m1->message_id=++count;
    pthread_mutex_unlock(&hold);
    m1->value=pi_value();      //Storing the value of pi calcualted after a random number of iterations.
    if(m1->sender_id<3)
    {
      k=write(fd1,m1,sizeof(msg));
      printf("Sending message %d from %d to dataqueue1:%lf \n",m1->message_id,m1->sender_id,m1->value);
    } 
    else
    {
      k=write(fd2,m1,sizeof(msg));
      printf("Sending message %d from %d to dataqueue2:%lf \n",m1->message_id,m1->sender_id,m1->value);
    }
    if(k<0)
    {
      errno=EINVAL;
    }
	
    else
    {
      pthread_mutex_lock(&hold);
      tx_count++;
      pthread_mutex_unlock(&hold);
    }
	close(fd1);
	close(fd2);
   if((next.tv_nsec+period.tv_nsec)>=1000000000)
    {
      next.tv_nsec=(next.tv_nsec+period.tv_nsec)%1000000000;
      next.tv_sec++;
    }
   else
    {
      next.tv_nsec=next.tv_nsec+period.tv_nsec;
    }
    clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&next,0);
    }
free(m1);	
return(NULL);
}
void *receiver_function(void *ptr)
{
  struct timespec next,period;
  clock_gettime(CLOCK_MONOTONIC, &next);
  int fd1,fd2,k;
  msg *m1;
  m1=(msg*) malloc(sizeof(msg));
  period.tv_sec=0;
  period.tv_nsec=r_period_multiplier*1000000;
  while(cexit)
  { 
	fd1=open("/dev/dataqueue1",O_RDWR);
	fd2=open("/dev/dataqueue2",O_RDWR);
    k=read(fd1,m1,sizeof(msg));
    if (k==0)
    {
    printf("Message at the receiver: source id:%d, message id: %d\n",m1->sender_id,m1->message_id);
    pthread_mutex_lock(&hold);
    rx_count++;
    queueing_time[rx_count-1]=((double)(m1->accum_time))/CPU_CLOCKS_PER_MILLISECOND;
    pthread_mutex_unlock(&hold);
    }
    else
    {
     errno = EINVAL;
     pthread_mutex_lock(&hold);
     if(stop_fwd == 1)
      stop_rec=1;
     pthread_mutex_unlock(&hold);
    }
    k=read(fd2,m1,sizeof(msg));
    if (k==0)
    {
    printf("Message at the receiver: source id:%d, message id: %d\n",m1->sender_id,m1->message_id);
    pthread_mutex_lock(&hold);
    rx_count++;
    queueing_time[rx_count-1]=((double)(m1->accum_time))/CPU_CLOCKS_PER_MILLISECOND;
    pthread_mutex_unlock(&hold);
    }
    else
    {
      errno = EINVAL;
      pthread_mutex_lock(&hold);
      if(stop_fwd == 1)
        stop_rec=1;
      pthread_mutex_unlock(&hold);
	}
	close(fd1);
  	close(fd2);
    if((next.tv_nsec+period.tv_nsec)>=1000000000) 
    {
      next.tv_nsec=((next.tv_nsec+period.tv_nsec)%1000000000);
      next.tv_sec++;
    }
    else
    {
      next.tv_nsec=next.tv_nsec+period.tv_nsec;
    }
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, 0);
  }
  free(m1);
  return(NULL);
}
double pi_value() 
{
  double i;       // Number of iterations and control variable
  double s = 1;      //Signal for the next iteration
  double pi = 0;
  int n;
  n=rand() % 40 +11;     
  for(i = 1; i <= (n * 2); i += 2)
  {
    pi = pi + s * (4 / i);
    s = -s;
  }
  return pi;
}

void *mousecl(void *ptr)
{
  int* source_id;
  source_id=(int *)ptr;
  int k;
  msg* m1;
  int fd,fd1,fd2, bytes;
  int count=0;
  unsigned char data[3];
  struct timeval timer_for_double_click,current_time;     
  memset((void*)&timer_for_double_click,0,sizeof(timer_for_double_click));  
  memset((void*)&current_time,0,sizeof(current_time));
  unsigned long time_difference; 
  const char *mouse = "/dev/input/mouse0";
  bool one_click = false;
  //float timer_for_double_click;
  // Open Mouse
  fd = open(mouse, O_RDWR);
  if(fd == -1)
  {
    printf("ERROR Opening %s\n", mouse);
   // return -1;
  }
  int left,right;
  while(1)
  {  
    bytes = read(fd, data, sizeof(data));//Read Mouse
    if(bytes > 0)
        {
          left = data[0] & 0x1;
          right = data[0] & 0x2;
         // printf("left=%d,right=%d\n",left,right);
        }
      if(left==1)
      {
        if(!one_click) // first click no previous clicks
        {
          one_click = true;
          gettimeofday(&timer_for_double_click,NULL);
        } 
        else
        {
        one_click = false; // found a double click, now reset
        }
      }
    if(one_click)
    { 
      gettimeofday(&current_time,NULL);
      time_difference=(1000000*current_time.tv_sec+current_time.tv_usec)-(1000000*timer_for_double_click.tv_sec+timer_for_double_click.tv_usec);
      if(time_difference > 0 && time_difference<500000){
       //printf("Double Clicked\n");
      cexit=0;
      }
      one_click = false;
      
    }
    m1=(msg*)malloc(sizeof(msg));
    m1->sender_id=*source_id;    //Set the values of the current message
    m1->enqueue_time=0;
    pthread_mutex_lock(&hold);
    m1->message_id=++count;
    pthread_mutex_unlock(&hold);
    m1->value=pi_value();      //Storing the value of pi calcualted after a random number of iterations.
	fd1=open("/dev/dataqueue1",O_RDWR);
	fd2=open("/dev/dataqueue2",O_RDWR);
    if(left==1 && m1->sender_id == 0)
    {
      k=write(fd1,m1,sizeof(msg));
      printf("Sending message %d from aperiodic %d to dataqueue1:%lf \n",m1->message_id,m1->sender_id,m1->value);
    } 
    else if(right==2 && m1->sender_id == 1)
    {
      k=write(fd2,m1,sizeof(msg));

      printf("Sending message %d from aperiodic %d to dataqueue2:%lf \n",m1->message_id,m1->sender_id,m1->value);
    }
    if(k<0)
    {
      errno=EINVAL;
      
    }
    else
    {
      pthread_mutex_lock(&hold);
      tx_count++;
      pthread_mutex_unlock(&hold);
    }
	free(m1);
  }
}
