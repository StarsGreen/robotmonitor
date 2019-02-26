#include  <unistd.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
//#include "global_data.h"
#include <semaphore.h>
//#include "include.h"
#include <signal.h>
#include <math.h>

#include "data_structure.h"
#include "data_config.h"

///////////////////////////////////////////////

/////////////////////////////////////////////////

/*
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
return result;
}
*/
///////////////////////////////////////////////
float slide_filter(float cur_value,float last_value,int flag)
{
float rate=fabs(cur_value-last_value);
if(flag==0)
{
  if(rate<ACCEL_WINDOW)
	return 0.8*cur_value+last_value*0.2;
  else return last_value;
}
else
{
  if(rate<ANGLE_WINDOW)
        return 0.8*cur_value+last_value*0.2;
  else return last_value;
}
}

#define Kp 10.0f                // 比例增益支配率收敛到加速度计/磁强计
#define Ki 0.002f                // 积分增益支配率的陀螺仪偏见的衔接
#define halfT (0.5*ST)             // 采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;      // 四元数的元素，代表估计方向
float exInt = 0, eyInt = 0, ezInt = 0;     // 按比例缩小积分误差

//float Yaw,Pitch,Roll;  //偏航角，俯仰角，翻滚角

int mems_fusion(float gx, float gy, float gz, float ax, float ay, float az,
posture_info* p_info)
{
        float norm;
        float vx, vy, vz;
        float ex, ey, ez;
        float pitch=0,roll=0,yaw=0;

        // 测量正常化
        norm = sqrt(ax*ax + ay*ay + az*az);

        //单位化
        ax = ax / norm;
        ay = ay / norm;
        az = az / norm;

        // 估计方向的重力
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

        // 错误的领域和方向传感器测量参考方向之间的交叉乘积的总和
        ex = (ay*vz - az*vy);
        ey = (az*vx - ax*vz);
        ez = (ax*vy - ay*vx);

        // 积分误差比例积分增益
        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;

        // 调整后的陀螺仪测量
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;

        // 整合四元数率和正常化
        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

        // 正常化四元
        norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 = q0 / norm;
        q1 = q1 / norm;
        q2 = q2 / norm;
        q3 = q3 / norm;

        pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
        roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv
        yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;

        p_info->pitch=pitch;
        p_info->roll=roll;
        p_info->yaw=yaw;

  return 0;
}
///////////////////////////////////////////////////
#define Q 0.018f //measurement variance
#define R 0.5f   //gauss variance

typedef struct
{
 float last_result;//last valid value
 float last_var;  //last valid variance
// float last_kg;   //last gain
}kalman_params;

kalman_params kalman_paramters[6];
///////////////////////////////////////////////////
int init_kalman_params()
{
//  extern motion_node m_node;
  extern ml_ptr ml_p;
  mn_ptr mn_p=ml_p->tail_ptr;

  kalman_params* k_params=kalman_paramters;

  k_params->last_result=mn_p->jour_info.xl;
  k_params->last_var=0.02;
  //k_params->last_kg=0;
  k_params++;

  k_params->last_result=mn_p->jour_info.yl;
  k_params->last_var=0.02;
  //k_params->last_kg=0;
  k_params++;

  k_params->last_result=mn_p->jour_info.zl;
  k_params->last_var=0.02;
  //k_params->last_kg=0;
  k_params++;

  k_params->last_result=mn_p->jour_info.xa;
  k_params->last_var=0.02;
  //k_params->last_kg=0;
  k_params++;

  k_params->last_result=mn_p->jour_info.ya;
  k_params->last_var=0.02;
  //k_params->last_kg=0;
  k_params++;

  k_params->last_result=mn_p->jour_info.za;
  k_params->last_var=0.02;
  //k_params->last_kg=0;

  return 0;

}

