//defined some datastructure to store global data
#include<pthread.h>
#include "cmd.h"
#ifndef __DATA_STRUCTURE_H__
#define __DATA_STRUCTURE_H__
#endif

/////////////////////////////////////////////
typedef struct control_cmd
{
int video_get_func;
int video_send_func;
int info_get_func;
int info_send_func;
int move_ctrl_func;
}Ctrl_Cmd;
typedef Ctrl_Cmd* Ctrl_Pointer;
////////////////////////////////////////////
struct Cmd
{
        int cmd_code;
        void (*func)(void);
	void  (*fun_parms)(void*);
        char* func_name;
};
typedef struct cmd_info
{
        struct Cmd cmd[CMD_NUM];
        int cmd_num;;
}Cmd_Info;
typedef Cmd_Info* Cmd_Info_Pointer;
/////////////////////////////
typedef struct move_cmd_struct
{
	int cmd_type;
	int angle;
	int vel;
	int led_rate;
pthread_mutex_t lock;
}move_cmd;
///////////////////////////////////
typedef struct accel
{
	float xl_accel;
	float yl_accel;
	float zl_accel;
}acceleration_info;

typedef struct velocity
{
	float xa_vel;
	float ya_vel;
	float za_vel;

	float xl_vel;
	float yl_vel;
	float zl_vel;
}velocity_info;

typedef struct journey
{
	float xa;
	float ya;
	float za;

	float xl;
	float yl;
	float zl;
}journey_info;
typedef struct posture
{
	float roll;
	float pitch;
	float yaw;
}posture_info;

typedef struct gra_component
{
	float gra_x;
	float gra_y;
	float gra_z;
}gyro_info;

typedef struct M_Node
{
	struct accel accel_info;
	struct velocity vel_info;
	struct journey jour_info;
	struct posture pos_info;
	struct gra_component gra_cpt;
	float temper;
	float dist;
	int num;
	float sample_time;
//	struct M_Node* next;
//	struct M_Node* prev;
	int prev_shmid;
	int next_shmid;
//	pthread_mutex_t node_lock;
}M_Node;
typedef struct M_Node* M_Pointer;

typedef struct M_LinkList
{
//M_Pointer M_Head_pointer;
//M_Pointer M_Tail_pointer;
int Head_shmid;
int Tail_shmid;
int count;
pthread_mutex_t move_ll_lock;
}mll;
typedef mll* mll_ptr;
/////////////////////////////////////////////
typedef struct motion_node
{
	struct accel accel_info;
	struct velocity vel_info;
	struct journey jour_info;
	struct posture pos_info;
	struct gra_component gra_cpt;
	float temper;
	float dist;
	int num;
	float sample_time;
	struct motion_node* next;
	struct motion_node* prev;
}motion_node;

typedef struct motion_node* mn_ptr; //motion node pointer

typedef struct motion_linklist
{
  mn_ptr head_ptr;
  mn_ptr tail_ptr;
  int count;
}motion_linklist;
typedef motion_linklist* ml_ptr;//motion linklist pointer
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
        int prev_shmid;
        int next_shmid;
//	struct Sock_Node* next;
//	struct Sock_Node* prev;
}Sock_Node;
typedef struct Sock_Node* Sock_Pointer;
typedef struct S_LinkList
{
//Sock_Pointer S_Head_pointer;
//Sock_Pointer S_Tail_pointer;
int Head_shmid;
int Tail_shmid;
int count;
pthread_mutex_t sock_ll_lock;
}sll;
typedef sll* sll_ptr;
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
typedef struce sensor_offset
{
  float xl_accel_offset;
  float yl_accel_offset;
  float zl_accel_offset;
  float xa_vel_offset;
  float ya_vel_offset;
  float za_vel_offset;
  float sensor_zero_shift;
}
