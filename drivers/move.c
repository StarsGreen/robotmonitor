#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <wiringPi.h>
#include <softPwm.h>
#include "cmd.h"
#define ENA 3
#define ENB 25
#define IN1 21
#define IN2 22
#define IN3 23
#define IN4 24
#define BASE_REV 20
#define LOW_REV 16
#define HIGH_REV 20
#define leftwheel_clockwise(vel) do {\
digitalWrite(IN1,HIGH);\
digitalWrite(IN2,LOW);\
softPwmWrite(ENA,50);\
}while(0)
#define leftwheel_anti_clockwise(vel) do{\
digitalWrite(IN1,LOW);\
digitalWrite(IN2,HIGH);\
softPwmWrite(ENA,vel);\
}while(0)
#define rightwheel_clockwise(vel) do{\
digitalWrite(IN3,HIGH);\
digitalWrite(IN4,LOW);\
softPwmWrite(ENB,50);\
}while(0)
#define rightwheel_anti_clockwise(vel) do{\
digitalWrite(IN3,LOW);\
digitalWrite(IN4,HIGH);\
softPwmWrite(ENB,vel);\
}while(0)
#define move_stop do{\
rightwheel_anti_clockwise(0);\
leftwheel_anti_clockwise(0);\
}while(0)
////////////////////////////////////////////////////
struct move_interface
{
        int(*move_direct_right)(int angle,int vel);
        int(*move_up_right)(int angle,int vel);
        int(*move_direct_up)(int angle,int vel);
        int(*move_up_left)(int angle,int vel);
        int(*move_direct_left)(int angle,int vel);
        int(*move_down_left)(int angle,int vel);
        int(*move_direct_down)(int angle,int vel);
        int(*move_down_right)(int angle,int vel);
	int(*move_direct_stop)(void);
}move_action;
////////////////////////////////////////////////

////////////////////////////////////////////////
int move_direct_up(int angle,int vel)
{
int ang=angle-6*45;
printf("this is move direct up,a%dv%d\n",angle,vel);
if(vel==0)goto last;
//printf("move direct up ,angle %d ,vel %d\n",angle,vel);
//leftwheel_clockwise((int)(BASE_REV+HIGH_REV*ang/45*vel));
//rightwheel_clockwise((int)(BASE_REV+LOW_REV*ang/45*vel));
return 0;
last:move_stop;return 1;
}

int move_up_right(int angle,int vel)
{
int ang=angle-7*45;
if(vel==0)goto last;
//printf("move up right ,angle %d ,vel %d\n",angle,vel);
leftwheel_clockwise((int)(BASE_REV+HIGH_REV*ang/45*vel));
rightwheel_clockwise((int)(BASE_REV+LOW_REV*ang/45*vel));
return 0;
last:move_stop;return 1;

}

int move_direct_right(int angle,int vel)
{
//printf("move direct right ,angle %d ,vel %d\n",angle,vel);
int ang=45-angle;
if(vel==0)goto last;
if(angle!=0)
  {
leftwheel_anti_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
rightwheel_anti_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
  }
else
  {
leftwheel_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
rightwheel_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
  }
return 0;
last:move_stop;return 1;

}
/////////////////////////////////////////////////////////////
int move_down_right(int angle,int vel)
{
//printf("move down right ,angle %d ,vel %d\n",angle,vel);
int ang=45-(angle-1*45);
if(vel==0)goto last;
leftwheel_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
rightwheel_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
return 0;
last:move_stop;return 1;

}

int move_direct_down(int angle,int vel)
{
//printf("move direct down ,angle %d ,vel %d\n",angle,vel);
int ang=angle-2*45;
if(vel==0)goto last;
leftwheel_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
rightwheel_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
return 0;
last:move_stop;return 1;
}

int move_down_left(int angle,int vel)
{
//printf("move down left ,angle %d ,vel %d\n",angle,vel);
int ang=angle-3*45;
if(vel==0)goto last;
rightwheel_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
leftwheel_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
return 0;
last:move_stop;return 1;
}

int move_direct_left(int angle,int vel)
{
//printf("move direct left ,angle %d ,vel %d\n",angle,vel);
int ang=45-(angle-4*45);
if(vel==0)goto last;
rightwheel_clockwise((int)(BASE_REV+HIGH_REV*(ang+45)/45*vel));
leftwheel_clockwise((int)(BASE_REV+LOW_REV*(ang+45)/45*vel));
return 0;
last:move_stop;return 1;
}

int move_up_left(int angle,int vel)
{
//printf("move up left ,angle %d ,vel %d\n",angle,vel);
int ang=45-(angle-5*45);
if(vel==0)goto last;
rightwheel_clockwise((int)(BASE_REV+HIGH_REV*ang/45*vel));
leftwheel_clockwise((int)(BASE_REV+LOW_REV*ang/45*vel));
return 0;
last:move_stop;return 1;
}

int move_direct_stop(void)
{
rightwheel_clockwise(0);
leftwheel_clockwise(0);
//printf("move direct stop\n");
}
////////////////////////////////////////////
void setup_pin()
{
//pinMode(ENA,OUTPUT);
pinMode(IN1,OUTPUT);
pinMode(IN2,OUTPUT);
pinMode(IN3,OUTPUT);
pinMode(IN4,OUTPUT);
//pinMode(ENB,OUTPUT);
//digitalWrite(ENA, LOW);
softPwmCreate(ENB,LOW,100);
softPwmCreate(ENA,LOW,100);
digitalWrite(IN1, HIGH);
digitalWrite(IN2, HIGH);
digitalWrite(IN3, HIGH);
digitalWrite(IN4, LOW);
//digitalWrite(ENB, LOW);
}
////////////////////////////////////////////
void move_init(void)
{
	setup_pin();
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
