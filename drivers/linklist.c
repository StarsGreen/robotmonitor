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

void init_mlist()
{
	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
	pointer->next=NULL;
	pointer->prev=NULL;
	pointer->num=0;
	move_ll.M_Head_pointer=pointer;
	move_ll.M_Tail_pointer=pointer;
	move_ll.count=0;
//	return pointer;
}
//////////////////////////////////////
void mlist_add(M_Node node)
{
	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
	pointer->next=NULL;

        pthread_mutex_lock(&move_ll.move_ll_lock);
	pointer->prev=move_ll.M_Tail_pointer;
	move_ll.M_Tail_pointer->next=pointer;
	move_ll.M_Tail_pointer=pointer;
	pointer->num=++move_ll.count+1;;
        pthread_mutex_unlock(&move_ll.move_ll_lock);

	pointer->accel_info.xl_accel=node.accel_info.xl_accel;
	pointer->accel_info.yl_accel=node.accel_info.yl_accel;
	pointer->accel_info.zl_accel=node.accel_info.zl_accel;
	pointer->vel_info.xa_vel=node.vel_info.xa_vel;
	pointer->vel_info.ya_vel=node.vel_info.ya_vel;
	pointer->vel_info.za_vel=node.vel_info.za_vel;
	pointer->vel_info.xl_vel=node.vel_info.xl_vel;
	pointer->vel_info.yl_vel=node.vel_info.yl_vel;
	pointer->vel_info.zl_vel=node.vel_info.zl_vel;
	pointer->jour_info.xa=node.jour_info.xa;
	pointer->jour_info.ya=node.jour_info.ya;
	pointer->jour_info.za=node.jour_info.za;
	pointer->jour_info.xl=node.jour_info.xl;
	pointer->jour_info.yl=node.jour_info.yl;
	pointer->jour_info.zl=node.jour_info.zl;
	pointer->temper=node.temper;
	pointer->dist=node.dist;
}
/////////////////////////////////////////////////
void mlist_clear(M_Pointer head)
{
if(head->next!=NULL&&head->prev==NULL)
	head=head->next;
while(head->next!=NULL)
{
	memset(head,0,M_NODE_SIZE);
	head=head->next;
}
	memset(head,0,M_NODE_SIZE);
}
////////////////////////////////////////////////
//销毁链表
int destroy_mlist(M_Pointer head)
{
    M_Pointer p;
    if(head==NULL)
        return 0;
    while(head!=NULL)
    {
        p=head->next; 
        free(head);
        head=p;
    }
    return 1;
}
////////////////////////////////////////////////
M_Pointer mlist_search_num(int num)
{
M_Pointer pointer=NULL;

pthread_mutex_lock(&move_ll.move_ll_lock);
M_Pointer p=move_ll.M_Head_pointer->next;
pthread_mutex_unlock(&move_ll.move_ll_lock);

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
	{
		pointer=p;
	}
return pointer;
}
////////////////////////////////////////////////
void mlist_delete(int num)
{
	M_Pointer mp=mlist_search_num(num);
	mp->next->prev=mp->prev;
        mp->prev->next=mp->next;
	if(mp!=NULL)free(mp);
	move_ll.count--;
}
////////////////////////////////////////////////
void init_slist()
{
	Sock_Pointer pointer = (Sock_Pointer)malloc(S_NODE_SIZE);
	memset(pointer,0,S_NODE_SIZE);
	pointer->next=NULL;
	pointer->prev=NULL;
	pointer->cli_num=0;

//        pthread_mutex_lock(&sock_ll.sock_ll_lock);
	sock_ll.S_Head_pointer=pointer;
	sock_ll.S_Tail_pointer=pointer;
	sock_ll.count=0;
//        pthread_mutex_unlock(&sock_ll.sock_ll_lock);
//	return pointer;
}
//////////////////////////////////////////////////////
void slist_add(Sock_Node node)
{
	Sock_Pointer pointer = (Sock_Pointer)malloc(S_NODE_SIZE);
	memset(pointer,0,S_NODE_SIZE);
	pointer->next=NULL;

        pthread_mutex_lock(&sock_ll.sock_ll_lock);
	pointer->prev=sock_ll.S_Tail_pointer;
	sock_ll.S_Tail_pointer->next=pointer;
	sock_ll.S_Tail_pointer=pointer;
	pointer->cli_num=++sock_ll.count+1;
        pthread_mutex_unlock(&sock_ll.sock_ll_lock);


	memcpy(pointer->cli_info.ip,node.cli_info.ip,15);
	pointer->cli_info.port=node.cli_info.port;

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
//////////////////////////////////////////////
//销毁链表
int destroy_slist(Sock_Pointer head)
{
    Sock_Pointer p;
    if(head==NULL)
        return 0;
    while(head!=NULL)
    {
        p=head->next; 
        free(head);
        head=p;
    }
    return 1;
}
////////////////////////////////////////////////
Sock_Pointer slist_search_ip(char* ip)
{
Sock_Pointer p=sock_ll.S_Head_pointer->next;
while(p!=NULL)
	{
	if(memcmp(p->cli_info.ip,ip,15)==0)
		{
		break;
		}
	p=p->next;
	}
return p;
}
////////////////////////////////////////////////
void slist_delete(char* ip)
{
	Sock_Pointer sp=slist_search_ip(ip);
	sp->next->prev=sp->prev;
        sp->prev->next=sp->next;
	if(sp!=NULL)free(sp);
	sock_ll.count--;
}
