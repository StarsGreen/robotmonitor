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

///////////////////////////////////////////////

/////////////////////////////////////////////////
float kalman_filter(float last_result,float last_value,float cur_value,
float time,float* p_next,float Q_offset,float R_offset)
{
	float result=0;
	float pos_est=last_result+last_value*last_value*time;
	float p_pos_est=*p_next+Q_offset;
	float kg=p_pos_est/(p_pos_est+Q_offset);
	float pos_mes=last_result+cur_value*cur_value*time;
	float pos_best=last_result+kg*(pos_mes-pos_est);
	*p_next=(1-kg)*p_pos_est;
	result=pos_best;
/*switch(flag)
{
	case 0:
	{
	float v_est=last_result+last_value*time;
	float p_est=p_est+Q_offset;
	float kg=p_vel_est/(p_vel_est+Q_offset);
	float v_mes=last_result+cur_value*time;
	float v_best=last_result+kg*(v_mes-v_est);
	float p_best=(1-kg)*p_vel_est;
	result=v_best;
	}break;
	case 1:
	{
	float pos_est=last_result+last_value*last_value*time;
	static float p_pos_est=P_conv+Q_offset;
	float kg=p_pos_est/(p_pos_est+Q_offset);
	float pos_mes=last_result+cur_value*cur_value*time;
	float pos_best=last_result+kg*(pos_mes-pos_est);
	p_pos_est=(1-kg)*p_pos_est;
	result=pos_best;
	}break;
	default:break;
}
*/
return result;
}
