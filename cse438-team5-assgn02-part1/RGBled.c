#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#define cycle_duration 20
#include<pthread.h>
#include "Gpio_func.h"
int click = 0, c1, c2;

void assigngpio(int a)	//General funcion used to initialise the gpio pins for a given IO number
{
	switch(a)
	{
		case 0:
			gpio_export(11);
			gpio_export(32);
			gpio_set_dir(11,1);
			gpio_set_dir(32,1);	
			gpio_set_value(11,0);
			gpio_set_value(32,0);	
		break;
		case 1:
			gpio_export(12);
			gpio_export(28);
			gpio_export(45);
			gpio_set_dir(12,1);
			gpio_set_dir(28,1);
			gpio_set_dir(45,1);
			gpio_set_value(12,0);
			gpio_set_value(28,0);
			gpio_set_value(45,0);
		break;
		case 2:
			gpio_export(13);
			gpio_export(34);
			gpio_export(77);
			gpio_set_dir(13,1);
			gpio_set_dir(34,1);
			gpio_set_value(13,0);
			gpio_set_value(34,0);
			gpio_set_value(77,0);
		break;
		case 3:
			gpio_export(14);
			gpio_export(16);
			gpio_export(76);
			gpio_export(64);
			gpio_set_dir(14,1);
			gpio_set_dir(16,1);
			gpio_set_value(14,0);
			gpio_set_value(16,0);
			gpio_set_value(76,0);
			gpio_set_value(64,0);
		break;
		case 4:
			gpio_export(6);
			gpio_export(36);
			gpio_set_dir(6,1);
			gpio_set_dir(36,1);
			gpio_set_value(6,0);
			gpio_set_value(36,0);
		break;	
		case 5:
			gpio_export(0);
			gpio_export(18);
			gpio_export(66);
			gpio_set_dir(0,1);
			gpio_set_dir(18,1);
			gpio_set_value(0,0);
			gpio_set_value(18,0);
			gpio_set_value(66,0);
		break;
		case 6:
			gpio_export(1);
			gpio_export(20);
			gpio_export(68);
			gpio_set_dir(1,1);
			gpio_set_dir(20,1);
			gpio_set_value(1,0);
			gpio_set_value(20,0);
			gpio_set_value(68,0);
		break;
		case 7:
			gpio_export(38);
			gpio_set_dir(38,1);
			gpio_set_value(38,0);
		break;
		case 8:
			gpio_export(40);
			gpio_set_dir(40,1);
			gpio_set_value(40,0);
		break;
		case 9:
			gpio_export(4);
			gpio_export(22);
			gpio_export(70);
			gpio_set_dir(4,1);
			gpio_set_dir(22,1);
			gpio_set_value(4,0);
			gpio_set_value(22,0);
			gpio_set_value(70,0);
		break;
		case 10:  
			gpio_export(10);
			gpio_export(26);
			gpio_export(74);
			gpio_set_dir(10,1);
			gpio_set_dir(26,1);
			gpio_set_value(10,0);
			gpio_set_value(26,0);
			gpio_set_value(74,0);
		break;
		case 11: 
			gpio_export(5);
			gpio_export(24);
			gpio_export(44);
			gpio_export(72);
			gpio_set_dir(5,1);
			gpio_set_dir(24,1);
			gpio_set_dir(44,1);
			gpio_set_value(5,0);
			gpio_set_value(24,0);
			gpio_set_value(44,0);
			gpio_set_value(72,0);
		break;
		case 12: 
			gpio_export(15);
			gpio_export(42);
			gpio_set_dir(15,1);
			gpio_set_dir(42,1);
			gpio_set_value(15,0);
			gpio_set_value(42,0);
		break;	
		case 13:
			gpio_export(7);
			gpio_export(30);
			gpio_export(46);
			gpio_set_dir(7,1);
			gpio_set_dir(30,1);
			gpio_set_dir(46,1);
			gpio_set_value(7,0);
			gpio_set_value(30,0);
			gpio_set_value(46,0);
		break;
	}

}

void IOSetup(int duty_cycle, int r, int g, int b)
{
	int FdExport;
	c1 = (duty_cycle/100)*cycle_duration*1000;
	c2 = 20000-c1;
	FdExport = open ("/sys/class/gpio/export", O_WRONLY);
	if(FdExport < 0)
		printf("\nGPIO export open failed\n");
	assigngpio(r);
	assigngpio(g);
	assigngpio(b);
}

void *mouseclick(void *ptr)
{
	int fd,bytes;
	const char *mouse = "/dev/input/mouse0";
	fd = open(mouse, O_RDWR);
    unsigned char data[3];
  	if(fd == -1)
  	{
    	printf("ERROR Opening %s\n", mouse);
    	
  	}
	int left,count=0;
	bool loop=false;
	while(!loop)
   	{
   		++count;
		bytes = read(fd, data, sizeof(data));//Read Mouse
        if(bytes > 0)
        {
            left = data[0] & 0x1;
        }
        if(left==1)
        {
        	click=1;
	  		loop=true;
	  	}
        else if(count==10000)
        {
            loop=true;
        }	
	}
return 0;
}


