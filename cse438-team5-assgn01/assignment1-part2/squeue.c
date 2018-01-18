#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/string.h>
#include<linux/jiffies.h>
#include<linux/semaphore.h>
#include<linux/device.h>
#include<linux/time.h>
#include<linux/moduleparam.h>
#define queue_length 10
static dev_t bus_id;			//Structure where major and minors numbers are embedded
#define DEVICE_NAME "MYDRIVER"

//Time Stamp Counter

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

// Message Structure 

struct msg_details 
{
   int sender_id;               // Source Id 
   int message_id;              // Message Id 
   int enqueue_time;        // Enqueue time stamp
   int accum_time;        // accumulated time in queue
   double *value;
};

struct squeue_dev
{
	struct msg_details* msg_pointer[queue_length];
	int Head_Pointer;					// Pointer Represnting Head
	int Tail_Pointer;					// Pointer Representing Tail
	int Full;						// Data present in Queue
};

static struct dataqueue
{
	struct cdev char_device;		//Structure for character device
	struct squeue_dev q;
	char *name;
	struct semaphore hold;
}*dq[2];

struct class *class1;          /* Tie with the device model */

int open(struct inode *i,struct file *fl)		//Operation to opena  device
{
	struct dataqueue *dataq;
	dataq = container_of(i->i_cdev, struct dataqueue, char_device);
	fl->private_data = dataq;
	printk(KERN_INFO"\n %s is opening \n", dataq->name);
	return 0;
}

int close(struct inode *i, struct file *f1)		//OPeration when file structure is released
{
	struct dataqueue *dataq= f1->private_data;
	printk(KERN_INFO"\n %s is closing\n", dataq->name);
	return 0;

}

ssize_t write(struct file *f1,const struct msg_details *m, size_t count, loff_t *fops)	//Function to write data into the device
{
	struct dataqueue *dataq=f1->private_data;
	if(dataq->q.Full==1 && dataq->q.Head_Pointer==dataq->q.Tail_Pointer)
	{
		printk("\n Queue of %s is full",dataq->name);
		return -1;
	}
	down(&dataq->hold);		//Semaphore value lowred so no other thread can access
	dataq->q.msg_pointer[dataq->q.Tail_Pointer]=(struct msg_details*)kmalloc(sizeof(struct msg_details), GFP_KERNEL);	//Assign memory
	if(copy_from_user (dataq->q.msg_pointer[dataq->q.Tail_Pointer],m,sizeof(struct msg_details)))	//Message taken from userspace to kernel space, 1 denotes failure of operation
		printk("\nUnable to write in %s\n",dataq->name);
	else 
		{
			printk("\nMessage written succesfully\n");
		}
	dataq->q.msg_pointer[dataq->q.Tail_Pointer]->enqueue_time = rdtsc();	//Store the time when the data is enqueued
	dataq->q.Tail_Pointer++;
	dataq->q.Tail_Pointer %= queue_length;
	if (dataq->q.Head_Pointer==dataq->q.Tail_Pointer)		//Checking whether the queue has become full after writing the data
	{
		dataq->q.Full=1;
	}
	up(&dataq->hold);	//Releasing semaphore to enable others to access it
	return 0;
}

ssize_t read(struct file *f1, struct msg_details* m1, size_t count, loff_t *fops)	//Function to read data from the device
{
	int b;
	struct dataqueue *dataq=f1->private_data;
	if((dataq->q.Head_Pointer==dataq->q.Tail_Pointer) && (dataq->q.Full==0))	//Checking whether the accessed data queue is empty
	{
		printk("\nQueue %s empty\n",dataq->name);
		return -1;
	}
	down(&dataq->hold);
	dataq->q.msg_pointer[dataq->q.Head_Pointer]->accum_time += (rdtsc() - (dataq->q.msg_pointer[dataq->q.Head_Pointer]->enqueue_time)); //Calculating the time spent in the queue
	b= copy_to_user(m1,dataq->q.msg_pointer[dataq->q.Head_Pointer],sizeof(m1)); //Message sent to the userspace
	if(b)
		printk("n\nUnable to read %d bytes from %s", b, dataq->name);
	else
		printk("\nMessage read with %d bytes",b);
	kfree(dataq->q.msg_pointer[dataq->q.Head_Pointer]);		//Free kernel space
	dataq->q.Head_Pointer++;
	dataq->q.Head_Pointer %= queue_length;
	if (dataq->q.Head_Pointer==dataq->q.Tail_Pointer)		//Checking whether the queue has become empty after reading the data
	{
		dataq->q.Full=0;
	}
	up(&dataq->hold);
	printk(KERN_INFO "Driver: read()\n");
	return b;

}

