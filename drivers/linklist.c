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
//////////////////////////////////////////////////////
//use linklist to store the sensor info
//////////////////////////////////////////////////////

ml_ptr ml_p;//define the linklist info structure

/*init the linklist by creating a single node as a guard*/

int init_mlist(ml_ptr ml_p)
{
  mn_ptr p_head=(mn_ptr)malloc(sizeof(motion_node));
  if (NULL ==p_head)
    {
        printf("内存分配失败！\n");
        exit(0);
    }
   memset(p_head,0,sizeof(motion_node));
   ml_p->head_ptr=p_head;
   ml_p->tail_ptr=p_head;
   ml_p->count=0;
  return 0;
}

/*add node containing the snesor info to the linklist */

int mlist_add_node(mn_ptr mn_p,ml_ptr ml_p)
{
  mn_ptr p;
  mn_ptr ptr=(mn_ptr)malloc(sizeof(motion_node));
  if (NULL ==ptr)
    {
        printf("内存分配失败！\n");
        exit(0);
    }

  memcpy(ptr,mn_p,sizeof(motion_node));

  p=ml_p->tail_ptr;

  p->next=ptr;
  ptr->prev=p;
  ptr->next=NULL;
  ptr->num=ml_p->count+1;

  ml_p->tail_ptr=ptr;
  ml_p->count++;


  return 0;
}

/*destroy all linklist by destroy all node including the node as a guard */

int destroy_mlist(ml_ptr ml_p)
{
  mn_ptr p,temp;
  p=ml_p->head_ptr;
  while(p)
  {
    temp=p;
    p=p->next;
    free(temp);
  }

  ml_p->head_ptr=NULL;
  ml_p->tail_ptr=NULL;
  ml_p->count=0;

  return 0;
}

/*destroy all linklist by destroy all nodebut not including the node as  guard */

int clear_mlist(ml_ptr ml_p)
{
  mn_ptr p,temp;
  p=ml_p->head_ptr;
  if(p->next)
    p=p->next;
  while(p)
  {
    temp=p;
    p=p->next;
    free(temp);
  }

  ml_p->tail_ptr= ml_p->head_ptr;
  ml_p->count=0;

  return 0;
}
