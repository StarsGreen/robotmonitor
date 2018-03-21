#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "global_data.h"
#include <semaphore.h>
#include "include.h"
#include <semaphore.h>
#include <signal.h>
#include <sys/wait.h>
//int sig_init(); 
int create_all_process();
int cancel_all_process();
void sig_proceed(int signo);
extern int destroy_slist(Sock_Pointer head);
extern int destroy_mlist(M_Pointer head);
extern void init_mlist();
extern void init_slist();
pid_t monitor_pro_pid,socket_pro_pid,cmd_pro_pid,sensor_pro_pid;
int main_err;
//////////////////////////////////////////////
int  main(int argc, char **argv)
{
	init_slist();
	init_mlist();
	create_all_process();
	if(signal(SIGINT,sig_proceed)==SIG_ERR)
		perror("signal error");
	while(1);
	return 0;
}
///////////////////////////////////////////
///////////////////////////////////////////
void sig_proceed(int signo)
{
if(signo==SIGINT)
	cancel_all_process();
destroy_mlist(move_ll.M_Head_pointer);
destroy_slist(sock_ll.S_Head_pointer);
exit(1);
}
///////////////////////////////////////////
int cancel_all_process()
{
	int status;
	if(kill(monitor_pro_pid,SIGINT)<0)
		printf("cancel monitor process failed");
	else{
		main_err=waitpid(monitor_pro_pid,&status,WUNTRACED|WCONTINUED);
		if(main_err>0)
		printf("cancel monitor process:%d successfully\n",main_err);
		else
		printf("cancel monitor process failed\n");
		}

	if(kill(cmd_pro_pid,SIGINT)<0)
		printf("cancel cmd process failed");
	else{
		main_err=waitpid(cmd_pro_pid,&status,WUNTRACED|WCONTINUED);
		if(main_err>0)
		printf("cancel cmd process:%d successfully\n",main_err);
		else
		printf("cancel cmd process failed\n");
		}

	if(kill(socket_pro_pid,SIGINT)<0)
		printf("cancel socket process failed");
	else{
		main_err=waitpid(socket_pro_pid,&status,WUNTRACED|WCONTINUED);
		if(main_err>0)
		printf("cancel socket process:%d successfully\n",main_err);
		else
		printf("cancel socket process failed\n");
		}

	if(kill(sensor_pro_pid,SIGINT)<0)
		printf("cancel sensor process failed");
	else{
		main_err=waitpid(sensor_pro_pid,&status,WUNTRACED|WCONTINUED);
		if(main_err>0)
		printf("cancel sensor process:%d successfully\n",main_err);
		else
		printf("cancel sensor process failed\n");
		}
	return 0;
}
/////////////////////////////////////////////
int create_all_process()
{
	monitor_pro_pid=fork();
	switch(monitor_pro_pid)
	{
	case -1:
	printf("monitor process create error!\n");break;
	case 0:
	monitor_process();break;
	default:
	printf("monitor process pid is:%d \n",monitor_pro_pid);break;
	}

	cmd_pro_pid=fork();
	switch(cmd_pro_pid)
	{
	case -1:
	printf("cmd process create error!\n");break;
	case 0:
	cmd_process();break;
	default:
	printf("cmd process pid is: %d \n",cmd_pro_pid);break;
	}

	socket_pro_pid=fork();
	switch(socket_pro_pid)
	{
	case -1:
	printf("socket process create error!\n");break;
	case 0:
	socket_process();break;
	default:
	printf("socket process pid is: %d \n",socket_pro_pid);break;
	}

	sensor_pro_pid=fork();
	switch(sensor_pro_pid)
	{
	case -1:
	printf("sensor process create error!\n");break;
	case 0:
	sensor_process();break;
	default:
	printf("sensor process pid is:%d \n",sensor_pro_pid);break;
	}
	return 0;
}
