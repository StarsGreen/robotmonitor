#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "cmd.h"
extern void move(short cmd_type,short angle,short vel);
extern void move_init(void);
extern void* get_move_cmd_addr(void);

void* move_ctl_thread(void)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL);
	move_init();
while(1)
	{
	pthread_testcancel();
//        while(ctrl_cmd.move_ctrl_func==MOVE_CTRL_DISABLE)
  //              pthread_testcancel();
	usleep(100000);
	move_cmd* m_cmd=get_move_cmd_addr();
	pthread_mutex_lock(&m_cmd->lock);
	move(m_cmd->cmd_type,m_cmd->angle,m_cmd->vel);
//printf("cmd_type is %d,cmd_angle is %d,cmd_vel is %d\n",m_cmd->cmd_type,
//m_cmd->angle,m_cmd->vel);
	pthread_mutex_unlock(&m_cmd->lock);
	shmdt(m_cmd);
	}
}

