#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include "RGBLed.h"


struct rgb_led values;
int click = 0;
void set_config(int fd)
{
	printf("In set_config\n");
	if(ioctl(fd,CONFIG,&values) == -1)
    {
        perror("query_apps ioctl set");
    }

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
	  		printf("Click detected\n");
	  	}
        else if(count==10000)
        {
            loop=true;
        }	
	}
return 0;
}
int main()
{
	values.r=4;
	values.b=8;
	values.g=12;
	values.intensity = 50;
	printf("In main\n");
	values.intensity=50;
	int fd,k;
	int seq,ret;
	fd=open("/dev/RGBLed",O_RDWR);
	if(fd<0)
		printf("Device Open error %d\n",fd);
	else
		printf("Device Opened %d\n", fd);

	pthread_t thread_ID[1];
	ret = pthread_create(&thread_ID[1],NULL,mouseclick,NULL);//create thread ,mouseclick
    if(ret!=0)
    	printf("Error openeing thread\n");
	while(!click)
	{
		set_config(fd);
		seq=0x01;
		printf("Inside while loop\n");
		k=write(fd,&seq,sizeof(seq));
		seq=0x02;
		k=write(fd,&seq,sizeof(seq));
		seq=0x04;
		k=write(fd,&seq,sizeof(seq));
		seq=0x03;
		k=write(fd,&seq,sizeof(seq));
		seq=0x05;
		k=write(fd,&seq,sizeof(seq));
		seq=0x07;
		k=write(fd,&seq,sizeof(seq));
		if(k<0)
    	{
      		errno=EINVAL;
      
   		}
	}
	return 0;
}