//defined some datastructure to store global data
#include<data_structure.h>
#ifndef __GLOBAL_DATA_H__
#define __GLOBAL_DATA_H_
#endif
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
Ctrl_Cmd ctrl_cmd;
Ctrl_Pointer ctrl_pointer;
/////////////////////////////////////////////
Cmd_Info cmd_info;
Cmd_Info_Pointer cmd_pointer;

int move_ll_shmid;
int sock_ll_shmid;
