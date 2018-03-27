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
extern void init_dist_sensor();

extern float kalman_filter(float last_result,float last_value,float cur_value,
float time,float* p_next,float Q_offset,float R_offset);
extern void mlist_add(M_Node node);
extern void mlist_clear(M_Pointer head);

extern sem_t sensor_start,sensor_mid,sensor_stop;

float pxl_conv=0.5;
float pyl_conv=0.5;
float pzl_conv=0.5;
float pxa_conv=0.5;
float pya_conv=0.5;
float pza_conv=0.5;
const float Q_offset=0.5;
const float R_offset=0.25;
const float dt=0.1;

//extern int cond;
//extern pthread_mutex_t thread_mutex;
//extern pthread_cond_t thread_cond;
///////////////////////////////////////////////
static void sensor_cleanup_handler(void *arg)
{
	sem_destroy(&sensor_start);

	sem_destroy(&sensor_mid);

	sem_destroy(&sensor_stop);
}
////////////////////////////////////////////////
void* temper_get_thread(void)
{
	float value=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
while(1)
	{
		pthread_testcancel();
		sem_wait(&sensor_start);
		while(ctrl_cmd.info_get_func==INFO_GET_DISABLE)
		pthread_testcancel();
//		usleep(100000);
		value=temper_read();
	if(value!=-1)
		M_info.temper=value;
	else goto nothing;

		sem_post(&sensor_mid);
//		pthread_testcancel();
	}
nothing:
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
////////////////////////////////////////////////
void* dist_get_thread(void)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
	init_dist_sensor();
while(1)
	{
		pthread_testcancel();

		sem_wait(&sensor_mid);
//		usleep(100000);
		M_info.dist=dist_read();
		sem_post(&sensor_stop);

		pthread_testcancel();
	}
        pthread_cleanup_pop(0);
}
//////////////////////////////////////////////
void* accel_get_thread(void)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler,NULL);
	int fd=init_mpu6050();
while(1)
	{
		pthread_testcancel();

		sem_wait(&sensor_stop);
//		usleep(100000);
	memcpy(M_info_pointer,move_ll.M_Tail_pointer,M_NODE_SIZE);
	if(fd!=-1)
	{
		M_info.accel_info.xa_accel=xa_read(fd);
		M_info.accel_info.ya_accel=ya_read(fd);
		M_info.accel_info.za_accel=za_read(fd);
		M_info.accel_info.xl_accel=xl_read(fd);
		M_info.accel_info.yl_accel=yl_read(fd);
		M_info.accel_info.zl_accel=zl_read(fd);
	}
	else
	{
	printf("the dev is not found");
	goto nothing;
	}

	if(M_info_pointer->prev!=NULL){
M_info.vel_info.xl_vel=M_info_pointer->prev->vel_info.xl_vel+
M_info.accel_info.xl_accel*dt;

M_info.vel_info.yl_vel=M_info_pointer->prev->vel_info.yl_vel+
M_info.accel_info.yl_accel*dt;

M_info.vel_info.zl_vel=M_info_pointer->prev->vel_info.zl_vel+
M_info.accel_info.zl_accel*dt;

M_info.vel_info.xa_vel=M_info_pointer->prev->vel_info.xa_vel+
M_info.accel_info.xa_accel*dt;

M_info.vel_info.ya_vel=M_info_pointer->prev->vel_info.ya_vel+
M_info.accel_info.ya_accel*dt;

M_info.vel_info.za_vel=M_info_pointer->prev->vel_info.za_vel+
M_info.accel_info.za_accel*dt;

M_info.jour_info.xl=kalman_filter(M_info_pointer->prev->jour_info.xl,
M_info_pointer->prev->accel_info.xl_accel,
M_info.accel_info.xl_accel,
dt,&pxl_conv,Q_offset,R_offset);

M_info.jour_info.yl=kalman_filter(M_info_pointer->prev->jour_info.yl,
M_info_pointer->prev->accel_info.yl_accel,
M_info.accel_info.yl_accel,
dt,&pyl_conv,Q_offset,R_offset);

M_info.jour_info.zl=kalman_filter(M_info_pointer->prev->jour_info.zl,
M_info_pointer->prev->accel_info.zl_accel,
M_info.accel_info.zl_accel,
dt,&pzl_conv,Q_offset,R_offset);
		}
mlist_add(M_info);

if(move_ll.count==MAX_NODE_NUM)
    {
memcpy(move_ll.M_Head_pointer->next,move_ll.M_Tail_pointer,M_NODE_SIZE);
mlist_clear(move_ll.M_Head_pointer->next->next);
    }
	sem_post(&sensor_start);

//		pthread_testcancel();
	}
nothing:
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);

}
///////////////////////////////////////////////

////////////////////////////////////////////////