int kalman_filter(mn_ptr last_node_ptr,mn_ptr cur_node_ptr)
{

  kalman_params* k_params=kalman_paramters;

  float last_value=last_node_ptr->accel_info.xl_accel;
  float cur_value=cur_node_ptr->accel_info.xl_accel;
  float last_vel=last_node_ptr->vel_info.xl_vel;
  /*1.this step is used to get xl value by kalman filter*/

  //estimate value
  float value_est=k_params->last_result+last_vel*ST+0.5*last_value*ST*ST;
  //estimate variance
  float p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  float kg=p_est/(p_est+R);
  //measure value
  float value_mes=k_params->last_result+last_vel*ST+0.5*cur_value*ST*ST;
  //real/best value
  float value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  float p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;

  cur_node_ptr->jour_info.xl=value_real;

  k_params++;

////////////////////////////////////////////////////////
  last_value=last_node_ptr->accel_info.yl_accel;
  cur_value=cur_node_ptr->accel_info.yl_accel;
  last_vel=last_node_ptr->vel_info.yl_vel;

  /*2.this step is used to get yl value by kalman filter*/

  //estimate value
  value_est=k_params->last_result+last_vel*ST+0.5*last_value*ST*ST;
  //estimate variance
  p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  kg=p_est/(p_est+R);
  //measure value
   value_mes=k_params->last_result+0.5*cur_value*ST*ST;
  //real/best value
  value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;
  cur_node_ptr->jour_info.yl=value_real;

  k_params++;

/////////////////////////////////////////////////////

  last_value=last_node_ptr->accel_info.zl_accel;
  cur_value=cur_node_ptr->accel_info.zl_accel;
  last_vel=last_node_ptr->vel_info.zl_vel;

  /*3.this step is used to get zl value by kalman filter*/

  //estimate value
  value_est=k_params->last_result+last_vel*ST+0.5*last_value*ST*ST;
  //estimate variance
  p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  kg=p_est/(p_est+R);
  //measure value
  value_mes=k_params->last_result+last_vel*ST+0.5*cur_value*ST*ST;
  //real/best value
  value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;
  cur_node_ptr->jour_info.zl=value_real;

  k_params++;

//////////////////////////////////////////////////////

  last_value=last_node_ptr->vel_info.xa_vel;
  cur_value=cur_node_ptr->vel_info.xa_vel;

  /*4.this step is used to get xa value by kalman filter*/

  //estimate value 

  value_est=k_params->last_result+last_value*ST;
  //estimate variance
  p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  kg=p_est/(p_est+R);
  //measure value
  value_mes=k_params->last_result+cur_value*ST;
  //real/best value
  value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;
  cur_node_ptr->jour_info.xa=value_real;

  k_params++;

//////////////////////////////////////////////////////

  last_value=last_node_ptr->vel_info.ya_vel;
  cur_value=cur_node_ptr->vel_info.ya_vel;

  /*5.this step is used to get ya value by kalman filter*/

  //estimate value 
  value_est=k_params->last_result+last_value*ST;
  //estimate variance
  p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  kg=p_est/(p_est+R);
  //measure value
  value_mes=k_params->last_result+cur_value*ST;
  //real/best value
  value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;
  cur_node_ptr->jour_info.ya=value_real;

  k_params++;
//////////////////////////////////////////////////////

  last_value=last_node_ptr->vel_info.za_vel;
  cur_value=cur_node_ptr->vel_info.za_vel;

  /*6.this step is used to get ya value by kalman filter*/

  //estimate value 
  value_est=k_params->last_result+last_value*ST;
  //estimate variance
  p_est=k_params->last_var;
  p_est=p_est+Q;
  //caculate kalman gain
  kg=p_est/(p_est+R);
  //measure value
  value_mes=k_params->last_result+cur_value*ST;
  //real/best value
  value_real=value_est+kg*(value_mes-value_est);
  //fresh the variance
  p_new=(1-kg)*p_est;

  k_params->last_result=value_real;
  k_params->last_var=p_new;
  cur_node_ptr->jour_info.za=value_real;

  return 0;
}
///////////////////////////////////////
/*由方程导出的函数在此，返回计算值*/
float fun(float x,float ma,float mb,float mc)
{
 float func_x =3*pow(x,2)+2*x*(ma+mc-2*mb)+
  pow((ma-mb),2)+pow((mc-mb),2)-pow(GRAVITY,2);
  return func_x;
}
//////////////////////////////////////
float diff_fun(float x,float ma,float mb,float mc)
{
 float func_x =6*x+2*(ma+mc-2*mb);
 return func_x;
}

///////////////////////////////////////
#define TOL 0.0001  //tolerance
float solve_noliner_equation(float ma,float mb,float mc)
{
  printf("the ma is %6.5f,the mb is %6.5f,the mc is %6.5f\n",ma,mb,mc);
  unsigned int c_times = 0;
  float c_result;
  float p=0.04;
  for(c_times=1;c_times < 1000;c_times++)
  {
    c_result = p - fun(p,ma,mb,mc)/diff_fun(p,ma,mb,mc);
    if (fabs(p - c_result) <TOL)
      break;
    p = c_result;
    if (c_times == 999)
    {
       printf("计算方法失效！");
       break;
    }
  }
  printf("the sovelution is %6.5f\n",c_result);
  return c_result;
}
///////////////////////////////////////
#define JUDGE_WITH_ZERO(x) (x>0?1:-1)
////////////////////////////////////////
int sensor_check(motion_node* mn,sensor_offset* so)
{
  float ma0=JUDGE_WITH_ZERO(mn->accel_info.xl_accel);
  float mb0=JUDGE_WITH_ZERO(mn->accel_info.yl_accel);
  float mc0=JUDGE_WITH_ZERO(mn->accel_info.zl_accel);

  float ma=fabs(mn->accel_info.xl_accel);
  float mb=fabs(mn->accel_info.yl_accel);
  float mc=fabs(mn->accel_info.zl_accel);
  float gra_y=solve_noliner_equation(ma,mb,mc);
  float gra_x=gra_y+ma-mb;
  float gra_z=gra_y+mc-mb;
  float zero_offset=mb-gra_y;

  so->gra_cpt_info.gra_x=gra_x*ma0;
  so->gra_cpt_info.gra_y=gra_y*mb0;
  so->gra_cpt_info.gra_z=gra_z*mc0;

  so->xa_vel_offset=mn->vel_info.xa_vel*XA_VEL_FACTOR;
  so->ya_vel_offset=mn->vel_info.ya_vel*YA_VEL_FACTOR;
  so->za_vel_offset=mn->vel_info.za_vel*ZA_VEL_FACTOR;

  so->xl_accel_offset=zero_offset*XL_ACCEL_FACTOR;
  so->yl_accel_offset=zero_offset*YL_ACCEL_FACTOR;
  so->zl_accel_offset=zero_offset*ZL_ACCEL_FACTOR;

  so->sensor_zero_shift=zero_offset;


  return 0;
}

//////////////////////////////////////////
float get_real_value(float value,float offset)
{
  float zero_offset=fabs(offset);
  float rtn_value;
/*
  if(value<0)
    rtn_value=value+zero_offset;
  else
*/
    rtn_value=value-zero_offset;
  return rtn_value;
}
//////////////////////////////////////////
float range_limit(float value,float min_range,float max_range)
{
if(fabs(value) > max_range || fabs(value) < min_range)
  return 0;
return value;
}
