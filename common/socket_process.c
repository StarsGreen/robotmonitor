#include <sys/types.h>
#include <sys/wait.h>
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
#include <sys/ipc.h>
#include <signal.h>
#include "data_structure.h"
#include "data_config.h"
//#include "data_structure.h"

//extern struct socket_info sock_info; 
extern void read_cmd(char* cmd);
//extern int slist_add(Sock_Node sock_node);
//extern int init_slist(void);
//extern void* slist_search_ip(void* ip);
//extern void* get_ll_shmid(key_t key,int size);
//extern sem_t v_get,v_send;
//extern void* video_send_thread(void);
extern void* info_send_thread(void* s);
extern void* info_recv_thread(void* s);
//extern void* video_broadcast_thread(void);
//extern void* info_conm_thread(void);
S_Params s_params;
pthread_t send_info,recv_info;
pid_t socket_fork[QUEUE];
static int num=0;
int sock_err;
void nothing(void)
{
}
///////////////////////////////////////////
void cancel_socket_pro_thread()
{
	if(pthread_cancel(send_info)!=0)
		printf("cancel msg send thread failed\n");
	else{
		if(pthread_join(send_info,NULL)==0)
		printf("cancel msg send thread successfully\n");
		}
	if(pthread_cancel(recv_info)!=0)
		printf("cancel msg recv thread failed\n");
	else{
		if(pthread_join(recv_info,NULL)==0);
		printf("cancel msg recv thread successfully\n");
		}
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
	for(i=0;i<num;i++)
	{
	kill(socket_fork[i],SIGINT);
	waitpid(socket_fork[i],NULL,0);
	}
exit(1);
}

////////////////////////////////////////////
/*
int check_ip(char* ip)
{
int status=0;
if(slist_search_ip(ip)!=NULL)status=1;
return status;
}
*/
////////////////////////////////////////////
///////////////////////////////////////////
/*
void sock_add(char* ip,int port)
{
Sock_Node S_info;
memset(&S_info,0,S_NODE_SIZE);
memcpy(S_info.cli_info.ip,ip,15);
S_info.cli_info.port=port;
slist_add(S_info);
}*/
//////////////////////////////////////////
void handle_request(int conn,char* ip)
{
	if(signal(SIGINT,signal_sockchild_proceed)==SIG_ERR)
		perror("socket child signal error");
	memcpy(s_params.ip,ip,15);
	s_params.conn=conn;
        printf("connection successful \n");
sock_err = pthread_create(&send_info, NULL,info_send_thread, &s_params);
        if (sock_err != 0) {
                printf("can't create info send thread\n");
		exit(1);
			}
	printf("create info send thread,%d\n",sock_err);
sock_err = pthread_create(&recv_info, NULL,info_recv_thread,&s_params);
        if (sock_err != 0) {
                printf("can't create info recv thread\n");
		exit(1);
		}
	 printf("create info recv thread,%d\n",sock_err);

/* sock_err = pthread_create(&recv_info, NULL, (void*)recv_info_thread, &conn);
        if (sock_err != 0) {
                fprintf(stderr, "can't create info recv thread: %s\n",
                strerror(sock_err));
		exit(1);
		}
*/
	while(1);
}
///////////////////////////////////////////
static void handle_connect(int s_s)
{
	printf("\nhandle connect\n");
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int port;
	char *ip;
//	int cli_num=0;
//	sock_info.data_trans_status=1;
	int conn;
//	sll_ptr sll_p=get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
while(1)
	{
	printf("\nthe server is open\n");
	conn=accept(s_s, (struct sockaddr*)&client_addr,&length);
	        if(conn<0)
                	{
                	perror("connect error");
			exit(1);
               		 }
		else
			{
//			sock_info.sock_con_status=1;
			ip=inet_ntoa(client_addr.sin_addr);
			port=ntohs(client_addr.sin_port);
//			if(check_ip(ip)==1)goto sock_nothing;
			if((socket_fork[num++]=fork())>0)
				{
				close(conn);
//			   if(num<=QUEUE)sock_add(ip,port);
//			   else printf("the conn is full");
	printf("\nclient %d IP is:%s,port is:%d  is connected\n",num,ip,port);
				}
			else
				{
			handle_request(conn,ip);
				}
//		printf("client one");
			}
sock_nothing:nothing();
	}
}
////////////////////////////////////////////
void socket_process(void)
{
	if(signal(SIGINT,signal_socket_proceed)==SIG_ERR)
		perror(" socket signal error");
//	sock_info.data_trans_status=0;
//	sock_info.sock_con_status=0;
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

    //客户端套接字
/*	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int i,port;
	char *ip;
	int cli_num=0;
	sock_info.data_trans_status=1;
	int conn;
*/
	handle_connect(server_sockfd);
	close(server_sockfd);
	while(1);
}
////////////////////////////////////////////
