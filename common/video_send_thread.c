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
#include "data_refer.h"



extern sem_t v_get,v_send;

extern int read_file_to_buff(unsigned char* filename,char* buff);
extern int yuyv_to_jpeg(unsigned char* yuv422,int quality,
int width, int height);


void* video_send_thread(int conn)
{
//	char msg[BUFFER_SIZE];
	int send_len,file_len;
	int j;
	char end[3]={"end"};
	char start[5]={"start"};

	unsigned char *filebuff;
	v_data.send_status=0;


	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);

while(1)
{
	pthread_testcancel();
//	while(v_data.send_status==0)
//	pthread_testcancel();
	sem_wait(&v_send);

//	yuyv_to_jpeg(v_data.start_data,80,320,240);
	
//	file_len=read_file_to_buff("video.jpg",file_buff);

	write(conn,start,5);

/*	for(j=0;j<file_len/1024+1;j++)
		{
		if(j==file_len/1024){
        		if((file_len-j*1024)!=0)
				send_len=write(conn, file_buff+j*1024,
	file_len%1024); 
			else	
				send_len=write(conn, file_buff+j*1024,
	file_len%1024); 
		else	
				send_len=write(conn, file_buff+j*1024,
	1024); 
		if(send_len<0)printf("send video data error");
		}

*/
//	printf("send start"); 
/*	for(j=0;j<file_len/1024+1;j++)
		{
		if(j==file_len/1024){
        		if((file_len-j*1024)!=0)
				send_len=write(conn, file_buff+j*1024,
	file_len%1024); 
			else	
				send_len=write(conn, file_buff+j*1024,
	file_len%1024); 
		else	
				send_len=write(conn, file_buff+j*1024,
	1024); 
		if(send_len<0)printf("send video data error");
		}*/
	write(conn,end,3);
	sem_post(&v_get);
//	printf("send over");
}
	close(conn);
	printf("Send finish\n");  
	return 0;  
}