void LEDOFF(int a)	//General function to set the output value to 0 for a gpio pin for a given IO
{
	switch(a)
	{
		case 0:
			gpio_set_value(11,0);
			gpio_set_value(32,0);	
		break;
		case 1:
			gpio_set_value(12,0);
			gpio_set_value(28,0);
		break;
		case 2:
			gpio_set_value(13,0);
			gpio_set_value(34,0);
		break;
		case 3:
			gpio_set_value(14,0);
			gpio_set_value(16,0);
		break;
		case 4:
			gpio_set_value(6,0);
			gpio_set_value(36,0);
		break;	
		case 5:
			gpio_set_value(0,0);
			gpio_set_value(18,0);
		break;
		case 6:
			gpio_set_value(1,0);
			gpio_set_value(20,0);
		break;
		case 7:
			gpio_set_value(38,0);
		break;
		case 8:
			gpio_set_value(40,0);
		break;
		case 9:
			gpio_set_value(4,0);
			gpio_set_value(22,0);
		break;
		case 10:  
			gpio_set_value(10,0);
			gpio_set_value(26,0);
		break;
		case 11:
			gpio_set_value(5,0);
			gpio_set_value(24,0);
		break;
		case 12: 
			gpio_set_value(15,0);
			gpio_set_value(42,0);
		break;	
		case 13:
			gpio_set_value(7,0);
			gpio_set_value(30,0);
		break;
	}
}
void LEDON(int a)	////General function to set the output value to 1 for a gpio pin for a given IO
{
	switch(a)
	{
		case 0:
			gpio_set_value(11,1);
			gpio_set_value(32,1);	
		break;
		case 1:
			gpio_set_value(12,1);
			gpio_set_value(28,1);
		break;
		case 2:
			gpio_set_value(13,1);
			gpio_set_value(34,1);
		break;
		case 3:
			gpio_set_value(14,1);
			gpio_set_value(16,1);
		break;
		case 4:
			gpio_set_value(6,1);
			gpio_set_value(36,1);
		break;	
		case 5:
			gpio_set_value(0,1);
			gpio_set_value(18,1);
		break;
		case 6:
			gpio_set_value(1,1);
			gpio_set_value(20,1);
		break;
		case 7:
			gpio_set_value(38,1);
		break;
		case 8:
			gpio_set_value(40,1);
		break;
		case 9:
			gpio_set_value(4,1);
			gpio_set_value(22,1);
		break;
		case 10:  
			gpio_set_value(10,1);
			gpio_set_value(26,1);
		break;
		case 11:
			gpio_set_value(5,1);
			gpio_set_value(24,1);
		break;
		case 12: 
			gpio_set_value(15,1);
			gpio_set_value(42,1);
		break;	
		case 13:
			gpio_set_value(7,1);
			gpio_set_value(30,1);
		break;
	}
}
void RED(int r)	//Function to operate the Red LED only
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		usleep(c1);
		LEDOFF(r);
		usleep(c2);
	}
}
void GREEN(int r)	////Function to operate the Green LED only
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		usleep(c1);
		LEDOFF(r);
		usleep(c2);
	}
}
void BLUE(int r)	//Function to operate the Blue LED only
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		usleep(c1);
		LEDOFF(r);
		usleep(c2);
	}
}
void RG(int r, int g)	//Function to operate the Red and Green LED together
{
	int count=0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		LEDON(g);
		usleep(c1);
		LEDOFF(r);
		LEDOFF(g);
		usleep(c2);
	}
}
void RB(int r, int b)	//Function to operate the Red and Blue LED together
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		LEDON(b);
		usleep(c1);
		LEDOFF(r);
		LEDOFF(b);
		usleep(c2);
	}
}
void BG(int b, int g)	//Function to operate the Blue and Green LED together
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(b);
		LEDON(g);
		usleep(c1);
		LEDOFF(b);
		LEDOFF(g);
		usleep(c2);
	}
}
void RBG(int r, int b, int g)	//Function to operate the Red, Blue and Green LED together
{
	int count = 0;
	while(count <= 25)
	{
		++count;
		LEDON(r);
		LEDON(b);
		LEDON(g);
		usleep(c1);
		LEDOFF(r);
		LEDOFF(b);
		LEDOFF(g);
		usleep(c2);
	}
}
int main()
{
	int r,b,g,duty_cycle,ret;
	r=0;// pin no. connected to lit red LEd
	b=1;// pin no. connected to lit blue LEd
	g=2;// pin no. connected to lit green LEd
	duty_cycle=10;//set duty cycle between 0-100
	pthread_t thread_ID[1];//create thread id for mouse click event
	ret = pthread_create(&thread_ID[1],NULL,mouseclick,NULL);//create thread ,mouseclick
        if(ret!=0)
	printf("error opening thread,err no.:%d",ret);
	IOSetup(duty_cycle,r,g,b);//initialise the mux,directions and values of r,g,b pins
	while(!click)//loop the sequence till mouse click event is not detected
	{
		RED(r);
		BLUE(b);
		GREEN(g);
		RG(r,g);
		BG(b,g);	
		RB(r,b);	
		RBG(r,g,b);

	}
return 0;
}
