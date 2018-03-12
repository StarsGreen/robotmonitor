#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/shm.h>
#include "global_data.h"



extern float temper_read();
extern float dist_read();
extern float xa_read(int fd);
extern float ya_read(int fd);
extern float za_read(int fd);
extern float xl_read(int fd);
extern float yl_read(int fd);
extern float zl_read(int fd);
extern int init_mpu6050();

////////////////////////////////////////////////
void* temper_get_thread()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
while(1)
	{
		pthread_testcancel();
		msleep(500);
		m_info.temper=temper_read();
	}

}
//////////////////////////////////////////////
void* accel_get_thread()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	int fd=init_mpu6050();
while(1)
	{
		pthread_testcancel();
		msleep(100);
		m_info.accel_info.xa_accel=xa_read(fd);
		m_info.accel_info.ya_accel=ya_read(fd);
		m_info.accel_info.za_accel=za_read(fd);
		m_info.accel_info.xl_accel=xl_read(fd);
		m_info.accel_info.yl_accel=yl_read(fd);
		m_info.accel_info.zl_accel=zl_read(fd);
	}

}
///////////////////////////////////////////////

void* dist_get_thread()
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
while(1)
	{
		pthread_testcancel();
		msleep(250);
		m_info.dist=dist_read();
	}

}
////////////////////////////////////////////////
