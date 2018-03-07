#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "global_data.h"

extern void move(short cmd_type,short angle,short vel);
extern void move_init(void);

extern struct move_cmd m_cmd;
extern struct move_info m_info;

void* move_thread(void)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	move_init();
while(1)
	{
	pthread_testcancel();
	move(m_cmd.cmd_type,m_cmd.angle,m_cmd.vel);
	}
}

