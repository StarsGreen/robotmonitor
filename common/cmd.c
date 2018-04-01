#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "global_data.h"
#include "include.h"
#include "cmd.h"

//extern char input_cmd[CMD_LENGTH];
//extern struct move_cmd m_cmd;

int *check_str(char* str)
{
	int i,j=0;
	static int index[3];
	int len=strlen(str);
	for(i=0;i<len;i++)
		if(((*(str+i))=='t')||((*(str+i)=='a'))||((*(str+i)=='v')))
		{
		index[j]=i;
		j++;
		}
	return index;
}
//////////////////////////////////////////////
char* get_cmd(char* buffer)
{
		return buffer;
}
///////////////////////////////////////////
void read_cmd(char* cmd)
{
	int index[3],i,j;
	char *temchar,cmd_t[2],vel[2],angle[3];
	memset(angle,0,sizeof(angle));
	memset(vel,0,sizeof(vel));
	memset(cmd_t,0,sizeof(cmd_t));
	temchar=get_cmd(cmd);
//        printf("cmd is %s\n",temchar);
	int len=strlen(temchar);
	for(j=0;j<3;j++)
		*(index+j)=*(check_str(temchar)+j);
//	for(i=0;i<3;i++)
//		printf("index[%d]: %d \n",i,index[i]);
	j=0;
	for(i=index[0]+1;i<index[1];i++)
		*(cmd_t+j++)=*(temchar+i);
	j=0;
	for(i=index[1]+1;i<index[2];i++)
		*(angle+j++)=*(temchar+i);
	j=0;

	for(i=index[2]+1;i<len;i++)
		*(vel+j++)=*(temchar+i);

	if(len>5)
		{
		m_cmd.cmd_type=atoi(cmd_t);
		m_cmd.angle=atoi(angle);
		m_cmd.vel=atoi(vel);
		}
//	printf("cmd_type %d\n",m_cmd.cmd_type);
//	printf("cmd_angle %d\n",m_cmd.angle);
//	printf("cmd_vel %d\n",m_cmd.vel);
}
////////////////////////////////////////////////////

///////////////////////////////////////////////////
void set_video_get_on()
{
	ctrl_cmd.video_get_func=1;
	printf("the video get func is on\n");
}
//////////////////////////////////////////////////
void set_video_get_off()
{
	ctrl_cmd.video_get_func=0;
	printf("the video get func is off\n");
}

//////////////////////////////////////////////////
void set_video_send_off()
{
	ctrl_cmd.video_send_func=0;
	printf("the video send func is off\n");
}
//////////////////////////////////////////////////
void set_video_send_on()
{
	ctrl_cmd.video_send_func=1;
	printf("the video send func is on\n");
}

//////////////////////////////////////////////////
void set_info_get_on()
{
	ctrl_cmd.info_get_func=1;
	printf("the info get func is on\n");
}
//////////////////////////////////////////////////
void set_info_get_off()
{
	ctrl_cmd.info_get_func=0;
	printf("the info get func is off\n");
}

//////////////////////////////////////////////////
void set_info_send_on()
{
	ctrl_cmd.info_send_func=1;
	printf("the info send func is on\n");
}
//////////////////////////////////////////////////
void set_info_send_off()
{
	ctrl_cmd.info_send_func=0;
	printf("the info send func is on\n");
}

//////////////////////////////////////////////////
void set_move_ctrl_on()
{
	ctrl_cmd.move_ctrl_func=1;
	printf("the move ctrl func is on\n");
}
//////////////////////////////////////////////////
void set_move_ctrl_off()
{
	ctrl_cmd.move_ctrl_func=0;
	printf("the move ctrl func is off\n");
}

///////////////////printf the move info ////////////
void print_move_info(M_Pointer mp,int flag)
{
char* ptr=(char*)malloc(10);
//printf("move info\n");
printf("--------------move info ----------------\n");
if(flag==0||flag==1)goto accel;
if(flag==2)goto vel;
if(flag==3)goto journey;
if(flag==4)goto temper;
if(flag==5)goto dist;

accel:
printf("accel info :\n");
gcvt((double)(mp->accel_info.xa_accel),5,ptr);
printf("|          |       xa_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.ya_accel),5,ptr);
printf("|          |       ya_accel     |  %s  |\n",ptr);
gcvt((double)(mp->accel_info.za_accel),5,ptr);
printf("|          |       za_accel     |  %s  |\n",ptr);

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
if(flag==3)goto last;

temper:
printf("temper info :\n");
gcvt((double)(mp->temper),5,ptr);
printf("|          |  env temperature  |  %s  |\n",ptr);
if(flag==4)goto last;

dist:
printf("dist info :\n");
gcvt((double)(mp->dist),5,ptr);
printf("|          |      distance     |  %s  |\n",ptr);
if(flag==5)goto last;

last:
printf("\n");
printf("------------------------------\n");
}
void get_move_info()
{
	print_move_info(&M_info,0);
}

//////////////////printf the accel info/////////////////
void get_accel_info()
{
	print_move_info(&M_info,1);
}
/////////////////printf the vel info//////////////////////
void get_vel_info()
{
	print_move_info(&M_info,2);
}
////////////////////printf the journey info///////////////////
void get_journey_info()
{
	print_move_info(&M_info,3);
}
////////////////////printf the journey info///////////////////
void get_temper_info()
{
	print_move_info(&M_info,4);
}
////////////////////printf the journey info///////////////////
void get_dist_info()
{
	print_move_info(&M_info,5);
}
////////////get ctrl cmd info/////////////////////
void get_ctrl_cmd_info()
{
printf("\n----------------ctrl cmd ifo----------------\n");
printf("\n           1 is on and 0 is off             \n");
printf("\n      video get func   is       %d",ctrl_cmd.video_get_func);
printf("\n     video send func   is       %d",ctrl_cmd.video_send_func);
printf("\n     info get func     is       %d",ctrl_cmd.info_get_func);
printf("\n     info send func    is       %d",ctrl_cmd.info_send_func);
printf("\n     move ctrl func    is       %d",ctrl_cmd.move_ctrl_func);
printf("\n----------------ctrl cmd ifo-----------------\n");
}
////////////////////////////////////////////////
void help_info()
{
int i;
printf("\n------------------help info-------------------\n");
for(i=0;i<cmd_info.cmd_num;i++)
	printf("\n  %d  |  %s  |  %x  \n",i+1,cmd_info.cmd[i].func_name,
cmd_info.cmd[i].cmd_code);
printf("\n------------------help info--------------------\n");
}
/////////fin char num////////////////////////
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
	else goto last;
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
void init_ctrl_cmd()
{
memset(&ctrl_cmd,0,CTRL_CMD_SIZE);
}
///////////////////////////////////////////////
void init_cmd()
{
	init_ctrl_cmd();

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

}
//////////////////////////////////////////////////

////////////////////////////////////////////////////
