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
#include <signal.h>
#include "global_data.h"

extern struct socket_info sock_info; 
extern void read_cmd(char* cmd);
extern sem_t v_get,v_send;
extern void* video_send_thread(void);
extern void* video_broadcast_thread(void);
extern void* info_conm_thread(void);

pthread_t vsend_thread,vget_thread,info_thread;
pid_t socket_fork[QUEUE];
int err;

///////////////////////////////////////////
void cancel_socket_pro_thread()
{

	if(pthread_cancel(info_thread)<0)
		printf("cancel info thread failed");
	else{
		pthread_join(info_thread,NULL);
		printf("cancel info thread successfully\n");
		}
/*	if(pthread_cancel(vsend_thread)<0)
		printf("cancel video send thread failed");
	else{
		pthread_join(vsend_thread,NULL);
		printf("cancel video send thread successfully\n");
		}*/
}
///////////////////////////////////////////
void signal_sockchild_proceed(int signo)
{
if(signo==SIGINT)
	cancel_socket_pro_thread();
exit(1);
}

///////////////////////////////////////////
void signal_socket_proceed(int signo)
{
int i;
if(signo==SIGINT)
	for(i=0;i<sock_info.cli_num;i++)
	kill(socket_fork[i],SIGINT);
exit(1);
}

////////////////////////////////////////////
void handle_request(int conn)
{
	if(signal(SIGINT,signal_sockchild_proceed)==SIG_ERR)
		perror("socket child signal error");
/* err = pthread_create(&vsend_thread, NULL, (void*)video_send_thread, &conn);
        if (err != 0) {
                fprintf(stderr, "can't create video send thread: %s\n",
                strerror(err));
		exit(1);
		}*/ 
err = pthread_create(&info_thread, NULL, (void*)info_conm_thread, &conn);
        if (err != 0) {
                fprintf(stderr, "can't create info conmunication thread: %s\n",
                strerror(err));
		exit(1);
		}
	while(1);
}
////////////////////////////////////////////
int check_ip(char* ip)
{
int status=0;
int i;
int num=sock_info.cli_num;
if(num>0)
	{
	for(i=0;i<num-1;i++)
		if(memcmp(sock_info.cli_info[i].ip,ip,strlen(ip))==0)
		{
		status=1;break;
		}
	}
else 
	{
	memcpy(sock_info.cli_info[num].ip,ip,strlen(ip));
	}
return status;
}
////////////////////////////////////////////
void socket_process(void)
{
	if(signal(SIGINT,signal_socket_proceed)==SIG_ERR)
		perror(" socket signal error");
	sock_info.data_trans_status=0;
	sock_info.sock_con_status=0;
	sock_info.cli_num=0;
	///定义sockfd
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(MYPORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
        perror("bind");
        exit(1);
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,QUEUE) == -1)
    {
        perror("listen");
        exit(1);
    }

    ///客户端套接字
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int i,port;
	char *ip;
	for(i=0;i<QUEUE;i++)
	memset(sock_info.cli_info[i].ip,0,
	sizeof(sock_info.cli_info[i].ip));


	sock_info.data_trans_status=1;
	int conn;
while(1)
	{
	conn=accept(server_sockfd, (struct sockaddr*)&client_addr,&length);
	        if(conn<0)
                	{
                	perror("connect");
			exit(1);
               		 }
		else
			{
			sock_info.sock_con_status=1;
			ip=inet_ntoa(client_addr.sin_addr);
			port=ntohs(client_addr.sin_port);
			check_ip(ip);
			if((socket_fork[sock_info.cli_num]=fork())>0)
				{
			   if(sock_info.cli_num<QUEUE)sock_info.cli_num++;
			   else  sock_info.cli_num=0;
		printf("client %d IP is:%s,port is:%d ",
		sock_info.cli_num,ip,port);
				close(conn);
				}
			else
			handle_request(conn);
                printf("connection successful \n");
			}


	}
}
////////////////////////////////////////////
