#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "data_structure.h"
#include "data_config.h"
#include "cmd.h"
#include <signal.h>

///////////////////////////////////////
char input_cmd[CMD_LENGTH];
Cmd_Info cmd_info;
extern unsigned int get_input_cmd(char* input_cmd);
extern int excute_cmd(unsigned int code);
extern void init_cmd();
extern void get_move_info();
///////////////////////////////////////


///////////////////////////////////////
int input(char* cmd)
{
		char tem_char;
		char input[CMD_LENGTH];
		int cmd_length=0;
		int num=0;
		memset(input,0,sizeof(input));
	while(1)
	{
                tem_char=getchar();
		if(tem_char == '\n')
		{
		cmd_length=num;
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
	return cmd_length;
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
	int cmd_length=0;
	unsigned int code=0;
	if(signal(SIGINT,signal_cmd_proceed)==SIG_ERR)
		perror("cmd signal error");
	init_cmd();
while(1)
  {
	printf("\n-->");
	cmd_length=input(input_cmd);
	printf("\n the input is %s\n",input_cmd);
	if(cmd_length==0)goto last;
	if(!strcmp(input_cmd,"exit"))
    {
	kill(getppid(),SIGINT);
	break;
    }
	else
    {
//	printf("excute step\n");
	printf("cmd length is %d\n",strlen(input_cmd));
//	excute_cmd(0xfffffff0);
//	printf("the code is %x",code);
	code=get_input_cmd(input_cmd);
//	printf("the code is %x",code);
	excute_cmd(code);

    }

last:
	do{
//	get_move_info();
	}while(0);

  }

	while(1);
}
