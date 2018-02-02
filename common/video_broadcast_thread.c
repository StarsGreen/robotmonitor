#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <semaphore.h>
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <pthread.h>
#include <sys/time.h>
#include "global_data.h"


extern struct video_data v_data;
extern struct udp_flag u_flag;
extern sem_t v_get,v_send;

extern int read_file_to_buff(char* filename,unsigned char* buff);
extern int yuyv_to_jpeg(unsigned char* yuv422,unsigned char** jpeg_buff,
unsigned long* jpeg_size,int quality,int width, int height);
////////////////////////////////////////////
int get_len(unsigned char* buff,int len)
{
int num=0;
int i;
int jump=0;
for(i=0;i<len;i++)
{
	if(*(buff+i)!=0)
	num++;
	else{
		if((i<len-2)&&(*(buff+i+1)==0)&&(*(buff+i+2)==0))
		jump=1;
		else num++;
		}
	if(jump==1)break;
}
	return num;
}
//////////////////////////////////////////
void* video_broadcast_thread(void)
{
	struct sockaddr_in client_addr;
//	char msg[BUFFER_SIZE];
	int ss,ret,opt=1;
	int j;
   /* create the socket commanted by greenstar*/  
	if ((ss = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {  
	perror("Create UDPclient failed");  
	exit(0);  
	}
	
	memset(&client_addr,0,sizeof(client_addr));  
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
//	client_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
//	client_addr.sin_addr.s_addr = inet_addr(AIMED_IP);
	client_addr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

	socklen_t addr_len=sizeof(client_addr);

	ret = setsockopt(ss, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));  
	if(ret == -1)
		{  
		printf("set udp broadcast error...\n");  
		exit(0);  
		}
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
while(1)
{		
	pthread_testcancel();	
unsigned char* jpeg_buff=(unsigned char*)malloc(VIDEO_WIDTH*VIDEO_HEIGHT);
	unsigned long jpeg_size=VIDEO_WIDTH*VIDEO_HEIGHT;
	
	yuyv_to_jpeg(v_data.start_data,&jpeg_buff,&jpeg_size,
	QUALITY,VIDEO_WIDTH,VIDEO_HEIGHT);	

//	file_len=read_file_to_buff(FILENAME,video_buff);
		
//	printf("the video pic is %d bytes\n",file_len);

	sem_wait(&v_send);


sendto(ss,jpeg_buff,jpeg_size,0,(struct sockaddr*)&client_addr,addr_len);


	sem_post(&v_get);
//	printf("send over\n");
}
	close(ss);
	printf("Send finish\n");  
	return 0;  
}
//////////////////////////////////////////////
