//defined some datastructure to store global data
#include<netdb.h>
#include<data_structure.h>
#include<pthread.h>
#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H_
#endif

#define MYPORT  50000
#define QUEUE   4
#define BUFFER_SIZE 1024
#define ARRAY_SIZE 1024

#define    MAXLINE        1024*1024
#define    PORT         51000
#define    BROADCAST_IP "224.0.0.100"
#define    AIMED_IP     "192.168.1.12"
#define    BACKLOG      4
#define    FILENAME	"video.jpg"

#define	SMPLRT_DIV	0x19
//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG		0x1A
//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG	0xe0
//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0xe1
//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_RANGE	2*9.8

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define GYRO_RANGE	250
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)
//IIC写入时的地址字节数据，+1为读取
#define   DEVIIC_ID	0x68

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define QUALITY	80


#define M_NODE_SIZE sizeof(M_Node)//motion structure size
#define S_NODE_SIZE sizeof(Sock_Node)//motion structure size
#define MAX_NODE_NUM 200

/////////////////////////////
struct move_cmd m_cmd;
///////////////////////////////////
struct M_LinkList move_ll;
M_Node M_info;
M_Pointer M_info_pointer;
/////////////////////////////////////////
//////////////////////////////////////////////
Sock_Node S_info;
Sock_Pointer S_info_pointer;
struct S_LinkList sock_ll;
//////////////////////////////////////////////
S_Params s_params;
///////////////////////////////////////////////
struct socket_info sock_info;
///////////////////////////////////////////////
struct udp_flag u_flag;
struct video_data v_data;
/////////////////////////////////////////////
