#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/timer.h>   /* Needed for timer */
#include <linux/semaphore.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/gpio.h>
#include <linux/types.h>
#include <linux/ioctl.h>

#include "RGBLed.h"
#define DEVICE_NAME "RGBLed"
struct led
{
	struct rgb_led strled;	//Variable of structure that holds the value of the gpio pin
	struct cdev ledcdev;	//cdev structure
};
static dev_t LEDdev_number;	//Allotted device number
struct class *LEDdev_class;		//Tie with device model
static struct device *LEDdev_device;	
struct led *LEDdev_vp;

void requestgpio(int a)	//General funcion used to initialise the gpio pins for a given IO number
{
	switch(a)
	{
		case 0:
			gpio_request(11,"sysfs");	//Allocates gpio in the system
			gpio_request(32,"sysfs");
				
		break;
		case 1:
			gpio_request(12,"sysfs");
			gpio_request(28,"sysfs");
			gpio_request(45,"sysfs");
			
		break;
		case 2:
			gpio_request(13,"sysfs");
			gpio_request(34,"sysfs");
			gpio_request(77,"sysfs");
			
		break;
		case 3:
			gpio_request(14,"sysfs");
			gpio_request(16,"sysfs");
			gpio_request(76,"sysfs");
			gpio_request(64,"sysfs");
			
		break;
		case 4:
			gpio_request(6,"sysfs");
			gpio_request(36,"sysfs");
			
			
		break;	
		case 5:
			gpio_request(0,"sysfs");
			gpio_request(18,"sysfs");
			gpio_request(66,"sysfs");
			
		break;
		case 6:
			gpio_request(1,"sysfs");
			gpio_request(20,"sysfs");
			gpio_request(68,"sysfs");
			
		break;
		case 7:
			gpio_request(38,"sysfs");
			
		break;
		case 8:
			gpio_request(40,"sysfs");
			
		break;
		case 9:
			gpio_request(4,"sysfs");
			gpio_request(22,"sysfs");
			gpio_request(70,"sysfs");
			
		break;
		case 10:  
			gpio_request(10,"sysfs");
			gpio_request(26,"sysfs");
			gpio_request(74,"sysfs");
			
		break;
		case 11: 
			gpio_request(5,"sysfs");
			gpio_request(24,"sysfs");
			gpio_request(44,"sysfs");
			gpio_request(72,"sysfs");
			
		break;
		case 12: 
			gpio_request(15,"sysfs");
			gpio_request(42,"sysfs");
			
		break;	
		case 13:
			gpio_request(7,"sysfs");
			gpio_request(30,"sysfs");
			gpio_request(46,"sysfs");
			
		break;
	}

}
void assigngpio(int a)	//General funcion used to initialise the gpio pins for a given IO number
{
	switch(a)
	{
		case 0:
			gpio_export(11,0);
			gpio_export(32,0);
			gpio_direction_output(11,1);
			gpio_direction_output(32,1);	
			gpio_set_value(11,0);
			gpio_set_value(32,0);	
		break;
		case 1:
			gpio_export(12,0);
			gpio_export(28,0);
			gpio_export(45,0);
			gpio_direction_output(12,1);
			gpio_direction_output(28,1);
			gpio_direction_output(45,1);
			gpio_set_value(12,0);
			gpio_set_value(28,0);
			gpio_set_value(45,0);
		break;
		case 2:
			gpio_export(13,0);
			gpio_export(34,0);
			gpio_export(77,0);
			gpio_direction_output(13,1);
			gpio_direction_output(34,1);
			gpio_set_value(13,0);
			gpio_set_value(34,0);
			gpio_set_value(77,0);
		break;
		case 3:
			gpio_export(14,0);
			gpio_export(16,0);
			gpio_export(76,0);
			gpio_export(64,0);
			gpio_direction_output(14,1);
			gpio_direction_output(16,1);
			gpio_set_value(14,0);
			gpio_set_value(16,0);
			gpio_set_value(76,0);
			gpio_set_value(64,0);
		break;
		case 4:
			gpio_export(6,0);
			gpio_export(36,0);
			gpio_direction_output(6,1);
			gpio_direction_output(36,1);
			gpio_set_value(6,0);
			gpio_set_value(36,0);
		break;	
		case 5:
			gpio_export(0,0);
			gpio_export(18,0);
			gpio_export(66,0);
			gpio_direction_output(0,1);
			gpio_direction_output(18,1);
			gpio_set_value(0,0);
			gpio_set_value(18,0);
			gpio_set_value(66,0);
		break;
		case 6:
			gpio_export(1,0);
			gpio_export(20,0);
			gpio_export(68,0);
			gpio_direction_output(1,1);
			gpio_direction_output(20,1);
			gpio_set_value(1,0);
			gpio_set_value(20,0);
			gpio_set_value(68,0);
		break;
		case 7:
			gpio_export(38,0);
			gpio_direction_output(38,1);
			gpio_set_value(38,0);
		break;
		case 8:
			gpio_export(40,0);
			gpio_direction_output(40,1);
			gpio_set_value(40,0);
		break;
		case 9:
			gpio_export(4,0);
			gpio_export(22,0);
			gpio_export(70,0);
			gpio_direction_output(4,1);
			gpio_direction_output(22,1);
			gpio_set_value(4,0);
			gpio_set_value(22,0);
			gpio_set_value(70,0);
		break;
		case 10:  
			gpio_export(10,0);
			gpio_export(26,0);
			gpio_export(74,0);
			gpio_direction_output(10,1);
			gpio_direction_output(26,1);
			gpio_set_value(10,0);
			gpio_set_value(26,0);
			gpio_set_value(74,0);
		break;
		case 11: 
			gpio_export(5,0);
			gpio_export(24,0);
			gpio_export(44,0);
			gpio_export(72,0);
			gpio_direction_output(5,1);
			gpio_direction_output(24,1);
			gpio_direction_output(44,1);
			gpio_set_value(5,0);
			gpio_set_value(24,0);
			gpio_set_value(44,0);
			gpio_set_value(72,0);
		break;
		case 12: 
			gpio_export(15,0);
			gpio_export(42,0);
			gpio_direction_output(15,1);
			gpio_direction_output(42,1);
			gpio_set_value(15,0);
			gpio_set_value(42,0);
		break;	
		case 13:
			gpio_export(7,0);
			gpio_export(30,0);
			gpio_export(46,0);
			gpio_direction_output(7,1);
			gpio_direction_output(30,1);
			gpio_direction_output(46,1);
			gpio_set_value(7,0);
			gpio_set_value(30,0);
			gpio_set_value(46,0);
		break;
	}

}

