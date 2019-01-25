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

extern int kalman_filter(mn_ptr last_node_ptr,mn_ptr cur_node_ptr);
extern int init_kalman_params();
extern float slide_filter(float cur_value,float last_value);
extern int mems_fusion(float gx, float gy, float gz, float ax, float ay, float az,
posture_info* p_info);

extern float get_real_value(float value,float offset);
extern int sensor_check(motion_node* mn,sensor_offset* so);

//extern void mlist_add(M_Node node);
//extern void mlist_clear(void);
//extern void* get_ll_shmid(key_t key,int size);
//extern int rebuild_mlist();

extern int mlist_add_node(mn_ptr mn_p,ml_ptr ml_p);
extern int clear_mlist(ml_ptr ml_p);

extern void print_move_info(mn_ptr mn_p,int flag);

extern sem_t sensor_start,sensor_mid,sensor_stop;
/*
float pxl_conv=0.5;
float pyl_conv=0.5;
float pzl_conv=0.5;
float pxa_conv=0.5;
float pya_conv=0.5;
float pza_conv=0.5;
const float Q_offset=0.5;
const float R_offset=0.25;
static float dt=0;
*/
sensor_offset sensor_off;

M_Node M_info;
M_Pointer M_info_pointer;

motion_node m_node,temp_node;
int temper_state=0,accel_state=0,dist_state=0;//show the snesor data state is ready or not
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
                if(!temper_state)
                  temper_state=1;
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
                if(!dist_state)
                  dist_state=1;
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
        int i=0,sample_freq=20;

	float xa_vel_offset=0;
        float ya_vel_offset=0;
        float za_vel_offset=0;
	float xl_accel_offset=0;
	float yl_accel_offset=0;
	float zl_accel_offset=0;
       //used in loss test
        motion_node t_node;


	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler,NULL);


	fd=init_mpu6050();
        memset(&sensor_off,0,sizeof(sensor_off));
      //first sample to get the loss bass;
       for(i=0;i<sample_freq;i++)
       {
        xa_vel_offset+=xa_read(fd);
        ya_vel_offset+=ya_read(fd);
        za_vel_offset+=za_read(fd);
        xl_accel_offset+=xl_read(fd);
        yl_accel_offset+=yl_read(fd);
        zl_accel_offset+=zl_read(fd);
       }
       t_node.accel_info.xl_accel=xl_accel_offset/sample_freq;
       t_node.accel_info.yl_accel=yl_accel_offset/sample_freq;
       t_node.accel_info.zl_accel=zl_accel_offset/sample_freq;
       t_node.vel_info.xa_vel=xa_vel_offset/sample_freq;
       t_node.vel_info.ya_vel=ya_vel_offset/sample_freq;
       t_node.vel_info.za_vel=za_vel_offset/sample_freq;

      sensor_check(&t_node,&sensor_off);
      printf("gyro_x_offset:%6.5f,gyro__offset:%6.5f,gyro_z_offset:%6.5f\n",
      sensor_off.xa_vel_offset,sensor_off.ya_vel_offset,sensor_off.za_vel_offset);
      printf("gra_x:%6.5f,gra_y:%6.5f,gra_z:%6.5f,zero_offset:%6.5f\n",
      sensor_off.gra_cpt_info.gra_x,
      sensor_off.gra_cpt_info.gra_y,
      sensor_off.gra_cpt_info.gra_z,
      sensor_off.sensor_zero_shift);