struct file_operations squeue_fops = {		//A structure telling the file system about the various functions defined by the user for file operations
	.owner	=	THIS_MODULE,  	     // Owner
	.open	=	open,       // Open method
	.release	=	close,     // Release method
	.read	=	read,	//Read method  
	.write	=	write,       // Write method
};

int __init mydriver_init(void)	//The first function called when module is inserted at boot time
{
	int k,err;
	if(alloc_chrdev_region(&bus_id, 0, 2,DEVICE_NAME)<0)		//Allocate 2 minor numbers starting from 0
	{
		printk(KERN_DEBUG "Can't register device\n");
		return -1;
	}
	if((class1=class_create(THIS_MODULE,DEVICE_NAME)) == NULL)
	{
		unregister_chrdev_region(bus_id, 2);		//Device number is freed, cleanup function
		return -1;
	}
	for(k=0;k<2;k++)		//Dataqueus are initislaised 
	{
		dq[k] = (struct dataqueue*) kmalloc(sizeof(struct dataqueue), GFP_KERNEL);
		if(!dq[k])
		{
			printk("Bad malloc\n");
			return -ENOMEM;
		}
		dq[k]->q.Head_Pointer=0;
		dq[k]->q.Tail_Pointer=0;
		dq[k]->q.Full=0;
		sema_init(&(dq[k]->hold),1);
	}
	dq[0]->name="dataqueue1";
	dq[1]->name="dataqueue2";
	for(k=0;k<2;k++)
	{
			if(device_create(class1, NULL, MKDEV(MAJOR(bus_id), MINOR(bus_id)+k), NULL, "dataqueue%d", k+1) == NULL)	//Dataqueues are created
			{
				class_destroy(class1);
				unregister_chrdev_region(bus_id, 2);
				printk(KERN_DEBUG "Unable to register device\n");
				return -1;
			}
	}
	for(k=0;k<2;k++)
	{
		cdev_init(&dq[k]->char_device, &squeue_fops);		//Allocating the device as character device and linking to the various user defined file operation functions
		dq[k]->char_device.owner = THIS_MODULE;
		err=cdev_add(&dq[k]->char_device, MKDEV(MAJOR(bus_id),MINOR(bus_id)+k), 1);	//Addind the character device using the assigned major and minor numbers, error returns 1
		if(err)
		{
			printk("Bad cdev\n");
			return err;
		}
	}
	printk(KERN_INFO "Dataqueues Created");
	return 0;
}

void __exit mydriver_exit(void)		//The function called during module removal time
{
	int k;
	for(k=0;k<2;k++)
	{
		cdev_del(&dq[k]->char_device);
		device_destroy(class1,MKDEV(MAJOR(bus_id),MINOR(bus_id)+k));		//Removing the device upon removal of module from the kernelspaace
		kfree(dq[k]);
	}
	class_destroy(class1);
	unregister_chrdev_region(bus_id,2);		//Device numbers are freed
}
module_init(mydriver_init);		//Information passed as to which is the user defined init function
module_exit(mydriver_exit);		//Information passed as to which is the user defined exit function
MODULE_LICENSE("GPL v2");		//License to be mentioned as GPL which is an open source license otherwise kernel would give error
