#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "global_data.h"
#include "include.h"
#include "cmd.h"

extern char input_cmd[CMD_LENGTH];
extern struct move_cmd m_cmd;

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
}
//////////////////////////////////////////////////
void set_video_get_off()
{
	ctrl_cmd.video_get_func=0;
}

//////////////////////////////////////////////////
void set_video_send_off()
{
	ctrl_cmd.video_send_func=0;
}
//////////////////////////////////////////////////
void set_video_send_on()
{
	ctrl_cmd.video_send_func=1;
}

//////////////////////////////////////////////////
void set_info_get_on()
{
	ctrl_cmd.info_get_func=1;
}
//////////////////////////////////////////////////
void set_info_get_off()
{
	ctrl_cmd.info_get_func=0;
}

//////////////////////////////////////////////////
void set_info_send_on()
{
	ctrl_cmd.info_send_func=1;
}
//////////////////////////////////////////////////
void set_info_send_off()
{
	ctrl_cmd.info_send_func=0;
}

//////////////////////////////////////////////////
void set_move_ctrl_on()
{
	ctrl_cmd.move_ctrl_func=1;
}
//////////////////////////////////////////////////
void set_move_ctrl_off()
{
	ctrl_cmd.move_ctrl_func=0;
}

///////////////////printf the move info ////////////
void print_move_info(M_Poninter mp,int flag)
{
char* ptr;
printf("\n");
printf("------------------------------\n");
if(flag==0||flag==1)goto accel;
if(flag==2)goto vel;
if(flag==3)goto journey;

accel:
gcvt((double)mp.accel_info.xa_accel,5,ptr);
printf("|          |  xa_accel  |  %s  |\n",ptr);
gcvt((double)mp.accel_info.ya_accel,5,ptr);
printf("|          |  ya_accel  |  %s  |\n",ptr);
gcvt((double)mp.accel_info.za_accel,5,ptr);
printf("|          |  za_accel  |  %s  |\n",ptr);

gcvt((double)mp.accel_info.xl_accel,5,ptr);
printf("|          |  xl_accel  |  %s  |\n",ptr);
gcvt((double)mp.accel_info.yl_accel,5,ptr);
printf("|          |  yl_accel  |  %s  |\n",ptr);
gcvt((double)mp.accel_info.zl_accel,5,ptr);
printf("|          |  zl_accel  |  %s  |\n",ptr);
if(flag==1)goto last;


vel:
gcvt((double)mp.vel_info.xa_vel,5,ptr);
printf("|          |  xa_vel  |  %s  |\n",ptr);
gcvt((double)mp.vel_info.ya_vel,5,ptr);
printf("|          |  ya_vel  |  %s  |\n",ptr);
gcvt((double)mp.vel_info.za_vel,5,ptr);
printf("|          |  za_vel  |  %s  |\n",ptr);

gcvt((double)mp.vel_info.xl_vel,5,ptr);
printf("|          |  xl_vel  |  %s  |\n",ptr);
gcvt((double)mp.vel_info.yl_vel,5,ptr);
printf("|          |  yl_vel  |  %s  |\n",ptr);
gcvt((double)mp.vel_info.zl_vel,5,ptr);
printf("|          |  zl_vel  |  %s  |\n",ptr);
if(flag==2)goto last;


journey:
gcvt((double)mp.jour_info.xa,5,ptr);
printf("|          |  xa  |  %s  |\n",ptr);
gcvt((double)mp.jour_info.ya,5,ptr);
printf("|          |  ya  |  %s  |\n",ptr);
gcvt((double)mp.jour_info.za,5,ptr);
printf("|          |  za  |  %s  |\n",ptr);

gcvt((double)mp.jour_info.xl,5,ptr);
printf("|          |  xl  |  %s  |\n",ptr);
gcvt((double)mp.jour_info.yl,5,ptr);
printf("|          |  yl  |  %s  |\n",ptr);
gcvt((double)mp.jour_info.zl,5,ptr);
printf("|          |  zl  |  %s  |\n",ptr);


last:
printf("\n");
printf("------------------------------\n");
}
void get_move_info()
{
	print_move_info(M_info,0);
}