while(1)
  {
    pthread_testcancel();
    usleep(5000);
/*
    temp_node.accel_info.xl_accel=xl_read(fd);
    temp_node.accel_info.yl_accel=yl_read(fd);
    temp_node.accel_info.zl_accel=zl_read(fd);
    temp_node.vel_info.xa_vel=xa_read(fd);
    temp_node.vel_info.ya_vel=ya_read(fd);
    temp_node.vel_info.za_vel=za_read(fd);
*/
   printf("sensor_xl:%6.5f\n",xl_read(fd));
   printf("sensor_yl:%6.5f\n",yl_read(fd));
   printf("sensor_zl:%6.5f\n",zl_read(fd));

    temp_node.accel_info.xl_accel=
    get_real_value(xl_read(fd),sensor_off.sensor_zero_shift)
    -sensor_off.gra_cpt_info.gra_x;

    temp_node.accel_info.yl_accel=
    get_real_value(yl_read(fd),sensor_off.sensor_zero_shift)
    -sensor_off.gra_cpt_info.gra_y;

    temp_node.accel_info.zl_accel=
    get_real_value(zl_read(fd),sensor_off.sensor_zero_shift)
    -sensor_off.gra_cpt_info.gra_z;

    temp_node.vel_info.xa_vel=xa_read(fd)-sensor_off.xa_vel_offset;
    temp_node.vel_info.ya_vel=ya_read(fd)-sensor_off.ya_vel_offset;
    temp_node.vel_info.za_vel=za_read(fd)-sensor_off.za_vel_offset;

    if(!accel_state)
    {
      accel_state=1;
    }
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
        float temper,dist,last_temper,last_dist;
        long start,stop;
        float dt;//sensor sample time

//        motion_node t_node;

        struct timeval t1,t2;  // 结构体，可以记录秒和微秒两部分值
	memset(&t1,0,sizeof(t1));
        memset(&t2,0,sizeof(t2));

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
//	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
        pthread_cleanup_push(sensor_cleanup_handler, NULL);
//	init_dist_sensor();

//      usleep(500000);
	while(!temper_state||!dist_state||!accel_state);
//       printf("the last temper is %6.3f\n",temp_node.temper);
        memcpy(&m_node,&temp_node,sizeof(motion_node));


       //add the first node as a base to caculate
        mlist_add_node(&m_node,ml_p);
        print_move_info(ml_p->tail_ptr,0);
//      mp=ml_p->tail_ptr
        init_kalman_params();

while(1)
  {
	pthread_testcancel();

        gettimeofday(&t1, NULL);


	usleep(10000);
 //       temp_node.sample_time=0.05f;

        memcpy(&m_node,&temp_node,sizeof(motion_node));

        mp=ml_p->tail_ptr;

        xl_accel=m_node.accel_info.xl_accel;
        yl_accel=m_node.accel_info.yl_accel;
        zl_accel=m_node.accel_info.zl_accel;
        xa_vel=m_node.vel_info.xa_vel;
        ya_vel=m_node.vel_info.ya_vel;
        za_vel=m_node.vel_info.za_vel;

        temper=m_node.temper;
        dist=m_node.dist;
//      printf("the temper is %6.3f\n",temper);

//      mp=ml_p->tail_ptr;
        last_xl=mp->accel_info.xl_accel;
        last_yl=mp->accel_info.yl_accel;
        last_zl=mp->accel_info.zl_accel;
        last_xa=mp->vel_info.xa_vel;
        last_ya=mp->vel_info.ya_vel;
        last_za=mp->vel_info.za_vel;

        last_temper=mp->temper;
        last_dist=mp->dist;

       //use slide filter to deal with oringinal sensor data
        xl_accel=slide_filter(xl_accel,last_xl);
        yl_accel=slide_filter(yl_accel,last_yl);
        zl_accel=slide_filter(zl_accel,last_zl);
        xa_vel=slide_filter(xa_vel,last_xa);
        ya_vel=slide_filter(ya_vel,last_ya);
        za_vel=slide_filter(za_vel,last_za);

        temper=slide_filter(temper,last_temper);
        dist=slide_filter(dist,last_dist);

//    mems_fusion(0,0,9.8,0,0,0,
//    &m_node.pos_info);

      //use n_node to store sensor data after slide filter
        m_node.vel_info.xa_vel=xa_vel;
        m_node.vel_info.ya_vel=ya_vel;
        m_node.vel_info.za_vel=za_vel;

     //posture info fusion

        mems_fusion(xa_vel,ya_vel,za_vel,
        xl_accel+sensor_off.gra_cpt_info.gra_x,
        yl_accel+sensor_off.gra_cpt_info.gra_y,
        zl_accel+sensor_off.gra_cpt_info.gra_z,
        &m_node.pos_info);

/*
        mems_fusion(xa_vel,ya_vel,za_vel,xl_accel,yl_accel,zl_accel,
        &m_node.pos_info);
*/

        m_node.accel_info.xl_accel=xl_accel;
        m_node.accel_info.yl_accel=yl_accel;
        m_node.accel_info.zl_accel=zl_accel;

/*
        m_node.accel_info.xl_accel=xl_accel-sensor_off.gra_cpt_info.gra_x;
        m_node.accel_info.yl_accel=yl_accel-sensor_off.gra_cpt_info.gra_y;
        m_node.accel_info.zl_accel=zl_accel-sensor_off.gra_cpt_info.gra_z;
*/
        m_node.temper=temper;
        m_node.dist=dist;


       gettimeofday(&t2, NULL);

       stop = t2.tv_sec*1000000+t2.tv_usec; // 开始时刻
       start = t1.tv_sec*1000000+t1.tv_usec;  // 结束时刻计算距离
       dt=(float)(stop - start)/1000000;

       m_node.sample_time=dt;
//       printf("the start time is %6.5f\n",start);
 //      printf("the stop time is %6.5f\n",stop);

//       printf("the time is %6.5f\n",dt);
     //caculate three axis vel info by accel info
        m_node.vel_info.xl_vel=m_node.accel_info.xl_accel*ST;
        m_node.vel_info.yl_vel=m_node.accel_info.yl_accel*ST;
        m_node.vel_info.zl_vel=m_node.accel_info.zl_accel*ST;

     //use kalman filter to proceed journey info

       kalman_filter(mp,&m_node);

    if(ml_p->count==MAX_NODE_NUM)
        clear_mlist(ml_p);
    mlist_add_node(&m_node,ml_p);
    if(ml_p->count%10==1)
      print_move_info(ml_p->tail_ptr,0);

  }
        pthread_cleanup_pop(0);
}
////////////////////////////////////////////////
