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
extern float xa_read();
extern float ya_read();
extern float za_read();
extern float xl_read();
extern float yl_read();
extern float zl_read();

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
while(1)
	{
		pthread_testcancel();
		msleep(100);
		m_info.accel_info.xa_accel=xa_read();
		m_info.accel_info.ya_accel=ya_read();
		m_info.accel_info.za_accel=za_read();
		m_info.accel_info.xl_accel=xl_read();
		m_info.accel_info.yl_accel=yl_read();
		m_info.accel_info.zl_accel=zl_read();
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
