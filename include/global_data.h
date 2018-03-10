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


#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define QUALITY	80

struct move_cmd
{
	int cmd_type;
	int angle;
	int vel;
};
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
struct volecity
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
	M_Node* next;
	M_Node* prev;
}M_Node;
typedef struct M_Node* M_Pointer;
M_pointer Head_Pointer,Tail_Pointer;
/////////////////////////////////////////
struct move_info
{
	struct accel accel_info;
	struct velocity vel_info;
	struct journey jour_info;
	float temper;
	float dist;
};
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
};

struct udp_flag
{
	unsigned char *aimed_ip;
	int port;
        int video_send_status;
};
struct video_data
{
        unsigned char *start_data;
        int length;
	int send_status;
};
