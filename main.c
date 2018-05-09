#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "data_structure.h"
#include "data_config.h"
//#include "global_data.h"
//#include "cmd.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "include.h"
#include <semaphore.h>
#include <signal.h>
#include <sys/wait.h>
#include<wiringPi.h>
//int sig_init(); 
int create_all_process();
int cancel_all_process();
void sig_proceed(int signo);
void sys_init();
void init_syslock();
void init_share_mem();
extern int destroy_slist(void);
extern int destroy_mlist(void);
extern int init_mlist();
extern int init_slist();
extern void* get_ll_shmid(key_t key,int size);
extern void *get_move_cmd_addr();
extern void* get_ctrl_cmd_addr();
extern void* move_direct_stop(void);
pid_t monitor_pro_pid,socket_pro_pid,cmd_pro_pid,sensor_pro_pid;
int main_err;
//////////////////////////////////////////////
int  main(int argc, char **argv)
{
	sys_init();
	create_all_process();
	if(signal(SIGINT,sig_proceed)==SIG_ERR)
		perror("signal error");
	while(1);
	return 0;
}
///////////////////////////////////////////
void sys_init()
{
	init_slist();
	init_mlist();
	init_share_mem();
	init_syslock();
	wiringPiSetup();
}
//////////////////////////////////////////
void init_syslock()
{
struct M_LinkList* m_gp=(struct M_LinkList*)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
pthread_mutex_init(&m_gp->move_ll_lock,NULL);
struct S_LinkList* s_gp=(struct S_LinkList*)get_ll_shmid(SOCK_LL_KEY,S_NODE_SIZE);
pthread_mutex_init(&s_gp->sock_ll_lock,NULL);
move_cmd* m_cmd=get_move_cmd_addr();
pthread_mutex_init(&m_cmd->lock,NULL);
shmdt(m_gp);
shmdt(s_gp);
shmdt(m_cmd);
}
////////////////////////////////////////////
void init_share_mem()
{
shmdt(get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE));
shmdt(get_ll_shmid(SOCK_LL_KEY,S_NODE_SIZE));
move_cmd* m_cmd=get_move_cmd_addr();
memset(m_cmd,0,sizeof(move_cmd));
shmdt(m_cmd);
Ctrl_Pointer c_ptr=get_ctrl_cmd_addr();
memset(c_ptr,0,sizeof(Ctrl_Cmd));
shmdt(c_ptr);
}
///////////////////////////////////////////
void sig_proceed(int signo)
{
if(signo==SIGINT)
	cancel_all_process();
destroy_mlist();
destroy_slist();
struct M_LinkList* m_gp=get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
struct S_LinkList* s_gp=get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
move_cmd* m_cmd=get_move_cmd_addr();
pthread_mutex_destroy(&m_gp->move_ll_lock);
pthread_mutex_destroy(&s_gp->sock_ll_lock);
pthread_mutex_destroy(&m_cmd->lock);
shmctl(shmget(MOVE_LL_KEY,MOVE_LL_SIZE,IPC_CREAT|0666),IPC_RMID, NULL);
shmctl(shmget(SOCK_LL_KEY,SOCK_LL_SIZE,IPC_CREAT|0666), IPC_RMID, NULL); 
shmctl(shmget(MOVE_CMD_KEY,MOVE_CMD_SIZE,IPC_CREAT|0666),IPC_RMID,NULL);
shmctl(shmget(CTRL_CMD_KEY,CTRL_CMD_SIZE,IPC_CREAT|0666),IPC_RMID,NULL);
move_direct_stop();
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
	return 0;
}
