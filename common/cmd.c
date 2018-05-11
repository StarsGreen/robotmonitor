#include <sys/types.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "cmd.h"
#include "data_config.h"
//extern char input_cmd[CMD_LENGTH];
extern void* get_ll_shmid(key_t key,int size);
extern Cmd_Info cmd_info;
/////////////////////////////////////////////////
void* get_move_cmd_addr()
{
int move_cmd_shmid = shmget(MOVE_CMD_KEY,MOVE_CMD_SIZE,IPC_CREAT|0666);
    if(move_cmd_shmid == -1)
    {
        perror("failed to shmget move_cmd_addr\n");
        return NULL;
    }
    void* ptr=NULL;
    if (NULL !=  ptr)
          return NULL;
    ptr= (void*)shmat(move_cmd_shmid,ptr,0);
    return ptr;
}
////////////////////////////////////////// 
int *check_str(char* str)
{
	int i,j=0;
	static int index[3];
	char ch[3]={'t','a','v'};
	int len=strlen(str);
	if(len<6)goto last;
	for(j=0;j<3;j++)
	for(i=0;i<len;i++)
		if(((*(str+i))==ch[j]))
		{
		index[j]=i;
		}
	if(index[0]==0&&index[1]==2&&index[2]==6)
	return index;
	else
	{
//	printf("recv wrong move cmd\n");
	return NULL;
	}
last:return NULL;
}
///////////////////////////////////////////
int read_cmd(char* cmd)
{
	int index[3],i,j;
	int *p;
	int len=strlen(cmd);
//	int len=9;
	p=check_str(cmd);
	if(p!=NULL)
	{
	index[0]=p[0];
	index[1]=p[1];
	index[2]=p[2];
	}
	else goto last;
	char cmd_t[2],vel[2],angle[3];
	memset(angle,0,sizeof(angle));
	memset(vel,0,sizeof(vel));
	memset(cmd_t,0,sizeof(cmd_t));
//        printf("cmd is %s\n",temchar);
//	for(i=0;i<3;i++)
//		printf("index[%d]: %d \n",i,index[i]);
	j=0;
	for(i=index[0]+1;i<index[1];i++)
		*(cmd_t+j++)=*(cmd+i);
	j=0;
	for(i=index[1]+1;i<index[2];i++)
		*(angle+j++)=*(cmd+i);
	j=0;
	for(i=index[2]+1;i<len;i++)
		*(vel+j++)=*(cmd+i);
	if(len>5)
		{
//		move_cmd* m_cmd=get_ll_shmid(MOVE_CMD_KEY,MOVE_CMD_SIZE);
		move_cmd* m_cmd=get_move_cmd_addr();
		pthread_mutex_lock(&m_cmd->lock);
		m_cmd->cmd_type=atoi(cmd_t);
		m_cmd->angle=atoi(angle);
		m_cmd->vel=atoi(vel);
               pthread_mutex_unlock(&m_cmd->lock);
      printf("cmd_type: %d\n",m_cmd->cmd_type);
      printf("cmd_angle: %d\n",m_cmd->angle);
      printf("cmd_vel: %d\n",m_cmd->vel);
		shmdt(m_cmd);
		}
	return 0;
last:	return 1;
}
////////////////////////////////////////////////////
void* get_ctrl_cmd_addr()
{
int ctrl_cmd_shmid = shmget(CTRL_CMD_KEY,CTRL_CMD_SIZE,IPC_CREAT|0666);
    if(ctrl_cmd_shmid == -1)
    {
        perror("failed to shmget move_ll\n");
        return NULL;
    }
    void* ptr=NULL;
    if (NULL !=  ptr)
          return NULL;
    ptr= (void*)shmat(ctrl_cmd_shmid,ptr,0);
    return ptr;
}

