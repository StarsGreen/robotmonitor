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
#include <sys/ipc.h>
#include <sys/shm.h>
#include "data_refer.h"
#include "data_config.h"
//#include "cmd.h"

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
extern void get_move_info();

extern float kalman_filter(float last_result,float last_value,float cur_value,
float time,float* p_next,float Q_offset,float R_offset);
extern void mlist_add(M_Node node);
extern void mlist_clear(void);
extern void* get_ll_shmid(key_t key,int size);
extern int rebuild_mlist();
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
	int num=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
while(1)
	{
	start:
		pthread_testcancel();
		sem_wait(&sensor_start);
//		while(ctrl_cmd.info_get_func==INFO_GET_DISABLE)
		value=temper_read();
	if(value!=0)
		{
		M_info.temper=value;
//		printf("the temper is %6.3f\n",value);
		}
	else
		{
		num++;
		if(num==10)
		  {
		printf("can not get the temper data\n");
		num=0;
		goto nothing;
		  }
		goto start;
		}
		sem_post(&sensor_mid);
//		pthread_testcancel();
	}
nothing:
	sem_post(&sensor_mid);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
////////////////////////////////////////////////
void* dist_get_thread(void)
{
	float value;
	int num=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
	init_dist_sensor();
while(1)
	{
	start: 
		pthread_testcancel();

		sem_wait(&sensor_mid);
//		usleep(100000);
		value=dist_read();
//		printf("the dist is %6.3f\n",value);
		if(value>0)
		{
		M_info.dist=value;
//		printf("the dist is %6.3f\n",value);
		}
		else
		{
		num++;
		if(num==10)
		  {
		num=0;
		printf("can not get the dist data\n");
		goto nothing;
		  }
		goto start;
		}
		sem_post(&sensor_stop);
//		pthread_testcancel();
	}
nothing:
	sem_post(&sensor_stop);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
//////////////////////////////////////////////
void* accel_get_thread(void)
{
	int num=0;
	int fd;
	mll_ptr p=NULL;
	M_Pointer tail=NULL;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler,NULL);
start:
	fd=init_mpu6050();
while(1)
	{
		pthread_testcancel();

		sem_wait(&sensor_stop);
//		usleep(100000);
	M_info_pointer=(M_Pointer)malloc(M_NODE_SIZE);
	memset(M_info_pointer,0,M_NODE_SIZE);

	p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	memcpy(M_info_pointer,tail,M_NODE_SIZE);
//	shmdt(mp);
//	shmdt(tail);
	pthread_mutex_unlock(&p->move_ll_lock);
	if(fd!=-1)
	    {
		M_info.vel_info.xa_vel=xa_read(fd);
		M_info.vel_info.ya_vel=ya_read(fd);
		M_info.vel_info.za_vel=za_read(fd);
		M_info.accel_info.xl_accel=xl_read(fd);
		M_info.accel_info.yl_accel=yl_read(fd);
		M_info.accel_info.zl_accel=zl_read(fd);
	    }
	else
	    {
	num++;
	if(num==10)
	      {
	printf("the dev is not found\n");
	goto nothing;
	      }
	goto start;
     	    }
	if(M_info_pointer!=NULL){
M_info.vel_info.xl_vel=M_info_pointer->vel_info.xl_vel+
M_info.accel_info.xl_accel*dt;

M_info.vel_info.yl_vel=M_info_pointer->vel_info.yl_vel+
M_info.accel_info.yl_accel*dt;

M_info.vel_info.zl_vel=M_info_pointer->vel_info.zl_vel+
M_info.accel_info.zl_accel*dt;

M_info.jour_info.xl=kalman_filter(M_info_pointer->jour_info.xl,
M_info_pointer->accel_info.xl_accel,
M_info.accel_info.xl_accel,
dt,&pxl_conv,Q_offset,R_offset);

M_info.jour_info.yl=kalman_filter(M_info_pointer->jour_info.yl,
M_info_pointer->accel_info.yl_accel,
M_info.accel_info.yl_accel,
dt,&pyl_conv,Q_offset,R_offset);

M_info.jour_info.zl=kalman_filter(M_info_pointer->jour_info.zl,
M_info_pointer->accel_info.zl_accel,
M_info.accel_info.zl_accel,
dt,&pzl_conv,Q_offset,R_offset);
		}
		mlist_add(M_info);
//		get_move_info();

if(p->count==MAX_NODE_NUM)
    {
/*
//M_Pointer tem_p=malloc(M_NODE_SIZE);
M_Pointer head=shmat(p->Head_shmid,NULL,0);
//M_Pointer mp0=shmat(head->next_shmid,NULL,0);
//M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
//memcpy(tem_p,tail,M_NODE_SIZE);
p->Tail_shmid=head->next_shmid;
p->count=0;*/
rebuild_mlist();
mlist_add(M_info);
//shmdt(head);
//shmdt(mp0);
//free(tem_p);
    }
	sem_post(&sensor_start);
	free(M_info_pointer);
	shmdt(p);
	shmdt(tail);
//		pthread_testcancel();
	}
nothing:
	sem_post(&sensor_start);
	free(M_info_pointer);
	shmdt(tail);
	shmdt(p);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
///////////////////////////////////////////////

////////////////////////////////////////////////
