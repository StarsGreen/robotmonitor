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
#include <signal.h>


extern void *temper_get_thread(void);
extern void *dist_get_thread(void);
extern void *accel_get_thread(void);


int sensor_err;
pthread_t temper_thread,accel_thread,dist_thread;
sem_t sensor_start,sensor_mid,sensor_stop;
////////////////////////////////////////////
int cancel_sensor_thread()
{
	if(pthread_cancel(temper_thread)<0)
		printf("cancel move thread failed");
	else{
		pthread_join(temper_thread,NULL);
		printf("cancel move thread successfully\n");
		}
	if(pthread_cancel(accel_thread)<0)
		printf("cancel video get thread failed");
	else{
		pthread_join(accel_thread,NULL);
		printf("cancel video get thread successfully\n");
		}
	if(pthread_cancel(dist_thread)<0)
		printf("cancel video send thread failed");
	else{
		pthread_join(dist_thread,NULL);
		printf("cancel video send thread successfully\n");
		}
	return 0;
}
//////////////////////////////////////////////////
int create_sensor_thread()
{
	sensor_err = pthread_create(&accel_thread, NULL, (void*)accel_get_thread, 
	NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create video get thread: %s\n",
                strerror(sensor_err));
                exit(1);
                }
	sensor_err = pthread_create(&dist_thread, NULL, (void*)temper_get_thread,
	 NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create video send thread: %s\n",
                strerror(sensor_err));
                exit(1);
                }
        sensor_err = pthread_create(&temper_thread, NULL, (void*)dist_get_thread, 
	NULL);
        if (sensor_err != 0) {
                fprintf(stderr, "can't create move thread: %s\n",
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
exit(1);
}
//////////////////////////////////////
void sensor_process()
{
	spro_sem_init();
	create_sensor_thread();
	if(signal(SIGINT,signal_sensor_proceed)==SIG_ERR)
		perror("signal error");
	while(1);
}
