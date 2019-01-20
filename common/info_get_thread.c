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
#include <sys/time.h>
#include "data_structure.h"
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
//extern void init_dist_sensor();
//extern void get_move_info();

extern int kalman_filter(mn_ptr result,mn_ptr last_node_ptr,mn_ptr cur_node_ptr);
extern int init_kalman_params();
extern float slide_filter(float cur_value,float last_value);
//extern void mlist_add(M_Node node);
//extern void mlist_clear(void);
//extern void* get_ll_shmid(key_t key,int size);
//extern int rebuild_mlist();

extern int mlist_add_node(mn_ptr mn_p,ml_ptr ml_p);
extern int clear_mlist(ml_ptr ml_p);

extern void print_move_info(mn_ptr mn_p,int flag);

extern sem_t sensor_start,sensor_mid,sensor_stop;

float pxl_conv=0.5;
float pyl_conv=0.5;
float pzl_conv=0.5;
float pxa_conv=0.5;
float pya_conv=0.5;
float pza_conv=0.5;
const float Q_offset=0.5;
const float R_offset=0.25;
static float dt=0;


M_Node M_info;
M_Pointer M_info_pointer;

motion_node m_node,temp_node;
int data_state=0;//show the snesor data state is ready or not
//extern int cond;
//extern pthread_mutex_t thread_mutex;
//extern pthread_cond_t thread_cond;
///////////////////////////////////////////////
static void sensor_cleanup_handler(void *arg)
{
//	sem_destroy(&sensor_start);

//	sem_destroy(&sensor_mid);

//	sem_destroy(&sensor_stop);
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
//		sem_wait(&sensor_start);
//		while(ctrl_cmd.info_get_func==INFO_GET_DISABLE);
                usleep(50000);
		value=temper_read();
	if(value!=0)
		{
//		M_info.temper=value;
                temp_node.temper=value;
//		printf("the temper is %6.3f\n",value);
                value=0;
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
//		sem_post(&sensor_mid);
//printf("temper get finished!\n");
//		pthread_testcancel();
	}
nothing:
//	sem_post(&sensor_mid);
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
//	init_dist_sensor();
while(1)
	{
	start:
		pthread_testcancel();
//		sem_wait(&sensor_mid);
		usleep(50000);
		value=dist_read();
//		printf("the dist is %6.3f\n",value);

		if(value>0)
		{
//		M_info.dist=value;
                temp_node.dist=value;
                value=0;
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
//		sem_post(&sensor_stop);
//printf("dist get finished!\n");
//		pthread_testcancel();
	}
nothing:
//	sem_post(&sensor_start);
	while(1)pthread_testcancel();
        pthread_cleanup_pop(0);
}
//////////////////////////////////////////////
void* accel_get_thread(void)
{
//	int num=0;
	int fd;
/*	float gra_x=0;
        float gra_y=0;
        float gra_z=0;
	float xl_accel;
	float yl_accel;
	float zl_accel;
*/
        long start,stop,dt;//sensor sample time

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler,NULL);

        struct timeval t1,t2;  // 结构体，可以记录秒和微秒两部分值
	memset(&t1,0,sizeof(t1));
        memset(&t2,0,sizeof(t2));

	fd=init_mpu6050();
while(1)
  {
    pthread_testcancel();
    usleep(50000);
    gettimeofday(&t1, NULL);

    temp_node.accel_info.xl_accel=xl_read(fd);
    temp_node.accel_info.yl_accel=yl_read(fd);
    temp_node.accel_info.zl_accel=zl_read(fd);
    temp_node.vel_info.xa_vel=xa_read(fd);
    temp_node.vel_info.ya_vel=ya_read(fd);
    temp_node.vel_info.za_vel=za_read(fd);
    if(!data_state)
        data_state=1;

    gettimeofday(&t2, NULL);

    start = t2.tv_sec*1000000+t2.tv_usec; // 开始时刻
    stop = t1.tv_sec*1000000+t1.tv_usec;  // 结束时刻计算距离
    dt=(float)(stop - start)/1000000;

    temp_node.sample_time=dt;

   }
  pthread_cleanup_pop(0);
}
//////////////////////////////////////////////


//////////////////////////////////////////////
void* collect_info_thread(void)
{
        extern ml_ptr ml_p;
        mn_ptr mp;
	float xl_accel,yl_accel,zl_accel,last_xl,last_yl,last_zl;
        float xa_vel,ya_vel,za_vel,last_xa,last_ya,last_za;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
//	init_dist_sensor();

	while(!data_state);
        mlist_add_node(&temp_node,ml_p);//add the first node as a base to caculate
//      mp=ml_p->tail_ptr
        init_kalman_params();

while(1)
  {
	pthread_testcancel();
	usleep(50000);

        mp=ml_p->tail_ptr;

        xl_accel=temp_node.accel_info.xl_accel;
        yl_accel=temp_node.accel_info.yl_accel;
        zl_accel=temp_node.accel_info.zl_accel;
        xa_vel=temp_node.vel_info.xa_vel;
        ya_vel=temp_node.vel_info.ya_vel;
        za_vel=temp_node.vel_info.za_vel;

//        mp=ml_p->tail_ptr;
        last_xl=mp->accel_info.xl_accel;
        last_yl=mp->accel_info.yl_accel;
        last_zl=mp->accel_info.zl_accel;
        last_xa=mp->vel_info.xa_vel;
        last_ya=mp->vel_info.ya_vel;
        last_za=mp->vel_info.za_vel;

       //use slide filter to deal with oringinal sensor data
        xl_accel=slide_filter(xl_accel,last_xl);
        yl_accel=slide_filter(xl_accel,last_yl);
        zl_accel=slide_filter(xl_accel,last_zl);
        xa_vel=slide_filter(xl_accel,last_xa);
        ya_vel=slide_filter(xl_accel,last_ya);
        za_vel=slide_filter(xl_accel,last_za);

      //use n_node to store sensor data after slide filter
        m_node.accel_info.xl_accel=xl_accel;
        m_node.accel_info.yl_accel=yl_accel;
        m_node.accel_info.zl_accel=zl_accel;
        m_node.vel_info.xa_vel=xa_vel;
        m_node.vel_info.ya_vel=ya_vel;
        m_node.vel_info.za_vel=za_vel;

     //caculate three axis vel info by accel info
        m_node.vel_info.xl_vel=mp->vel_info.xl_vel+xl_accel*ST;
        m_node.vel_info.yl_vel=mp->vel_info.yl_vel+yl_accel*ST;
        m_node.vel_info.zl_vel=mp->vel_info.zl_vel+zl_accel*ST;

     //use kalman filter to proceed journey info

    kalman_filter(&m_node,mp,&temp_node);

    if(ml_p->count==MAX_NODE_NUM)
        clear_mlist(ml_p);
    mlist_add_node(&m_node,ml_p);
    print_move_info(&temp_node,0);

  }
        pthread_cleanup_pop(0);
}
////////////////////////////////////////////////
