#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "data_structure.h"
#include "data_config.h"
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
//#include "include.h"
#include <signal.h>
void* get_shm_addr(int size)
{
    int shmid0 = shmget(0,size,IPC_CREAT|0666);
    if(shmid0 == -1)
    {
        perror("failed to shmget move_ll_node when create new node\n");
        return NULL;
    }
    return (void*)shmat(shmid0,NULL,0);
}
/////////////////////////////////////////////////
/////////////////////////////////////////////////
void* get_ll_shmid(key_t key,int size)
{
    int move_ll_shmid = shmget(key,size,IPC_CREAT|0666);
    if(move_ll_shmid == -1)
    {
        perror("failed to shmget move_ll\n");
        return NULL;
    }
    void* ptr=NULL;
    if (NULL !=  ptr)
          return NULL;
    ptr= (void*)shmat(move_ll_shmid,ptr,0);
    return ptr;
}
////////////////////////////////////
int init_mlist()
{
    mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
    int shmid0 = shmget(0,M_NODE_SIZE,IPC_CREAT|0666);
    if(shmid0 == -1)
    {
        perror("failed to shmget move_ll_node when create head pointer\n");
        return -1;
    }
        M_Pointer pointer = (M_Pointer)shmat(shmid0,NULL,0);
//	M_Pointer pointer = (M_Pointer)get_shm_addr(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
	pointer->next_shmid=0;
	pointer->prev_shmid=0;
	pointer->num=0;

	p->Head_shmid=shmid0;
	p->Tail_shmid=shmid0;
//	p->M_Head_pointer=pointer;
//	p->M_Tail_pointer=pointer;
	p->count=0;
//	return pointer;
	shmdt(p);
	shmdt(pointer);
	return 0;
}
//////////////////////////////////////
int mlist_add(M_Node node)
{
    mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
    int shmid0 = shmget(0,M_NODE_SIZE,IPC_CREAT|0666);
    if(shmid0 == -1)
    {
        perror("failed to shmget move_ll_node when add node\n");
        return -1;
    }
        M_Pointer pointer = (M_Pointer)shmat(shmid0,NULL,0);
//	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,M_NODE_SIZE);
//	pointer->next=NULL;
        pointer->next_shmid=0;
        pointer->prev_shmid=0;
//	shmat(move_ll_shmid,NULL,0);

	pointer->accel_info.xl_accel=node.accel_info.xl_accel;
	pointer->accel_info.yl_accel=node.accel_info.yl_accel;
	pointer->accel_info.zl_accel=node.accel_info.zl_accel;

	pointer->vel_info.xa_vel=node.vel_info.xa_vel;
	pointer->vel_info.ya_vel=node.vel_info.ya_vel;
	pointer->vel_info.za_vel=node.vel_info.za_vel;

	pointer->vel_info.xl_vel=node.vel_info.xl_vel;
	pointer->vel_info.yl_vel=node.vel_info.yl_vel;
	pointer->vel_info.zl_vel=node.vel_info.zl_vel;

	pointer->pos_info.pitch=node.pos_info.pitch;
	pointer->pos_info.roll=node.pos_info.roll;
	pointer->pos_info.yaw=node.pos_info.yaw;

	pointer->gra_cpt.gra_x=node.gra_cpt.gra_x;
	pointer->gra_cpt.gra_y=node.gra_cpt.gra_y;
	pointer->gra_cpt.gra_z=node.gra_cpt.gra_z;

	pointer->jour_info.xa=node.jour_info.xa;
	pointer->jour_info.ya=node.jour_info.ya;
	pointer->jour_info.za=node.jour_info.za;

	pointer->jour_info.xl=node.jour_info.xl;
	pointer->jour_info.yl=node.jour_info.yl;
	pointer->jour_info.zl=node.jour_info.zl;

	pointer->temper=node.temper;
	pointer->dist=node.dist;
	pointer->sample_time=node.sample_time;


        pthread_mutex_lock(&p->move_ll_lock);
	M_Pointer mp=shmat(p->Tail_shmid,NULL,0);
	mp->next_shmid=shmid0;
	pointer->prev_shmid=p->Tail_shmid;
	p->Tail_shmid=shmid0;
	pointer->num=p->count+1;
	p->count++;
        pthread_mutex_unlock(&p->move_ll_lock);

	shmdt(mp);
	shmdt(p);
	shmdt(pointer);

	return 0;
}
/////////////////////////////////////////////////
int mlist_clear(void)
{
int num=0;
int i=0;
M_Pointer tem_p[MAX_NODE_NUM];
memset(tem_p,0,M_NODE_SIZE);
    mll_ptr p=( mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
    M_Pointer pointer = (M_Pointer)shmat(p->Head_shmid,NULL,0);
    while(pointer->next_shmid!=0)
   {
        M_Pointer mp = (M_Pointer)shmat(pointer->next_shmid,NULL,0);
	tem_p[num]=mp;
	num++;
	memset(&mp->accel_info,0,sizeof(mp->accel_info));
	memset(&mp->vel_info,0,sizeof(mp->vel_info));
	memset(&mp->jour_info,0,sizeof(mp->jour_info));
	memset(&mp->temper,0,sizeof(mp->temper));

	memset(&mp->dist,0,sizeof(mp->dist));
	pointer=mp;
   }
   shmdt(pointer);
for(i=0;i<num-1;i++)
	shmdt(tem_p[i]);
	return 0;
//   shmdt(mp);
/*
if(head->next!=NULL&&head->prev==NULL)
	head=head->next;
while(head->next!=NULL)
{
	memset(head,0,M_NODE_SIZE);
	head=head->next;
}
	memset(head,0,M_NODE_SIZE);
*/
}
/////////////////////////////////////////////
int rebuild_mlist()
{
int num=0;
    mll_ptr p=( mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
    M_Pointer pointer = (M_Pointer)shmat(p->Tail_shmid,NULL,0);
    while(pointer->prev_shmid!=0)
   {
        M_Pointer mp = (M_Pointer)shmat(pointer->prev_shmid,NULL,0);
	shmctl(mp->next_shmid,IPC_RMID,NULL);
	pointer=mp;
//	printf("destroy shm %d",num++);
   }
   p->Tail_shmid= p->Head_shmid;
   p->count=0;
   pointer->next_shmid=0;
   pointer->prev_shmid=0;
   shmdt(pointer);
   shmdt(p);
   return 0;
}
////////////////////////////////////////////////
//销毁链表
int destroy_mlist()
{
    mll_ptr p=( mll_ptr)get_ll_shmid(MOVE_LL_KEY,M_NODE_SIZE);
    M_Pointer pointer = (M_Pointer)shmat(p->Head_shmid,NULL,0);
    if(pointer==NULL)return -1;
    while(pointer->next_shmid!=0)
    {
        M_Pointer mp = (M_Pointer)shmat(pointer->next_shmid,NULL,0);
	shmctl(mp->prev_shmid,IPC_RMID,NULL);
	pointer=mp;
    }
	shmctl(p->Tail_shmid,IPC_RMID,NULL);
	return 0;
/*
    M_Pointer p;
    if(head==NULL)
        return 0;
    while(head!=NULL)
    {
        p=head->next; 
        free(head);
        head=p;
    }
    return 1;*/

}
////////////////////////////////////////////////
M_Pointer mlist_search_num(int num)
{

int count=0;
int i=0;
M_Pointer tem_p[MAX_NODE_NUM];
memset(tem_p,0,MAX_NODE_NUM*sizeof(int));

M_Pointer rp=NULL;
M_Pointer mp=NULL;

mll_ptr p=(mll_ptr)get_ll_shmid(MOVE_LL_KEY,MOVE_LL_SIZE);
pthread_mutex_lock(&p->move_ll_lock);
M_Pointer pointer=(M_Pointer)shmat(p->Head_shmid,NULL,0);
while(pointer->next_shmid!=0)
{
mp=(M_Pointer)shmat(pointer->next_shmid,NULL,0);
if(mp->num==num)
	{
	rp=mp;
	goto last;
	}
tem_p[count]=mp;
count++;
pointer=mp;
}
//if(mp->num==num)rp=mp;
last:
pthread_mutex_unlock(&p->move_ll_lock);
shmdt(pointer);
for(i=0;i<count-1;i++)
        shmdt(tem_p[i]);
return rp;
/*
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
return pointer;*/
}
////////////////////////////////////////////////
int mlist_delete(int num)
{
        M_Pointer mp=mlist_search_num(num);
	M_Pointer mp0=NULL;
	M_Pointer mp1=NULL;
	int id=0;

	if(mp!=NULL)
	{
	if(mp->prev_shmid!=0)
		mp0=(M_Pointer)shmat(mp->prev_shmid,NULL,0);
	if(mp->next_shmid!=0)
		mp1=(M_Pointer)shmat(mp->next_shmid,NULL,0);
	id=mp0->next_shmid;
	if(mp0!=NULL)
		{
		mp0->next_shmid=mp->next_shmid;
		shmdt(mp0);
		}
	if(mp1!=NULL)
		{
		mp1->prev_shmid=mp->prev_shmid;
		shmdt(mp1);
		}
	shmctl(id,IPC_RMID,NULL);
	shmdt(mp);
	}
	return 0;
	/*
	M_Pointer mp=mlist_search_num(num);
	mp->next->prev=mp->prev;
        mp->prev->next=mp->next;
	if(mp!=NULL)free(mp);
	move_ll.count--;
*/
}
////////////////////////////////////////////////
int init_slist()
{
    int sock_ll_shmid = shmget(SOCK_LL_KEY,SOCK_LL_SIZE,IPC_CREAT|0666);
    if(sock_ll_shmid == -1)
    {
        perror("failed to shmget sock_ll\n");
        return -1;
    }
    sll_ptr p=(sll_ptr)shmat(sock_ll_shmid,NULL,0);

    int shmid0 = shmget(0,S_NODE_SIZE,IPC_CREAT|0666);
    if(shmid0 == -1)
    {
        perror("failed to shmget sock_ll_node\n");
        return -1;
    }
	Sock_Pointer pointer = (Sock_Pointer)shmat(shmid0,NULL,0);
	memset(pointer,0,S_NODE_SIZE);
	pointer->next_shmid=0;
	pointer->prev_shmid=0;
	pointer->cli_num=0;
//        pthread_mutex_lock(&sock_ll.sock_ll_lock);
	p->Head_shmid=shmid0;
	p->Tail_shmid=shmid0;
	p->count=0;
//      pthread_mutex_unlock(&sock_ll.sock_ll_lock);
//	return pointer;
	shmdt(p);
	shmdt(pointer);
	return 0;
}
//////////////////////////////////////////////////////
int slist_add(Sock_Node node)
{

sll_ptr p=(sll_ptr)get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
    int shmid0 = shmget(0,S_NODE_SIZE,IPC_CREAT|0666);
    if(shmid0 == -1)
    {
        perror("failed to shmget move_ll_node\n");
        return -1;
    }
        Sock_Pointer pointer = (Sock_Pointer)shmat(shmid0,NULL,0);
//	M_Pointer pointer = (M_Pointer)malloc(M_NODE_SIZE);
	memset(pointer,0,S_NODE_SIZE);
//	pointer->next=NULL;
        pointer->next_shmid=0;
//	shmat(move_ll_shmid,NULL,0);
        pthread_mutex_lock(&p->sock_ll_lock);

	Sock_Pointer mp=shmat(p->Tail_shmid,NULL,0);
	mp->next_shmid=shmid0;
	pointer->prev_shmid=p->Tail_shmid;
	pointer->next_shmid=0;
	pointer->cli_num=p->count+1;

	p->Tail_shmid=shmid0;
	p->count++;

        pthread_mutex_unlock(&p->sock_ll_lock);

	memcpy(pointer->cli_info.ip,node.cli_info.ip,15);
	pointer->cli_info.port=node.cli_info.port;
	pointer->cli_num=p->count+1;

	shmdt(p);
	shmdt(pointer);
	shmdt(mp);
	return 0;
/*
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
*/
}

/////////////////////////////////////////////////
int slist_clear()
{
int num=0;
int i=0;
Sock_Pointer tem_p[MAX_NODE_NUM];
memset(tem_p,0,M_NODE_SIZE);
sll_ptr p=(sll_ptr)get_ll_shmid(MOVE_LL_KEY,SOCK_LL_SIZE);
    Sock_Pointer pointer = (Sock_Pointer)shmat(p->Head_shmid,NULL,0);
    while(pointer->next_shmid!=0)
   {
        Sock_Pointer sp = (Sock_Pointer)shmat(pointer->next_shmid,NULL,0);
	tem_p[num]=sp;
	num++;
	memset(&sp->cli_info,0,sizeof(sp->cli_info));
	memset(&sp->cli_num,0,sizeof(sp->cli_num));
	pointer=sp;
   }
   shmdt(pointer);
for(i=0;i<num-1;i++)
	shmdt(tem_p[i]);
	return 0;
/*
if(head->next==NULL&&head->prev==NULL)
	memset(head,0,S_NODE_SIZE);
while(head->next!=NULL)
{
	memset(head,0,S_NODE_SIZE);
	head=head->next;
}
	memset(head,0,S_NODE_SIZE);
*/
}
//////////////////////////////////////////////
//销毁链表
int destroy_slist(void)
{
sll_ptr p=( sll_ptr)get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
    Sock_Pointer pointer = (Sock_Pointer)shmat(p->Head_shmid,NULL,0);
    if(pointer==NULL)return -1;
    while(pointer->next_shmid!=0)
    {
        Sock_Pointer sp = (Sock_Pointer)shmat(pointer->next_shmid,NULL,0);
	shmctl(sp->prev_shmid,IPC_RMID,NULL);
	pointer=sp;
    }
	shmctl(p->Tail_shmid,IPC_RMID,NULL);
	return 0;
/*
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
*/
}
////////////////////////////////////////////////
Sock_Pointer slist_search_ip(char* ip)
{
int count=0,i=0;
Sock_Pointer tem_p[MAX_NODE_NUM];
memset(tem_p,0,MAX_NODE_NUM*sizeof(int));

Sock_Pointer rp=NULL;
Sock_Pointer sp=NULL;

sll_ptr p=(sll_ptr)get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
pthread_mutex_lock(&p->sock_ll_lock);
Sock_Pointer pointer=(Sock_Pointer)shmat(p->Head_shmid,NULL,0);
while(pointer->next_shmid!=0)
{
sp=(Sock_Pointer)shmat(pointer->next_shmid,NULL,0);
if(memcmp(sp->cli_info.ip,ip,15)==0)
	{
	rp=sp;
	goto last;
	}
tem_p[count]=sp;
count++;
pointer=sp;
}
//if(memcmp(sp->cli_info.ip,ip,15)==0)rp=sp;
last:
pthread_mutex_unlock(&p->sock_ll_lock);
shmdt(pointer);
for(i=0;i<count;i++)
        shmdt(tem_p[i]);
return rp;
/*
Sock_Pointer p=sock_ll.S_Head_pointer->next;
while(p!=NULL)
	{
	if(memcmp(p->cli_info.ip,ip,15)==0)
		{
		break;
		}
	p=p->next;
	}
return p;*/
}
/////////////////////////////////////////////
int rebuild_slist()
{
    sll_ptr p=(sll_ptr)get_ll_shmid(SOCK_LL_KEY,SOCK_LL_SIZE);
    Sock_Pointer pointer = (Sock_Pointer)shmat(p->Tail_shmid,NULL,0);
    while(pointer->prev_shmid!=0)
   {
        Sock_Pointer mp = (Sock_Pointer)shmat(pointer->prev_shmid,NULL,0);
	shmctl(mp->next_shmid,IPC_RMID,NULL);
	pointer=mp;
   }
   p->Tail_shmid= p->Head_shmid;
   p->count=0;
   pointer->next_shmid=0;
   pointer->prev_shmid=0;
   shmdt(p);
   shmdt(pointer);
   return 0;
}
////////////////////////////////////////////////
int slist_delete(char* ip)
{
        Sock_Pointer sp=slist_search_ip(ip);
	Sock_Pointer sp0=NULL;
	Sock_Pointer sp1=NULL;
	int id=0;
	if(sp!=NULL)
	{
	if(sp->prev_shmid!=0)sp0=shmat(sp->prev_shmid,NULL,0);
	if(sp->next_shmid!=0)sp1=shmat(sp->next_shmid,NULL,0);
	id=sp0->next_shmid;
	if(sp0!=NULL)
	sp0->next_shmid=sp->next_shmid;
	if(sp1!=NULL)
	sp1->prev_shmid=sp->prev_shmid;
	shmctl(id,IPC_RMID,NULL);
	}
	return 0;
/*
	Sock_Pointer sp=slist_search_ip(ip);
	sp->next->prev=sp->prev;
        sp->prev->next=sp->next;
	if(sp!=NULL)free(sp);
	sock_ll.count--;
*/
}
//////////////////////////////////////////////////////
//use linklist to store the sensor info
//////////////////////////////////////////////////////
// create_mlist
