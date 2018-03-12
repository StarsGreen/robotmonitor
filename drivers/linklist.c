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

#define sizeof(M_Node) M_NODE_SIZE//motion structure size

#define sizeof(Sock_Node) S_NODE_SIZE//motion structure size

M_Pointer init_mlist()
{
	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
	pointer->next=NULL;
	pointer->prev=NULL;
	pointer->num=0;
	move_ll.M_Head_pointer=pointer;
	move_ll.M_Tail_Pinter=pointer;
	move_ll.count=0;
	return pointer;
}
//////////////////////////////////////
void mlist_add(M_Node node)
{
	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
	pointer->next=NULL;

	pointer->prev=move_ll.M_Tail_pointer;
	move_ll.M_Tail_pointer.next=pointer;

	move_ll.M_Tail_pointer=pointer;

	pointer->num=++move_ll.count+1;;

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
}
/////////////////////////////////////////////////
void mlist_clear(M_Pointer head)
{
if(head->next==NULL&&head->prev==NULL)
	memset(head,0,M_NODE_SIZE);
while(head->next!=NULL)
{
	memset(head,0,M_NODE_SIZE);
	head=head->next;
}
	memset(head,0,M_NODE_SIZE);
}
////////////////////////////////////////////////
void* mlist_search_num(int num)
{
void* pointer=NULL;
void* p=move_ll.M_Head_Pointer;
if(p->next==NULL&&p->num==num)
	pointer=p;
while(p->next!=NULL)
	{
	if(p->num==num)
		{
			pointer=p;
			break;
		}
	p=p->next;
	}
if(p->next==NULL&&p->num==num)
	pointer=p;
return pointer;
}
////////////////////////////////////////////////
void mlist_delete(int num)
{
	memset((M_Pointer)mlist_search_num(num),0,M_NODE_SIZE);
}
////////////////////////////////////////////////
Sock_Pointer init_slist()
{
	Sock_Pointer pointer = (Sock_Pointer)malloc(S_NODE_SIZE);
	memset(pointer,0,S_NODE_SIZE);
	pointer->next=NULL;
	pointer->prev=NULL;
	pointer->cli_num=1;

	sock_ll.S_Head_pointer=pointer;
	sock_ll.S_Tail_pinter=pointer;
	sock_ll.count=0;

	return pointer;
}
//////////////////////////////////////////////////////
void mlist_add(Sock_Node node)
{
	Sock_Pointer pointer = (Sock_Pointer)malloc(S_NODE_SIZE);
	memset(pointer,0,S_NODE_SIZE);
	pointer->next=NULL;
	pointer->prev=sock_ll.S_Tail_pointer;
	sock_ll.S_Tail_pointer->next=pointer;

	pointer->cli_num=++sock_ll.count+1;
	
	memcpy(pointer->cli_info->ip,node->cli_info->ip,15);
	
	pointer->cli_info->port=node->cli_info->port;

	sock_ll.S_Tail_pointer=pointer;
}

/////////////////////////////////////////////////
void slist_clear(Sock_Pointer head)
{
if(head->next==NULL&&head->prev==NULL)
	memset(head,0,S_NODE_SIZE);
while(head->next!=NULL)
{
	memset(head,0,S_NODE_SIZE);
	head=head->next;
}
	memset(head,0,S_NODE_SIZE);
}

////////////////////////////////////////////////
void* slist_search_ip(void* ip)
{
void* p=sock_ll.S_Head_pointer;
void* pointer=NULL;
if(p->next==NULL&&memcmp(p->cli_info->ip,ip)==0)
	pointer=p;
while(p->next!=NULL)
	{
	if(memcmp(p->cli_info->ip,ip))
		{
		pointer=p;
		}
	p=p->next;
	}
if(p->next==NULL&&memcmp(p->cli_info->ip,ip)==0)
	num=p->cli_num;
return num;
}
////////////////////////////////////////////////
void slist_delete_num(char* ip)
{
	memset((M_Pointer)mlist_search_ip(ip),0,M_NODE_SIZE);
}

