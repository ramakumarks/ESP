#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
struct rgb_led
{
	int intensity,r,b,g;
};
#define CONFIG  _IOW('values',1, struct rgb_led *)
#endif 