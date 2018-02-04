#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "global_data.h"
#include <semaphore.h>
#include "include.h"
#include <signal.h>

///////////////////////////////////////
void input(char* cmd)
{
		char tem_char;
		char input[CMD_LENGTH];
		int num=0;
		memset(input,0,sizeof(input));
	while(1)
	{
                tem_char=getchar();
		if(tem_char == '\n')
		{
		num=0;
		break;
		}
		else
		{
		input[num]=tem_char;
		num++;
		}
	}
	strcpy(cmd,input);
}
/////////////////////////////////////////
void signal_cmd_proceed(int signo)
{
if(signo==SIGINT)	
exit(1);
}
//////////////////////////////////////////
void* cmd_process()
{
	if(signal(SIGINT,signal_cmd_proceed)==SIG_ERR)
		perror("cmd signal error");
while(1)
	{
	char input_cmd[CMD_LENGTH];
	printf("-->");
	input(input_cmd);
//	printf("%s",input_cmd);
	if(!strcmp(input_cmd,"exit"))
	{
	kill(getppid(),SIGINT);
	break;
	}
    }
	while(1);
}
