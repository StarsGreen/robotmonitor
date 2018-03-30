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

extern void *move_ctl_thread(void);
extern void *video_get_thread(void);
extern void *video_broadcast_thread(void);

int monitor_err;

sem_t v_get,v_send;

pthread_t m_thread,vget_thread,vsend_thread;

///////////////////////////////////////////
int sig_init()
{
        if(sem_init(&v_get,0,1)>0)
        printf("v_get init error");
        if(sem_init(&v_send,0,0)>0)
        printf("v_send init error");
       // if(sem_init(&server_sock,0,0)>0)
       // printf("server_sock init error");
        return 0;
}
////////////////////////////////////////////
int cancel_monitor_thread()
{
	if(pthread_cancel(m_thread)!=0)
		printf("cancel move thread failed\n");
	else{
		if(pthread_join(m_thread,NULL)==0)
		printf("cancel move thread successfully\n");
		}

	if(pthread_cancel(vget_thread)!=0)
		printf("cancel video get thread failed\n");
	else{
		if(pthread_join(vget_thread,NULL)==0)
		printf("cancel video get thread successfully\n");
		}

	if(pthread_cancel(vsend_thread)!=0)
		printf("cancel video send thread failed");
	else{
		if(pthread_join(vsend_thread,NULL)==0)
		printf("cancel video send thread successfully\n");
		}

	return 0;
}
//////////////////////////////////////////////////
int create_monitor_thread()
{
	monitor_err = pthread_create(&vget_thread, NULL, (void*)video_get_thread, 
	NULL);
        if (monitor_err != 0) {
                fprintf(stderr, "can't create video get thread: %s\n",
                strerror(monitor_err));
                exit(1);
                }

	monitor_err = pthread_create(&vsend_thread, NULL, (void*)video_broadcast_thread,
	 NULL);
        if (monitor_err != 0) {
                fprintf(stderr, "can't create video send thread: %s\n",
                strerror(monitor_err));
                exit(1);
                }

        monitor_err = pthread_create(&m_thread, NULL, (void*)move_ctl_thread, NULL);
        if (monitor_err != 0) {
                fprintf(stderr, "can't create move thread: %s\n",
                strerror(monitor_err));
		exit(1);
			}
	return 0;
}
///////////////////////////////////////////
void signal_monitor_proceed(int signo)
{
if(signo==SIGINT)
	cancel_monitor_thread();
exit(1);
}
//////////////////////////////////////
void monitor_process()
{
	sig_init();
	create_monitor_thread();
	if(signal(SIGINT,signal_monitor_proceed)==SIG_ERR)
		perror("signal error");
	while(1);
}
