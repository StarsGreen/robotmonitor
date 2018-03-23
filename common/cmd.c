#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "global_data.h"
#include "include.h"
#include "cmd.h"

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
char* get_cmd(char* buffer)
{
		return buffer;
}
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
void get_input_cmd()
{
	

}
///////////////////////////////////////////////////
void init_cmd()
{


}
////////////////////////////////////////////////////