//////////////////printf the accel info/////////////////
void get_accel_info()
{
	print_accel_info(M_info,1);
}
/////////////////printf the vel info//////////////////////
void get_vel_info()
{
	print_move_info(M_info,2);
}
////////////////////printf the journey info///////////////////
void get_journey_info()
{
	print_journey_info(M_info,3);
}
////////////////////////////////////////////////
void help_info()
{
int i;
printf("\n-------------help info-------------\n");
for(i=0;i<CMD_NUM;i++)
	printf("\n  %d  |  %s  |  %x  \n",i+1,&(cmd_info.cmd[i].func),
cmd_info.cmd[i].cmd_code);
printf("\n-------------help info-------------\n");
}
/////////read the input cmd///////////////////
int get_input_cmd()
{
	int cmd_code=0,i;
	char* delim=" ";
	char* str[5];
	char* p;
	p=strtok(input_cmd,delim);
	memcpy(str[0],p,strlen(p));
	for(i=0;i<3;i++)
		if((p=strtok(NULL,delim)))
		memcpy(str[i+1],p,strlen(p));
printf("str0 is:%s \n str1 is:%s \n str2 is:%s \n ",str[0],str[1],str[2]);

	if(strncasecmp(str[0],"set"))
	{
	cmd_code=cmd_code|(0<<28);
	if(strncasecmp(str[1],"videoget"))cmd_code=cmd_code|(0<<12);
	else if(strncasecmp(str[1],"videosend"))cmd_code=cmd_code|(1<<12);
	else if(strncasecmp(str[1],"infoget"))cmd_code=cmd_code|(2<<12);
	else if(strncasecmp(str[1],"infosend"))cmd_code=cmd_code|(3<<12);
	else if(strncasecmp(str[1],"move"))cmd_code=cmd_code|(4<<12);
	else goto last;

	if(strncasecmp(str[2],"on"))cmd_code=cmd_code|(1<<4);
	else if(strncasecmp(str[2],"off"))cmd_code=cmd_code|(1<<4);
	else goto last;
	}
	else
	if(strncasecmp(str[0],"get"))
	{
	cmd_code=cmd_code|(1<<28);
	if(strncasecmp(str[1],"moveinfo"))cmd_code=cmd_code|(0<<12);
	else if(strncasecmp(str[1],"accel"))cmd_code=cmd_code|(1<<12);
	else if(strncasecmp(str[1],"vel"))cmd_code=cmd_code|(2<<12);
	else if(strncasecmp(str[1],"journey"))cmd_code=cmd_code|(3<<12);
	else if(strncasecmp(str[1],"temp"))cmd_code=cmd_code|(4<<12);
	else if(strncasecmp(str[1],"dist")cmd_code=cmd_code|(5<<12);
	else goto last;
	}
	else if(strncasecmp(str[0],"help"))cmd_code=0xffffffff;
	else goto last;

	return cmd_code;
//	if(strncasecmp(str[1],"videosend"))cmd_code=cmd_code|(1<<12);
last:	return 0;

}
/////////////////////////////
void excute_cmd(int code)
{
int i;
for(i=0;i<CMD_NUM;i++)
	if(cmd_info.cmd[i].cmd_code==code)cmd_info.cmd[i].func();
}
///////////////////////////////////////////////////
void init_cmd()
{
	cmd_info.cmd[0].cmd_code=SET_V_GET_ON;
	cmd_info.cmd[0].func=set_video_get_on;
	cmd_info.cmd_num=1;
	cmd_info.cmd[1].cmd_code=SET_V_GET_OFF;
	cmd_info.cmd[1].func=set_video_get_off;
	cmd_info.cmd_num++;
	cmd_info.cmd[2].cmd_code=SET_V_SEND_ON;
	cmd_info.cmd[2].func=set_video_send_on;
	cmd_info.cmd_num++;
	cmd_info.cmd[3].cmd_code=SET_V_SEND_OFF;
	cmd_info.cmd[3].func=set_video_send_off;
	cmd_info.cmd_num++;
	cmd_info.cmd[4].cmd_code=INFO_GET_ON;
	cmd_info.cmd[4].func=set_info_get_on;
	cmd_info.cmd_num++;
	cmd_info.cmd[5].cmd_code=INFO_GET_OFF;
	cmd_info.cmd[5].func=set_info_get_off;
	cmd_info.cmd_num++;
	cmd_info.cmd[6].cmd_code=INFO_SEND_OFF;
	cmd_info.cmd[6].func=set_info_send_off;
	cmd_info.cmd_num++;
	cmd_info.cmd[7].cmd_code=INFO_SEND_ON;
	cmd_info.cmd[7].func=set_info_send_on;
	cmd_info.cmd_num++;
	cmd_info.cmd[8].cmd_code=MOVE_CTRL_OFF;
	cmd_info.cmd[8].func=set_move_ctrl_off;
	cmd_info.cmd_num++;
	cmd_info.cmd[9].cmd_code=MOVE_CTRL_ON;
	cmd_info.cmd[9].func=set_move_ctrl_on;
	cmd_info.cmd_num++;
	cmd_info.cmd[10].cmd_code=GET_MOVE_INFO;
	cmd_info.cmd[10].func=get_move_info;
	cmd_info.cmd_num++;
	cmd_info.cmd[11].cmd_code=GET_MOVE_ACCEL;
	cmd_info.cmd[11].func=get_accel_info;
	cmd_info.cmd_num++;
	cmd_info.cmd[12].cmd_code=GET_MOVE_VEL;
	cmd_info.cmd[12].func=get_vel_info;
	cmd_info.cmd_num++;
	cmd_info.cmd[13].cmd_code=GET_MOVE_JOURNEY;
	cmd_info.cmd[13].func=get_journey_info;
	cmd_info.cmd_num++;

	cmd_info.cmd[14].cmd_code=HELP_CODE;
	cmd_info.cmd[14].func=help_info;
	cmd_info.cmd_num++;
}
////////////////////////////////////////////////////