void LED(int a, int v)	//General function used to switch on and off led
{
	switch(a)
	{
		case 0:
			gpio_set_value(11,v);
			gpio_set_value(32,v);	
		break;
		case 1:
			gpio_set_value(12,v);
			gpio_set_value(28,v);
		break;
		case 2:
			gpio_set_value(13,v);
			gpio_set_value(34,v);
		break;
		case 3:
			gpio_set_value(14,v);
			gpio_set_value(16,v);
		break;
		case 4:
			gpio_set_value(6,v);
			gpio_set_value(36,v);
		break;	
		case 5:
			gpio_set_value(0,v);
			gpio_set_value(18,v);
		break;
		case 6:
			gpio_set_value(1,v);
			gpio_set_value(20,v);
		break;
		case 7:
			gpio_set_value(38,v);
		break;
		case 8:
			gpio_set_value(40,v);
		break;
		case 9:
			gpio_set_value(4,v);
			gpio_set_value(22,v);
		break;
		case 10:  
			gpio_set_value(10,v);
			gpio_set_value(26,v);
		break;
		case 11:
			gpio_set_value(5,v);
			gpio_set_value(24,v);
		break;
		case 12: 
			gpio_set_value(15,v);
			gpio_set_value(42,v);
		break;	
		case 13:
			gpio_set_value(7,v);
			gpio_set_value(30,v);
		break;
	}
}

