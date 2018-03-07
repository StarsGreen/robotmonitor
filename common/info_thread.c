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

extern void read_cmd(char* cmd);
extern struct move_info m_info;
/////////////////////////////////////////
void recv_info_thread(int conn)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	char buffer[ARRAY_SIZE];
//	int optval;
//	socklen_t optlen = sizeof(int);
	while(1)
	{
		pthread_testcancel();
		memset(buffer,0,sizeof(buffer));
		int len = recv(conn, buffer, sizeof(buffer),0);
		if(len>0)
			{
			if(strcmp(buffer,"exit")==0)
				{
				close(conn);
				break;
				}
			else
				{
				read_cmd(buffer);
				}
			}
		else 
			{
			close(conn);
			break;
			}
	}
		printf("this thread is closed");
}
//////////////////////////////////////////////
int recong_info()
{




}
///////////////////////////////////////////////
char* compile_info()
{
char info[20];
info[0]='a';
info[1]=m_info.accel;
info[4]='v';
info[3]=m_info.vel;
info[8]='s';
info[5]=m_info.journey;
info[12]='t';
info[7]=m_info.temper;
info[16]='d';
info[9]=m_info.dist;
return info;
}
//////////////////////////////////////////////
void send_info_thread(int conn)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	while(1)
	{
		pthread_testcancel();





	]









}
