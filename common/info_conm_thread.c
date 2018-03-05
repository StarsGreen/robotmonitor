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

/////////////////////////////////////////
void info_conm_thread(int conn)
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
				kill()
				pthread_exit();
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