int open(struct inode *i, struct file *f)		//Function that opens the RGBLed device
{
	struct led* LEDdev_p;
	printk("Opening RGBLed\n");
	LEDdev_p = container_of(i->i_cdev, struct led, ledcdev);	//Get the device structure that contains this ledcdev
	f->private_data = LEDdev_p;
	printk("RGBLed Opened\n");
	return 0;
}
int release(struct inode *i, struct file *f)	//Function that closes the opened RGBLed device
{
	printk("Releasing RGBLed\n");
	printk("Released RGBLed\n");
	return 0;
}
ssize_t write(struct file *f, const char *buf, size_t count, loff_t *ppos) //Writes an integer to enable RGBLed display, the least significant bits indicates which of the pins are to be displayed
{
	
	struct led *LEDdev_p = f->private_data;
	int *led_val;
	led_val = kmalloc(sizeof(int),GFP_KERNEL);
	printk("Begin Write\n");
	if(copy_from_user((void *)led_val,buf,count))
		return -EFAULT;
	printk("led_val = %d", *led_val);
	LED(LEDdev_p->strled.r, (*led_val && 0x01));
	printk("Write Complete\n");
	return 0;
}
static long ioctl_function(struct file *f, unsigned int cmd, unsigned long arg)	//Used to configure the RGBLed
{
	struct led *LEDdev_p = f->private_data;
	printk("Begin ioctl_function\n");
	switch(cmd)
	{
		case CONFIG:
            if (copy_from_user(&LEDdev_p->strled, (struct rgb_led*)arg, sizeof(struct rgb_led)))
            {
                return -1;
            }
            else
            {
            	printk("Copy successful\n");
            }
	    	requestgpio(LEDdev_p->strled.r);
	    	requestgpio(LEDdev_p->strled.g);
	    	requestgpio(LEDdev_p->strled.b);
		    assigngpio(LEDdev_p->strled.r);
	  		assigngpio(LEDdev_p->strled.g);
	    	assigngpio(LEDdev_p->strled.b);
            break;
        default:
            return -EINVAL;	

	}
	printk("Exiting ioctl_function\n");
	return 0;
}
static struct file_operations LEDfops = 
{
	.owner		=	THIS_MODULE,	//Owner
	.open		=	open,			//Open Method
	.write		=	write,			//Write Method
	.unlocked_ioctl		=	ioctl_function,	//IO Control function
	.release		=	release,	//Release Method
};
int __init mydriver_init(void)
{
	int ret;
	printk("Begin mydriver_init\n");
	if(alloc_chrdev_region(&LEDdev_number,0,1,DEVICE_NAME)<0)	//Dynamic allocation of the device number
	{
		printk(KERN_DEBUG "Can't register device\n");
		return -1;
	}
	LEDdev_class = class_create(THIS_MODULE,DEVICE_NAME);	//Populate sysfs entries
	LEDdev_vp = kmalloc(sizeof(struct rgb_led), GFP_KERNEL);	//Allocate memory for the device struucture
	if(!(LEDdev_vp))
	{
		printk("Bad Kmalloc\n");
		return -ENOMEM;
	}
	cdev_init(&LEDdev_vp->ledcdev, &LEDfops);	//Connect the file operations with the cdev
	LEDdev_vp->ledcdev.owner = THIS_MODULE;
	ret = cdev_add(&LEDdev_vp->ledcdev, (LEDdev_number),1);	//Connect the major/minor number to the cdev
	if(ret)
	{
		printk("Bad cdev\n");
		return -1;
	}
	LEDdev_device = device_create(LEDdev_class, NULL, MKDEV(MAJOR(LEDdev_number),0),NULL,DEVICE_NAME);
	printk("mydriver_init Complete\n");
	return 0;
}
void __exit mydriver_exit(void)
{
	printk("Begin mydriver_exit\n");
	unregister_chrdev_region((LEDdev_number),1);	//Unregister the major number
	device_destroy(LEDdev_class,MKDEV(MAJOR(LEDdev_number),0));	//Destroy the created device
	cdev_del(&LEDdev_vp->ledcdev);
	kfree(LEDdev_vp);	//Free allocated memory in the kernel space
	class_destroy(LEDdev_class);	//Destory driver class
	printk("mydriver_exit complete\n");

}
module_init(mydriver_init);
module_exit(mydriver_exit);
MODULE_LICENSE("GPL v2");