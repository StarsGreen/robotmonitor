//defined some datastructure to store global data
#include<netdb.h>
#include<pthread.h>
#ifndef __DATA_STRUCTURE_H__
#define __DATA_STRUCTURE_H__
#endif

#define M_NODE_SIZE sizeof(M_Node)//motion structure size
#define S_NODE_SIZE sizeof(Sock_Node)//motion structure size
#define MAX_NODE_NUM 200

/////////////////////////////
struct move_cmd
{
	int cmd_type;
	int angle;
	int vel;
};
///////////////////////////////////
struct accel
{
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
	struct journey jour_info;
	float temper;
	float dist;
	int num;
	struct M_Node* next;
	struct M_Node* prev;
}M_Node;
typedef struct M_Node* M_Pointer;
struct M_LinkList
{
M_Pointer M_Head_pointer;
M_Pointer M_Tail_pointer;
int count;
pthread_mutex_t move_ll_lock;
};
//////////////////////////////////////////////
struct client_info
{
	char ip[15];
	int port;
};
typedef struct Sock_Node
{
	struct client_info cli_info;
	int cli_num;
	struct Sock_Node* next;
	struct Sock_Node* prev;
}Sock_Node;
typedef struct Sock_Node* Sock_Pointer;
struct S_LinkList
{
Sock_Pointer S_Head_pointer;
Sock_Pointer S_Tail_pointer;
int count;
pthread_mutex_t sock_ll_lock;
};
//////////////////////////////////////////////
typedef struct sock_params
{
        char ip[15];
        int conn;
}S_Params;
///////////////////////////////////////////////
struct socket_info
{
        int sock_con_status;
	int data_trans_status;
};
///////////////////////////////////////////////
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
/////////////////////////////////////////////
