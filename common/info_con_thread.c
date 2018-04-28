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
#include <sys/ipc.h>
#include "data_structure.h"
#include "data_config.h"
#include "cmd.h"
#include <signal.h>
#include <math.h>
extern void read_cmd(char* cmd);
//extern struct move_info M_info;
extern void slist_delete(char* ip);
extern void* get_ll_shmid(key_t key,int size);
extern void* get_ctrl_cmd_addr();
Ctrl_Pointer cp=NULL;
/////////////////////////////////////////
static void sock_cleanup_handler(void *arg)
{
	shmdt(cp);
      //  printf("Called clean-up handler\n");
      // cnt = 0;
/*        if(close(*(int*)arg)==0)
        printf("socket is closed.\n");
        else
        printf("can not close the socket\n");
*/
}
//////////////////////////////////////////////
int recong_info(int a)
{
int flag=0;
if(a>-20000&&a<=-10000)flag=10;
if(a>-10000&&a<=-1000)flag=9;
if(a>-1000&&a<=-100)flag=8;
if(a>-100&&a<=-10)flag=7;
if(a>-10&&a<0)flag=6;
if(a>=0&&a<10)flag=1;
if(a>=10&&a<100)flag=2;
if(a>=100&&a<1000)flag=3;
if(a>=1000&&a<10000)flag=4;
if(a>=10000&&a<20000)flag=5;
return flag;
}
//////////////////////////////////////////////
char* code_info(int value)
{
static char code_msg[6];
switch(recong_info(value))
{
	case 1:
	code_msg[0]='+';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]='0';
	code_msg[4]='0';
	code_msg[5]=value+'0';break;
	case 2:
	code_msg[0]='+';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]='0';
	code_msg[4]=value/10+'0';
	code_msg[5]=value%10+'0';break;

	case 3:
	code_msg[0]='+';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]=value/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;
	case 4:
	code_msg[0]='+';
	code_msg[1]='0';
	code_msg[2]=value/1000+'0';
	code_msg[3]=value%1000/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;
	case 5:
	code_msg[0]='+';
	code_msg[1]=value/10000+'0';
	code_msg[2]=value%10000/1000+'0';
	code_msg[3]=value%1000/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;

	case 6:
	value=value*(-1);
	code_msg[0]='-';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]='0';
	code_msg[4]='0';
	code_msg[5]=value+'0';break;
	case 7:
	value=value*(-1);
	code_msg[0]='-';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]='0';
	code_msg[4]=value/10+'0';
	code_msg[5]=value%10+'0';break;

	case 8:
	value=value*(-1);
	code_msg[0]='-';
	code_msg[1]='0';
	code_msg[2]='0';
	code_msg[3]=value/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;
	case 9:
	value=value*(-1);
	code_msg[0]='-';
	code_msg[1]='0';
	code_msg[2]=value/1000+'0';
	code_msg[3]=value%1000/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;

	case 10:
	value=value*(-1);
	code_msg[0]='-';
	code_msg[1]=value/10000+'0';
	code_msg[2]=value%10000/1000+'0';
	code_msg[3]=value%1000/100+'0';
	code_msg[4]=value%100/10+'0';
	code_msg[5]=value%10+'0';break;
	default:break;
}
return code_msg;
}
//////////////////////////////////////////////
char* assemble_info(void)
{
	static char info[43];

	M_Pointer p=(M_Pointer)malloc(M_NODE_SIZE);
 	mll_ptr gp=get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=(M_Pointer)shmat(gp->Tail_shmid,NULL,0);
	memcpy(p,tail,M_NODE_SIZE);

	int value=(int)(sqrt((p->accel_info.xl_accel)*(p->accel_info.xl_accel)+
	(p->accel_info.yl_accel)*(p->accel_info.yl_accel)+
	(p->accel_info.zl_accel)*(p->accel_info.zl_accel))*100);
	info[0]='a';
	memcpy(&info[1],code_info(value),6);
////////////////////////////////////////////
	value=(int)(sqrt((p->vel_info.xl_vel)*(p->vel_info.xl_vel)+
	(p->vel_info.yl_vel)*(p->vel_info.yl_vel)+
	(p->vel_info.zl_vel)*(p->vel_info.zl_vel))*100);
	info[7]='v';
	memcpy(&info[8],code_info(value),6);
///////////////////////////////////////////////
	value=(int)(p->jour_info.xl*10);
	info[14]='x';
	memcpy(&info[15],code_info(value),6);
///////////////////////////////////////////////
	value=(int)(p->jour_info.yl*10);
	info[21]='y';
	memcpy(&info[22],code_info(value),6);
//////////////////////////////////////////
	value=(int)(p->temper*10);
	info[28]='t';
	memcpy(&info[29],code_info(value),6);
//////////////////////////////////////////
	value=(int)(p->dist*10);
	info[35]='d';
	memcpy(&info[36],code_info(value),6);
///////////////////////////////////////////
	info[42]='\n';
//	memcpy(pointer,info,42);
//	return info;
//	printf("3\n");
	shmdt(gp);
	shmdt(tail);
	free(p);
	return info;
}

//////////////////////////////////////////////

/////////////////////////////////////////
void* info_recv_thread(void* s)
{
//	printf("one break \n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	char buffer[ARRAY_SIZE];
	int conn=((S_Params*)s)->conn;
	char* ip=((S_Params*)s)->ip;
//	Ctrl_Pointer cp=get_ctrl_cmd_addr();
//	int conn=*(int*)s;
        pthread_cleanup_push(sock_cleanup_handler, &conn);
//	int optval;
//	socklen_t optlen = sizeof(int);
	while(1)
	{
//		printf("recv info thread\n");
		pthread_testcancel();
		memset(buffer,0,sizeof(buffer));
//                while(cp->info_send_func==INFO_SEND_DISABLE)
		int len = recv(conn, buffer, sizeof(buffer),0);
		printf("the recv msg is:%s",buffer);
		if(len>0)
			{
			if(strcmp(buffer,"exit")==0)
				{
				close(conn);
				slist_delete(ip);
				raise(SIGINT);
				break;
				}
			else
				{
				printf("the recv msg is:%s",buffer);
				read_cmd(buffer);
				}
			}
		else
			{
			close(conn);
			slist_delete(ip);
			raise(SIGINT);
			break;
			}
	}
	        pthread_cleanup_pop(0);
//		printf("recv info thread is closed");
}
//////////////////////////////////////////////
void* info_send_thread(void* s)
{
//	printf("two break\n");
	char* msg_buf=NULL;
//	S_Params* sp=s;
	int conn=((S_Params*)s)->conn;
	char* ip=((S_Params*)s)->ip;
//	int conn=*(int*)s;
	int send_flag=0;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
        pthread_cleanup_push(sock_cleanup_handler,&conn);
	while(1)
	{
//		printf("send info thread\n");
		pthread_testcancel();
//		msg_buf="ok";
		msg_buf=assemble_info();
		sleep(1);
//                while(cp->info_send_func==INFO_SEND_DISABLE)
//                pthread_testcancel();
//		printf("msg_buf is: %s\n",msg_buf);
		if(msg_buf!=NULL)
			send_flag=send(conn,msg_buf,strlen(msg_buf), 0);
//		printf("send_flag is:%d\n",send_flag);
		if(send_flag==-1)
		{
                        close(conn);
                        slist_delete(ip);
		//	free(ip);
                        raise(SIGINT);
		}
//		if(msg_buf!=NULL)
//			free(msg_buf);
	}
        pthread_cleanup_pop(0);
}



