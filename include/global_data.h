//defined some datastructure to store global data

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

#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
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
#define   IICDEV_ID	0b1010000

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define QUALITY	80

struct move_cmd
{
	int cmd_type;
	int angle;
	int vel;
}m_cmd;
///////////////////////////////////
struct accel
{
	float xa_accel;
	float ya_accel;
	float za_accel;
	float xl_accel;
	float yl_accel;
	float zl_accel;
}accel_info;
struct velocity
{
	float xa_vel;
	float ya_vel;
	float za_vel;

	float xl_vel;
	float yl_vel;
	float zl_vel;
}vel_info;

struct journey
{
	float xa;
	float ya;
	float za;

	float xl;
	float yl;
	float zl;
}jour_info;
typedef struct M_Node
{
	struct accel accel_info;
	struct velocity vel_info;
	struct journey jou_info;
	int count;
	struct M_Node* next;
	struct M_Node* prev;
}M_Node;
typedef struct M_Node* M_Pointer;
M_Pointer Head_Pointer,Tail_Pointer;
M_Node M_node;
/////////////////////////////////////////
struct move_info
{
	struct accel accel_info;
	struct velocity vel_info;
	struct journey jour_info;
	float temper;
	float dist;
}m_info;
//////////////////////////////////////////////
struct client_info
{
	char ip[15];
	int port;
};

struct socket_info
{
	struct client_info cli_info[QUEUE];
	int cli_num;
        int sock_con_status;
	int data_trans_status;
}sock_info;

struct udp_flag
{
	unsigned char *aimed_ip;
	int port;
        int video_send_status;
}u_flag;
struct video_data
{
        unsigned char *start_data;
        int length;
	int send_status;
}v_data;