///////////////////////////////////////////////////
void set_video_get_on()
{
	Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->video_get_func=1;
	printf("the video get func is on\n");
	shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_video_get_off()
{
        Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->video_get_func=0;
	printf("the video get func is off\n");
        shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_video_send_off()
{
        Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->video_send_func=0;
	printf("the video send func is off\n");
        shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_video_send_on()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->video_send_func=1;
	printf("the video send func is on\n");
        shmdt(ctrl_cmd);
}

//////////////////////////////////////////////////
void set_info_get_on()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->info_get_func=1;
	printf("the info get func is on\n");
        shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_info_get_off()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->info_get_func=0;
	printf("the info get func is off\n");
       shmdt(ctrl_cmd);
}

//////////////////////////////////////////////////
void set_info_send_on()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->info_send_func=1;
	printf("the info send func is on\n");
       shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_info_send_off()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->info_send_func=0;
	printf("the info send func is on\n");
        shmdt(ctrl_cmd);
}

//////////////////////////////////////////////////
void set_move_ctrl_on()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->move_ctrl_func=1;
	printf("the move ctrl func is on\n");
        shmdt(ctrl_cmd);
}
//////////////////////////////////////////////////
void set_move_ctrl_off()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->move_ctrl_func=0;
	printf("the move ctrl func is off\n");
        shmdt(ctrl_cmd);
}
/////////////////////////////////////////////////
void set_all_func_on()
{
       Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
	ctrl_cmd->video_get_func=1;
	ctrl_cmd->video_send_func=1;
	ctrl_cmd->info_get_func=1;
	ctrl_cmd->info_send_func=1;
	ctrl_cmd->move_ctrl_func=1;
	printf("the all function is on\n");
        shmdt(ctrl_cmd);
}
/////////////////////////////////////////////////
void set_all_func_off()
{
 Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
ctrl_cmd->video_get_func=0;
ctrl_cmd->video_send_func=0;
ctrl_cmd->info_get_func=0;
ctrl_cmd->info_send_func=0;
ctrl_cmd->move_ctrl_func=0;
printf("the all function is off\n");
        shmdt(ctrl_cmd);
}
///////////////////printf the move info ////////////
void print_move_info(M_Pointer mp,int flag)
{
char* ptr=(char*)malloc(10);
//printf("move info\n");
printf("--------------move info ----------------\n");
printf("\nmove_ll num is :%d\n",mp->num);
if(flag==0||flag==1)goto accel;
if(flag==2)goto vel;
if(flag==3)goto pos;
if(flag==4)goto journey;
if(flag==5)goto temper;
if(flag==6)goto dist;
if(flag==7)goto gra_cpt;

accel:
printf("accel info :\n");
/*gcvt((double)(mp->accel_info.xa_accel),5,ptr);
printf("|          |       xa_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.ya_accel),5,ptr);
printf("|          |       ya_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.za_accel),5,ptr);
printf("|          |       za_accel     |  %s  |\n",ptr);
*/
gcvt((double)(mp->accel_info.xl_accel),5,ptr);
printf("|          |       xl_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.yl_accel),5,ptr);
printf("|          |       yl_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.zl_accel),5,ptr);
printf("|          |       zl_accel     |  %s  |\n",ptr);
if(flag==1)goto last;


vel:
printf("vel info :\n");

gcvt((double)(mp->vel_info.xa_vel),5,ptr);
printf("|          |       xa_vel      |  %s  |\n",ptr);
gcvt((double)(mp->vel_info.ya_vel),5,ptr);
printf("|          |       ya_vel      |  %s  |\n",ptr);
gcvt((double)(mp->vel_info.za_vel),5,ptr);
printf("|          |       za_vel      |  %s  |\n",ptr);

gcvt((double)(mp->vel_info.xl_vel),5,ptr);
printf("|          |       xl_vel      |  %s  |\n",ptr);
gcvt((double)(mp->vel_info.yl_vel),5,ptr);
printf("|          |       yl_vel      |  %s  |\n",ptr);
gcvt((double)(mp->vel_info.zl_vel),5,ptr);
printf("|          |       zl_vel      |  %s  |\n",ptr);
if(flag==2)goto last;

pos:
printf("pos info :\n");
gcvt((double)(mp->pos_info.roll),5,ptr);
printf("|          |       roll      |  %s  |\n",ptr);
gcvt((double)(mp->pos_info.pitch),5,ptr);
printf("|          |       pitch     |  %s  |\n",ptr);
gcvt((double)(mp->pos_info.yaw),5,ptr);
printf("|          |       yaw       |  %s  |\n",ptr);
if(flag==3)goto last;


journey:
printf("journey info :\n");
gcvt((double)(mp->jour_info.xa),5,ptr);
printf("|          |         xa        |  %s  |\n",ptr);
gcvt((double)(mp->jour_info.ya),5,ptr);
printf("|          |         ya        |  %s  |\n",ptr);
gcvt((double)(mp->jour_info.za),5,ptr);
printf("|          |         za        |  %s  |\n",ptr);

gcvt((double)(mp->jour_info.xl),5,ptr);
printf("|          |         xl        |  %s  |\n",ptr);
gcvt((double)(mp->jour_info.yl),5,ptr);
printf("|          |         yl        |  %s  |\n",ptr);
gcvt((double)(mp->jour_info.zl),5,ptr);
printf("|          |         zl        |  %s  |\n",ptr);
if(flag==4)goto last;

temper:
printf("temper info :\n");
gcvt((double)(mp->temper),5,ptr);
printf("|          |  env temperature  |  %s  |\n",ptr);
if(flag==5)goto last;

dist:
printf("dist info :\n");
gcvt((double)(mp->dist),5,ptr);
printf("|          |      distance     |  %s  |\n",ptr);
if(flag==6)goto last;

gra_cpt:
printf("gra_cpt info :\n");
gcvt((double)(mp->gra_cpt.gra_x),5,ptr);
printf("|          |      gra_x     |  %s  |\n",ptr);
gcvt((double)(mp->gra_cpt.gra_y),5,ptr);
printf("|          |      gra_y     |  %s  |\n",ptr);
gcvt((double)(mp->gra_cpt.gra_z),5,ptr);
printf("|          |      gra_z     |  %s  |\n",ptr);
if(flag==7)goto last;

last:
printf("\n");
printf("---------------move info---------------\n");
}
/////////////////////////////////////////////////
void get_move_info()
{
	mll_ptr p=get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,0);
        pthread_mutex_unlock(&p->move_ll_lock);
	shmdt(p);
	shmdt(tail);
}

//////////////////printf the accel info/////////////////
void get_accel_info()
{
/*        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,1);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
	mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,1);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);
}
/////////////////printf the vel info//////////////////////
void get_vel_info()
{
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,2);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
	mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,2);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);

}

/////////////////printf the pos info//////////////////////
void get_pos_info()
{
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,2);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
	mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,3);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);

}
////////////////////printf the journey info///////////////////
void get_journey_info()
{
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,3);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
	mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,4);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);

}
////////////////////printf the journey info///////////////////
void get_temper_info()
{

	mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,5);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,4);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
}
////////////////////printf the journey info///////////////////
void get_dist_info()
{
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,5);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,6);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);

}
////////////////////get gravity componnent info/////
void get_gra_cpt_info()
{
/*
        pthread_mutex_lock(&move_ll.move_ll_lock);
	print_move_info(move_ll.M_Tail_pointer,5);
        pthread_mutex_unlock(&move_ll.move_ll_lock);
*/
mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
	M_Pointer tail=shmat(p->Tail_shmid,NULL,0);
	pthread_mutex_lock(&p->move_ll_lock);
	print_move_info(tail,7);
        pthread_mutex_unlock(&p->move_ll_lock);
shmdt(p);
shmdt(tail);
}

////////////get ctrl cmd info/////////////////////
void get_ctrl_cmd_info()
{
Ctrl_Pointer ctrl_cmd=get_ctrl_cmd_addr();
printf("\n----------------ctrl cmd ifo----------------\n");
printf("\n           1 is on and 0 is off             \n");
printf("\n      video get func   is       %d",ctrl_cmd->video_get_func);
printf("\n     video send func   is       %d",ctrl_cmd->video_send_func);
printf("\n     info get func     is       %d",ctrl_cmd->info_get_func);
printf("\n     info send func    is       %d",ctrl_cmd->info_send_func);
printf("\n     move ctrl func    is       %d",ctrl_cmd->move_ctrl_func);
printf("\n----------------ctrl cmd ifo-----------------\n");
shmdt(ctrl_cmd);
}
////////////////////////////////////////////////
void help_info()
{
int i;
printf("\n------------------help info-------------------\n");
for(i=0;i<cmd_info.cmd_num;i++)
	printf("\n  %d  |  %s  |  %#8x  \n",i+1,cmd_info.cmd[i].func_name,
cmd_info.cmd[i].cmd_code);
printf("\n------------------help info--------------------\n");
}
/////////find char num////////////////////////
int find_char(char* src,char des)
{
//printf("b 0");
int num=0;
int i;
int str_length=strlen(src);

//printf("b 1");
for(i=0;i<str_length;i++)
	if(*(src+i)==des)num++;

//printf("b 2");
return num;
}
/////////read the input cmd///////////////////
unsigned int get_input_cmd(char*input_cmd)
{
	printf("the input cmd is %s\n",input_cmd);
	printf("get input cmd b0\n");
	unsigned int cmd_code=0;
	int i;
	char* delim=" ";
	printf("get input cmd b2\n");
	char* str[5];
	for(i=0;i<5;i++)
		{
		str[i]=(char*)malloc(MAX_CMD_LENGTH);
		memset(str[i],0,MAX_CMD_LENGTH);
		}
//	char* p=(char*)malloc(MAX_CMD_LENGTH/2);
	char* p;
	printf("get input cmd b3\n");
	int delim_num=find_char(input_cmd,' ');
	printf("the delim num is %d\n",delim_num);
/////////////////
	if(delim_num>0)
	{
	p=strtok(input_cmd,delim);
	printf("the first string is %s\n",p);
	memcpy(str[0],p,strlen(p));
	for(i=0;i<delim_num;i++)
		{
		p=strtok(NULL,delim);
		if(p!=NULL)
		  {
		memcpy(str[i+1],p,strlen(p));
		printf("str[%d] is %s\n",i+1,str[i+1]);
		  }
		}
	}
	else
	{
	printf("get input cmd b4\n");
	memcpy(str[0],input_cmd,strlen(input_cmd));
	printf("str0 is %s\n",str[0]);
	}
/////////////
	if(strncasecmp(str[0],"set",3)==0)
	{
	cmd_code=cmd_code|(0<<28);
	if(strncasecmp(str[1],"videoget",8)==0)cmd_code=cmd_code|(5<<12);
	else if(strncasecmp(str[1],"videosend",9)==0)cmd_code=cmd_code|(1<<12);
	else if(strncasecmp(str[1],"infoget",7)==0)cmd_code=cmd_code|(2<<12);
	else if(strncasecmp(str[1],"infosend",8)==0)cmd_code=cmd_code|(3<<12);
	else if(strncasecmp(str[1],"move",4)==0)cmd_code=cmd_code|(4<<12);
	else if(strncasecmp(str[1],"all",3)==0)cmd_code=cmd_code|(15<<12);

	if(strncasecmp(str[2],"on",2)==0)cmd_code=cmd_code|(1<<4);
	else if(strncasecmp(str[2],"off",3)==0)cmd_code=cmd_code|(0<<4);
	else goto last;
	}
	else
	if(strncasecmp(str[0],"get",3)==0)
	{
	cmd_code=cmd_code|(1<<28);
	if(strncasecmp(str[1],"moveinfo",8)==0)cmd_code=cmd_code|(0<<12);
	else if(strncasecmp(str[1],"accel",5)==0)cmd_code=cmd_code|(1<<12);
	else if(strncasecmp(str[1],"vel",3)==0)cmd_code=cmd_code|(2<<12);
	else if(strncasecmp(str[1],"journey",7)==0)cmd_code=cmd_code|(3<<12);
	else if(strncasecmp(str[1],"temp",4)==0)cmd_code=cmd_code|(4<<12);
	else if(strncasecmp(str[1],"dist",4)==0)cmd_code=cmd_code|(5<<12);
	else if(strncasecmp(str[1],"pos",3)==0)cmd_code=cmd_code|(6<<12);
	else if(strncasecmp(str[1],"gra_cpt",7)==0)cmd_code=cmd_code|(7<<12);
	else if(strncasecmp(str[1],"ctrlinfo",4)==0)cmd_code=0xffffffe0;
	else goto last;
	}
	else
	if(strncasecmp(str[0],"help",4)==0)cmd_code=0xfffffff0;
	else goto last;

	for(i=0;i<5;i++)
		{
		free(str[i]);
//		free(p);
		}
	return cmd_code;
last:	return ERROR_CODE;

}
/////////////////////////////
int excute_cmd(unsigned int code)
{
int i;
printf("\n the excute code is %x\n",code);
if(code==ERROR_CODE)goto last;
for(i=0;i<cmd_info.cmd_num;i++)
	if(cmd_info.cmd[i].cmd_code==code)cmd_info.cmd[i].func();
return 0;
last:
	do{
	printf("the cmd code %x is error\n",code);
	}while(0);
return 1;
}
///////////////////////////////////////////////////
int init_ctrl_cmd()
{
int ctrl_cmd_shmid = shmget(CTRL_CMD_KEY,CTRL_CMD_SIZE,IPC_CREAT|0666);
    if(ctrl_cmd_shmid == -1)
    {
        perror("failed to shmget ctrl_cmd_mem\n");
        return -1;
    }
	return 0;
}
int init_move_cmd()
{
int move_cmd_shmid = shmget(MOVE_CMD_KEY,MOVE_CMD_SIZE,IPC_CREAT|0666);
    if(move_cmd_shmid == -1)
    {
        perror("failed to shmget move_cmd_mem\n");
        return -1;
    }
	return 0;
}
///////////////////////////////////////////////
void init_info_cmd()
{
	cmd_info.cmd[0].cmd_code=SET_V_GET_ON;
	cmd_info.cmd[0].func=set_video_get_on;
	cmd_info.cmd[0].func_name="set video get on";
	cmd_info.cmd_num=1;

	cmd_info.cmd[1].cmd_code=SET_V_GET_OFF;
	cmd_info.cmd[1].func=set_video_get_off;
	cmd_info.cmd[1].func_name="set video get off";
	cmd_info.cmd_num++;

	cmd_info.cmd[2].cmd_code=SET_V_SEND_ON;
	cmd_info.cmd[2].func=set_video_send_on;
	cmd_info.cmd[2].func_name="set video send on";
	cmd_info.cmd_num++;

	cmd_info.cmd[3].cmd_code=SET_V_SEND_OFF;
	cmd_info.cmd[3].func=set_video_send_off;
	cmd_info.cmd[3].func_name="set video send off";
	cmd_info.cmd_num++;

	cmd_info.cmd[4].cmd_code=INFO_GET_ON;
	cmd_info.cmd[4].func=set_info_get_on;
	cmd_info.cmd_num++;
	cmd_info.cmd[4].func_name="set info get on";

	cmd_info.cmd[5].cmd_code=INFO_GET_OFF;
	cmd_info.cmd[5].func=set_info_get_off;
	cmd_info.cmd[5].func_name="set info get off";
	cmd_info.cmd_num++;

	cmd_info.cmd[6].cmd_code=INFO_SEND_OFF;
	cmd_info.cmd[6].func=set_info_send_off;
	cmd_info.cmd[6].func_name="set info send off";
	cmd_info.cmd_num++;

	cmd_info.cmd[7].cmd_code=INFO_SEND_ON;
	cmd_info.cmd[7].func=set_info_send_on;
	cmd_info.cmd[7].func_name="set info send on";
	cmd_info.cmd_num++;

	cmd_info.cmd[8].cmd_code=MOVE_CTRL_OFF;
	cmd_info.cmd[8].func=set_move_ctrl_off;
	cmd_info.cmd[8].func_name="set move ctrl on";
	cmd_info.cmd_num++;

	cmd_info.cmd[9].cmd_code=MOVE_CTRL_ON;
	cmd_info.cmd[9].func=set_move_ctrl_on;
	cmd_info.cmd[9].func_name="set move ctrl on";
	cmd_info.cmd_num++;

	cmd_info.cmd[10].cmd_code=GET_MOVE_INFO;
	cmd_info.cmd[10].func=get_move_info;
	cmd_info.cmd[10].func_name="get move info";
	cmd_info.cmd_num++;

	cmd_info.cmd[11].cmd_code=GET_MOVE_ACCEL;
	cmd_info.cmd[11].func=get_accel_info;
	cmd_info.cmd[11].func_name="get move accel";
	cmd_info.cmd_num++;

	cmd_info.cmd[12].cmd_code=GET_MOVE_VEL;
	cmd_info.cmd[12].func=get_vel_info;
	cmd_info.cmd[12].func_name="get move vel";
	cmd_info.cmd_num++;

	cmd_info.cmd[13].cmd_code=GET_MOVE_JOURNEY;
	cmd_info.cmd[13].func=get_journey_info;
	cmd_info.cmd[13].func_name="get move journey";
	cmd_info.cmd_num++;

	cmd_info.cmd[14].cmd_code=HELP_CODE;
	cmd_info.cmd[14].func=help_info;
	cmd_info.cmd[14].func_name="help info";
	cmd_info.cmd_num++;

	cmd_info.cmd[15].cmd_code=GET_CTRL_CMD;
	cmd_info.cmd[15].func=get_ctrl_cmd_info;
	cmd_info.cmd[15].func_name="ctrl cmd info";
	cmd_info.cmd_num++;

	cmd_info.cmd[16].cmd_code=GET_TEMPER;
	cmd_info.cmd[16].func=get_temper_info;
	cmd_info.cmd[16].func_name="get temperature info";
	cmd_info.cmd_num++;

	cmd_info.cmd[17].cmd_code=GET_DIST;
	cmd_info.cmd[17].func=get_dist_info;
	cmd_info.cmd[17].func_name="get distance info";
	cmd_info.cmd_num++;

	cmd_info.cmd[18].cmd_code=ALL_CTRL_FUNC_ON;
	cmd_info.cmd[18].func=set_all_func_on;
	cmd_info.cmd[18].func_name="set all ctrl func on";
	cmd_info.cmd_num++;

	cmd_info.cmd[19].cmd_code=ALL_CTRL_FUNC_OFF;
	cmd_info.cmd[19].func=set_all_func_off;
	cmd_info.cmd[19].func_name="set all ctrl func off";
	cmd_info.cmd_num++;

	cmd_info.cmd[20].cmd_code=GET_MOVE_POS;
	cmd_info.cmd[20].func=get_pos_info;
	cmd_info.cmd[20].func_name="get move pos";
	cmd_info.cmd_num++;

	cmd_info.cmd[21].cmd_code=GET_GRA_CPT;
	cmd_info.cmd[21].func=get_gra_cpt_info;
	cmd_info.cmd[21].func_name="get gra component info";
	cmd_info.cmd_num++;

}
////////////////////////////////////////////////
void init_cmd()
{
	init_ctrl_cmd();
	init_move_cmd();
	init_info_cmd();
}
//////////////////////////////////////////////////

////////////////////////////////////////////////////
