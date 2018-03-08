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

#define sizeof(M_Node) NODE_SIZE
M_Pointer init_list(M_Pointer pointer)
{
	pointer = (M_Pointer)malloc(sizeof(M_Node));
	memset(pointer,0,NODE_SIZE);
	pointer->next=NULL;
	pointer->pre=NULL;
	pointer->count=1;
	Head_Pointer=pointer;
	Tail_Pinter=pointer;
	return pointer;
}
//////////////////////////////////////
void list_add(M_Node node)
{
	M_Pointer pointer = (M_Pointer)malloc(sizeof(M_Node));
	memset(pointer,0,NODE_SIZE);
	pointer->next=NULL;
	pointer->pre=Tail_Pointer->next;
	pointer->accel_info->xa_accel=node.accel_info.xa_accel;
	pointer->accel_info->ya_accel=node.accel_info.ya_accel;
	pointer->accel_info->za_accel=node.accel_info.za_accel;
	pointer->accel_info->xl_accel=node.accel_info.xl_accel;
	pointer->accel_info->yl_accel=node.accel_info.yl_accel;
	pointer->accel_info->zl_accel=node.accel_info.zl_accel;
	pointer->vel_info->xa_vel=node.vel_info.xa_vel;
	pointer->vel_info->ya_vel=node.vel_info.ya_vel;
	pointer->vel_info->za_vel=node.vel_info.za_vel;
	pointer->vel_info->xl_vel=node.vel_info.xl_vel;
	pointer->vel_info->yl_vel=node.vel_info.yl_vel;
	pointer->vel_info->zl_vel=node.vel_info.zl_vel;
	pointer->jour_info->xa=node.jour_info.xa;
	pointer->jour_info->ya=node.jour_info.ya;
	pointer->jour_info->za=node.jour_info.za;
	pointer->jour_info->xl=node.jour_info.xl;
	pointer->jour_info->yl=node.jour_info.yl;
	pointer->jour_info->zl=node.jour_info.zl;
	pointer->count=pointer->pre->count+1;
	Tail_Pointer=pointer;
}
/////////////////////////////////////////////////
void list_clear(M_Pointer head)
{






}
