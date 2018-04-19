#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <time.h>
#include <pthread.h>
#include "data_structure.h"
#include "data_config.h"
#include "cmd.h"

#define CAMERA_DEVICE "/dev/video0"

#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV

#define BUFFER_COUNT 4

extern struct video_data v_data;

extern sem_t v_get,v_send;
extern Ctrl_Pointer ctrl_cmd;
extern void* get_ctrl_cmd_addr();
/////////////////////////////////////////////
static void vget_cleanup_handler(void *arg)
{
      //  printf("Called clean-up handler\n");
       // cnt = 0;
	if(close(*(int*)arg)==0)
	printf("Camera is closed.\n");
	else
	printf("can not close the camera");
}
//////////////////////////////////////////////
void *video_get_thread()
{
	int i, ret;
//	time_t timep;
//	time (&timep);
//	struct timeval tv;  
  // 打开设备
	int fd;
	fd = open(CAMERA_DEVICE, O_RDWR, 0);
	if (fd < 0) {
		printf("Open %s failed\n", CAMERA_DEVICE);
		goto vget_nothing;
	}

    // 获取驱动信息
	struct v4l2_capability cap;
	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("VIDIOC_QUERYCAP failed (%d)\n", ret);
	}
    // Print capability infomations
/*	printf("Capability Informations:\n");
	printf(" driver: %s\n", cap.driver);
	printf(" card: %s\n", cap.card);
	printf(" bus_info: %s\n", cap.bus_info);
	printf(" version: %08X\n", cap.version);
	printf(" capabilities: %08X\n", cap.capabilities);
*/ 
   // 设置视频格式
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = VIDEO_WIDTH;
	fmt.fmt.pix.height = VIDEO_HEIGHT;
	fmt.fmt.pix.pixelformat =  VIDEO_FORMAT;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
	if (ret < 0) {
		printf("VIDIOC_S_FMT failed (%d)\n", ret);
	}

    // 获取视频格式
	ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
	if (ret < 0) {
		printf("VIDIOC_G_FMT failed (%d)\n", ret);
	}
    // Print Stream Format
/*	printf("Stream Format Informations:\n");
	printf(" type: %d\n", fmt.type);
	printf(" width: %d\n", fmt.fmt.pix.width);
	printf(" height: %d\n", fmt.fmt.pix.height);
*/
	char fmtstr[8];
	memset(fmtstr, 0, 8);
	memcpy(fmtstr, &fmt.fmt.pix.pixelformat, 4);

/*	printf(" pixelformat: %s\n", fmtstr);
	printf(" field: %d\n", fmt.fmt.pix.field);
	printf(" bytesperline: %d\n", fmt.fmt.pix.bytesperline);
	printf(" sizeimage: %d\n", fmt.fmt.pix.sizeimage);
	printf(" colorspace: %d\n", fmt.fmt.pix.colorspace);
	printf(" priv: %d\n", fmt.fmt.pix.priv);
	printf(" raw_date: %s\n", fmt.fmt.raw_data);
*/
    // 请求分配内存
	struct v4l2_requestbuffers reqbuf;
	bzero(&reqbuf,sizeof(reqbuf));
	reqbuf.count = BUFFER_COUNT;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	reqbuf.memory = V4L2_MEMORY_MMAP;

	ret = ioctl(fd , VIDIOC_REQBUFS, &reqbuf);
	if(ret < 0) {
		printf("VIDIOC_REQBUFS failed (%d)\n", ret);
	}

	// 获取空间
	//VideoBuffer*  buffers = calloc( reqbuf.count, sizeof(*buffers) );
	struct v4l2_buffer buf[BUFFER_COUNT];
	int length[BUFFER_COUNT];
	unsigned char *start[BUFFER_COUNT];

	for (i = 0; i < BUFFER_COUNT; i++)
	{
	bzero(&buf[i],sizeof(buf[i]));
        buf[i].index = i;
        buf[i].type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf[i].memory = V4L2_MEMORY_MMAP;
        ret = ioctl(fd , VIDIOC_QUERYBUF, &buf[i]);
        if(ret < 0) {
		printf("VIDIOC_QUERYBUF (%d) failed (%d)\n", i, ret);
	}
	length[i]=buf[i].length;
	start[i]=(unsigned char *) mmap(0, buf[i].length,
	 PROT_READ|PROT_WRITE, MAP_SHARED,
	 fd, buf[i].m.offset);

        // Queen buffer
        ret = ioctl(fd , VIDIOC_QBUF, &buf[i]);
        if (ret < 0) {
            printf("VIDIOC_QBUF (%d) failed (%d)\n", i, ret);
        }

/*       printf("Frame buffer %d: address=0x%x, length=%d\n", i,
	(unsigned int)start[i], buf[i].length);*/
	}
	//开始录制
	enum v4l2_buf_type vtype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_STREAMON, &vtype);
	if (ret < 0) {
		printf("VIDEOC_STREAMON failed (%d)\n", ret);
	}
	struct v4l2_buffer v4l2buf;
	bzero(&v4l2buf,sizeof(v4l2buf));
	v4l2buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	v4l2buf.memory= V4L2_MEMORY_MMAP;

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	pthread_cleanup_push(vget_cleanup_handler, &fd);
	i=0;
	ctrl_cmd=get_ctrl_cmd_addr();
while(1)
    {
	pthread_testcancel();
	sem_wait(&v_get);
//        while(ctrl_cmd->video_get_func==VIDEO_GET_DISABLE)
//                pthread_testcancel();
//	printf("video get start\n");

	v4l2buf.index=i%BUFFER_COUNT;
	ret = ioctl(fd, VIDIOC_DQBUF, &v4l2buf);
	if (ret < 0) {
		printf("VIDIOC_DQBUF failed (%d)\n", ret);
	}
	// Re-queen buffer
	ret = ioctl(fd, VIDIOC_QBUF, &v4l2buf);
	if (ret < 0) {
		printf("VIDIOC_QBUF failed (%d)\n", ret);
	}
	v_data.start_data=start[i];
	v_data.length=length[i];
	i++;
	if(i==BUFFER_COUNT)
	i=0;
//	printf("video get successfully\n");
	sem_post(&v_send);
   }
vget_nothing:
	while(1)pthread_testcancel();
	pthread_cleanup_pop(0);
	return 0;
}
