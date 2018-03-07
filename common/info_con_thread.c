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
int recong_info(int value)
{
int status=0;
switch(value)
	{
	case 0...9:status=1;break;
	case 10...99:status=2;break;
	case 100...999:status=3;break;
	default:break;
	}
return status;
}
///////////////////////////////////////////////
char* compile_info()
{
char info[20];
info[0]='a';
switch(recong_info(m_info.accel))
{
	case 1:info[1]='0';info[2]='0';info[3]=m_info.accel+'0';break;
	case 2:info[1]='0';info[2]='0'+m_info.accel/10;
		info[3]=m_info.accel%10+'0';break;
	case 3:info[1]='0'+m_info/100;info[2]='0'+(m_info.accel%100)/10;
		info[3]=m_info.accel%10+'0';break;
	default:break;
}
info[4]='v';
switch(recong_info(m_info.vel))
{
	case 1:info[5]='0';info[6]='0';info[7]=m_info.vel+'0';break;
	case 2:info[5]='0';info[6]='0'+m_info.vel/10;
		info[7]=m_info.vel%10+'0';break;
	case 3:info[5]='0'+m_info/100;info[6]='0'+(m_info.vel%100)/10;
		info[7]=m_info.vel%10+'0';break;
	default:break;
}
info[8]='s';
switch(recong_info(m_info.journey))
{
	case 1:info[9]='0';info[10]='0';info[11]=m_info.journey+'0';break;
	case 2:info[9]='0';info[10]='0'+m_info.journey/10;
		info[11]=m_info.journey%10+'0';break;
	case 3:info[9]='0'+m_info/100;info[10]='0'+(m_info.journey%100)/10;
		info[11]=m_info.journey%10+'0';break;
	default:break;
}
info[12]='t';
switch(recong_info(m_info.temper))
{
	case 1:info[13]='0';info[14]='0';info[15]=m_info.temper+'0';break;
	case 2:info[13]='0';info[14]='0'+m_info.temper/10;
		info[15]=m_info.temper%10+'0';break;
	case 3:info[13]='0'+m_info/100;info[14]='0'+(m_info.temper%100)/10;
		info[15]=m_info.temper%10+'0';break;
	default:break;
}
info[16]='d';
switch(recong_info(m_info.dist))
{
	case 1:info[17]='0';info[18]='0';info[19]=m_info.dist+'0';break;
	case 2:info[17]='0';info[18]='0'+m_info.dist/10;
		info[19]=m_info.dist%10+'0';break;
	case 3:info[17]='0'+m_info/100;info[18]='0'+(m_info.dist%100)/10;
		info[19]=m_info.dist%10+'0';break;
	default:break;
}
return info;
}
//////////////////////////////////////////////
void send_info_thread(int conn)
{
	char* buf;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	while(1)
	{
		pthread_testcancel();
		buf=compile_info();
		send(conn, buf, strlen(buf), 0); 
	}
}
