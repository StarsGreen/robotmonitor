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
//#include "global_data.h"
#include <signal.h>


extern void *temper_get_thread(void);
extern void *dist_get_thread(void);
extern void *accel_get_thread(void);
extern void *collect_info_thread(void);
//extern void init_interface_pin(void);
//extern void *sensor_data_get_thread(void);

int sensor_err;
pthread_t temper_thread,accel_thread,dist_thread,collect_thread;
sem_t sensor_start,sensor_mid,sensor_stop;
////////////////////////////////////////////
int cancel_sensor_thread()
{
	if(pthread_cancel(temper_thread)!=0)
		printf("cancel temper thread failed!!!!!!\n");
	else{
		if(pthread_join(temper_thread,NULL)==0)
		printf("cancel temper thread successfully\n");
		}

	if(pthread_cancel(accel_thread)!=0)
		printf("cancel accel get thread failed!!!!!!\n");
	else{
		if(pthread_join(accel_thread,NULL)==0);
		printf("cancel accel get thread successfully\n");
		}

	if(pthread_cancel(dist_thread)!=0)
		printf("cancel dist get thread failed!!!!!!\n");
	else{
		if(pthread_join(dist_thread,NULL)==0);
		printf("cancel dist get thread successfully\n");
		}

	if(pthread_cancel(collect_thread)!=0)
		printf("cancel collect thread failed!!!!!!\n");
	else{
		if(pthread_join(collect_thread,NULL)==0)
		printf("cancel collect_thread successfully\n");
		}

	return 0;
}
//////////////////////////////////////////////////
int create_sensor_thread()
{

	sensor_err = pthread_create(&accel_thread, NULL, (void*)accel_get_thread, 
	NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create accel get thread: %s\n",
                strerror(sensor_err));
                exit(1);
                }

	sensor_err = pthread_create(&temper_thread, NULL, (void*)temper_get_thread,
	 NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create temper get thread: %s\n",
                strerror(sensor_err));
                exit(1);
                }

	sensor_err = pthread_create(&dist_thread, NULL,
		(void*)dist_get_thread,NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create dist get thread: %s\n",
                strerror(sensor_err));
		exit(1);
			}

	sensor_err = pthread_create(&collect_thread, NULL,
		(void*)collect_info_thread,NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create collect info thread: %s\n",
                strerror(sensor_err));
		exit(1);
			}

	return 0;
}
///////////////////////////////////////////
int spro_sem_init()
{
        if(sem_init(&sensor_start,0,1)>0)
        printf("sensor_start init error");
        if(sem_init(&sensor_mid,0,0)>0)
        printf("sensor_mid init error");
        if(sem_init(&sensor_stop,0,0)>0)
        printf("server_sock init error");
        return 0;
}
///////////////////////////////////////////
void signal_sensor_proceed(int signo)
{
if(signo==SIGINT)
  cancel_sensor_thread();
extern ml_ptr ml_p;
destroy_mlist(ml_p);
exit(1);
}
//////////////////////////////////////
void sensor_process()
{
	spro_sem_init();
//	init_interface_pin();
	create_sensor_thread();
	if(signal(SIGINT,signal_sensor_proceed)==SIG_ERR)
		perror("signal error");
	while(1);
}
