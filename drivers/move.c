#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "cmd.h"
struct move_interface
{
        void(*move_direct_right)(int angle,int vel);
        void(*move_up_right)(int angle,int vel);
        void(*move_direct_up)(int angle,int vel);
        void(*move_up_left)(int angle,int vel);
        void(*move_direct_left)(int angle,int vel);
        void(*move_down_left)(int angle,int vel);
        void(*move_direct_down)(int angle,int vel);
        void(*move_down_right)(int angle,int vel);
	void(*move_direct_stop)(void);
}move_action;
////////////////////////////////////////////////
void move_direct_up(int angle,int vel)
{
printf("move direct up ,angle %d ,vel %d\n",angle,vel);
}

void move_up_right(int angle,int vel)
{
printf("move up right ,angle %d ,vel %d\n",angle,vel);
}

void move_direct_right(int angle,int vel)
{
printf("move direct right ,angle %d ,vel %d\n",angle,vel);
}

void move_down_right(int angle,int vel)
{
printf("move down right ,angle %d ,vel %d\n",angle,vel);
}

void move_direct_down(int angle,int vel)
{
printf("move direct down ,angle %d ,vel %d\n",angle,vel);
}

void move_down_left(int angle,int vel)
{
printf("move down left ,angle %d ,vel %d\n",angle,vel);
}

void move_direct_left(int angle,int vel)
{
printf("move direct left ,angle %d ,vel %d\n",angle,vel);
}

void move_up_left(int angle,int vel)
{
printf("move up left ,angle %d ,vel %d\n",angle,vel);
}

void move_direct_stop(void)
{
printf("move direct stop\n");
}
/////////////////////////////////////////////
void move_init(void)
{
	move_action.move_direct_up=move_direct_up;
	move_action.move_up_right=move_up_right;
	move_action.move_direct_right=move_direct_right;
	move_action.move_down_right=move_down_right;
	move_action.move_direct_down=move_direct_down;
	move_action.move_down_left=move_down_left;
	move_action.move_direct_left=move_direct_left;
	move_action.move_up_left=move_up_left;
	move_action.move_direct_stop=move_direct_stop;
}
// used by outer function
void move(int move_type,int angle,int vel)
{
switch(move_type)
	{
	case MOVE_DIRECT_UP:move_action.move_direct_up(angle,vel);break;
	case MOVE_UP_RIGHT:move_action.move_up_right(angle,vel);break;
	case MOVE_DIRECT_RIGHT:move_action.move_direct_right(angle,vel);break;
	case MOVE_DOWN_RIGHT:move_action.move_down_right(angle,vel);break;
	case MOVE_DIRECT_DOWN:move_action.move_direct_down(angle,vel);break;
	case MOVE_DOWN_LEFT:move_action.move_down_left(angle,vel);break;
	case MOVE_DIRECT_LEFT:move_action.move_direct_right(angle,vel);break;
	case MOVE_UP_LEFT:move_action.move_up_left(angle,vel);break;
	case MOVE_DIRECT_STOP:move_action.move_direct_stop();break;
	default:break;
	}
}
