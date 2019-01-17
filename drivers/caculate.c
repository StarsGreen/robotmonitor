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
return result;
}
///////////////////////////////////////////////
float slide_filter(float cur_value,float last_value)
{
float rate=fabs((cur_value-last_value)/last_value);
if(rate<0.3)
	return  0.6*cur_value+0.4*last_value;
else return last_value;
}

#define Kp 100.0f                // 比例增益支配率收敛到加速度计/磁强计

#define Ki 0.002f                // 积分增益支配率的陀螺仪偏见的衔接

#define halfT 0.001f             // 采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;      // 四元数的元素，代表估计方向

float exInt = 0, eyInt = 0, ezInt = 0;     // 按比例缩小积分误差

float Yaw,Pitch,Roll;  //偏航角，俯仰角，翻滚角

typedef struct
{
  float pitch;
  float roll;
  float yaw;
}pos_info;

void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az,
pos_info* p_info)
{
        float norm;
        float vx, vy, vz;
        float ex, ey, ez;
        float pitch=0,roll=0,yaw=0;
        // 测量正常化

        norm = sqrt(ax*ax + ay*ay + az*az);

        ax = ax / norm;                   //单位化

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

        pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch ,转换为度数

        roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // rollv

        yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;  //此处没有价值，注掉

        p_info->pitch=pitch;

        p_info->roll=roll;

        p_info->yaw=yaw;
}
///////////////////////////////////////////////////
#define Q 0.018f //measurement variance
#define R 0.5F   //gauss variance
typedef struct
{
 float last_value;
 float mid_value;
 float now_value;
 float last_var;
 float mid_var;
 float now_var;
 float last_kg;
 float mid_kg;
 float now_kg;
}kalman_params;

static kalman_params k_params={0,0,0,0,0,0,0,0,0};
///////////////////////////////////////////////////
void init_kalman_params(kalman_params* kp)
{
  kp->last_value=0;
  kp->last_var=0.02;
  kp->last_kg=0;
}

float f_order_kalman_filter(float last_result,float last_value,float cur_value)
{





}
