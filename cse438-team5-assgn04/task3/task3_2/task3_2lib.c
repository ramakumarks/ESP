
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>


struct mysiglib_struct
{
    pthread_t kill_threadId;
    struct mysiglib_struct *next;
};
struct mysiglib_struct *head = NULL, *current = NULL;
volatile bool deinit_flag = false;


int mysiglib_reg_handler(pthread_t threadid)
{
	struct mysiglib_struct *ptr;
	if (NULL == head)
	{
		ptr = (struct mysiglib_struct*)malloc(sizeof(struct mysiglib_struct));
		if (NULL == ptr)
		{
			printf("\n linked list Node creation failed \n");
			return -1;
		}
		else
		{
			ptr->kill_threadId = threadid;
			ptr->next = NULL;
			head = ptr;
			current = ptr;
		}
	}
	else
	{
		ptr = (struct mysiglib_struct*)malloc(sizeof(struct mysiglib_struct));
		if (NULL == ptr)
		{
			printf("\n linked list Node creation failed \n");
			return -1;
		}
		else
		{
			ptr->kill_threadId = threadid;
			ptr->next = NULL;
			current->next = ptr;
			current = ptr;
		}
	}

	return 0;
}

static struct mysiglib_struct* search_in_list(pthread_t val, struct mysiglib_struct **prev)
{
    struct mysiglib_struct *ptr = head;
    struct mysiglib_struct *tmp = NULL;
    bool found = false;

    while(ptr != NULL)
    {
        if(ptr->kill_threadId == val)
        {
            found = true;
            break;
        }
        else
        {
            tmp = ptr;
            ptr = ptr->next;
        }
    }

    if(true == found)
    {
        if(prev)
            *prev = tmp;
        return ptr;
    }
    else
    {
        return NULL;
    }
}
/* *********************************************************************
 * NAME:             mysiglib_unreg_handler
 * CALLED BY:        user program
 * DESCRIPTION:      user program to unregister a signal handler for SIGIO
 * INPUT PARAMETERS: threadid which is to be unregistered
 * RETURN VALUES:    0 if success
 * REFERENCE:		 http://www.thegeekstuff.com/2012/08/c-linked-list-example/
 ***********************************************************************/
int mysiglib_unreg_handler(pthread_t threadid)
{
    struct mysiglib_struct *prev = NULL;
    struct mysiglib_struct *del = NULL;

    del = search_in_list(threadid,&prev);
    if(del == NULL)
    {
        return -1;
    }
    else
    {
        if(prev != NULL)
            prev->next = del->next;

        if(del == current)
        {
            current = prev;
        }
        else if(del == head)
        {
            head = del->next;
        }
    }
    free(del);
    del = NULL;

    return 0;
}

void SignalHandlerLibrary(int sig_num)
{
	struct mysiglib_struct *ptr = head;

	/* send the SIGIO signal to all the registered threads*/
    while (ptr != NULL)
    {

		/* to make sure that each mouse event triggers all thread handlers */
		printf("\n");
		pthread_kill(ptr->kill_threadId,SIGUSR1);
        ptr = ptr->next;
    }
    return;
}

/* *********************************************************************
 * NAME:             mysiglib_main
 * CALLED BY:        kernel
 * DESCRIPTION:      Library mainfunction that receives the SIGIO signal
 * INPUT PARAMETERS: none
 * RETURN VALUES:    None
 ***********************************************************************/
void* mysiglib_main(void* dummy)
{
	struct sigaction SignalAction;
	sigset_t block_mask;
	int MouseEventFd,ret,flags;
	
	/* open mice device to capture mouse events */
	if((MouseEventFd = open("/dev/input/mice", O_RDONLY)) < 0)
	{
		perror("opening device");
		return 0;
	}

    /* Unblock signal for SIGIO */
	memset(&SignalAction, 0, sizeof(SignalAction));
	sigfillset(&block_mask);
	SignalAction.sa_mask = block_mask;
	SignalAction.sa_handler = &SignalHandlerLibrary;
	sigaction(SIGIO, &SignalAction, NULL);
	/* Enable asynchronous signal notification for mice */
	ret = fcntl(MouseEventFd, F_SETOWN, getpid());
	if (ret < 0)
	{
		perror("fcntl1");
	}
	flags = fcntl(MouseEventFd, F_GETFL);
	fcntl(MouseEventFd, F_SETFL, flags | FASYNC);
	/* run till cntrl+c is pressed */
	while(deinit_flag == false)
	{
		;
	}
	return NULL;
}

void mysiglib_init(void)
{
	pthread_t mysiglib_main_threadid;
	pthread_create(&mysiglib_main_threadid,NULL,&mysiglib_main,NULL);

}

void mysiglib_deinit(void)
{
	deinit_flag = true;

}
