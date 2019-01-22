#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>
#include "data_config.h"
//#include "data_refer.h"
///////////////////////////////////////////
#define  BUFSIZE  40

///////////////////////////////////////////
float temper_read()
{
    float temp=0;
    int i, j;
    int fd;
    int ret;
    char buf[BUFSIZE];
    memset(buf,0,BUFSIZE);
    char tempBuf[5];
    fd = open("/sys/bus/w1/devices/28-0316b41b7eff/w1_slave", O_RDONLY);
    if(-1 == fd){
        perror("open device file error");
        return -1;
    }
    while(1){
        ret = read(fd, buf, BUFSIZE);  //将设备文件中的内容读入buf中
        if(0 == ret){                  //返回0表示读文件结束
            break;                     //跳出循环体
        }
        if(-1 == ret){                 //读文件出错
            if(errno == EINTR){        //出错是由中断引起的
                continue;              //继续读文件
            }
            perror("read()");          //打印出错
            close(fd);                 //关闭文件
            return 1;
        }
    }
    for(i=0;i<strlen(buf);i++){
        if(buf[i] == 't'){               //如果读到‘t’，说明后面马上出现温度值
            for(j=0;j<sizeof(tempBuf);j++){
                tempBuf[j] = buf[i+2+j]; //将温度值以字符串的形式写入tempBuf
            }
        }
    }
    temp = (float)atoi(tempBuf) / 1000;  //将字符串转换为浮点型温度数据
//    printf("%.3f C\n",temp);
	close(fd);
	return temp;
}
///////////////////////////////////////////
int read_i2c(int fd ,int addr)
{
	int result;
	result=wiringPiI2CReadReg8(fd,addr);
	return result;
}
//////////////////////////////////////////
void write_i2c(int fd ,int reg,int data)
{
	wiringPiI2CWriteReg8(fd,reg,data);
}
///////////////////////////////////////////
int init_mpu6050()
{
	int fd = wiringPiI2CSetup(DEVIIC_ID);
        if (fd < 0) {
                printf("Error setup I2C device %x\n", DEVIIC_ID);
                return -1;
        }
	write_i2c(fd,PWR_MGMT_1, 0x00);	//解除休眠状态
	write_i2c(fd,SMPLRT_DIV, 0x07);
	write_i2c(fd,CONFIG, 0x06);
	write_i2c(fd,GYRO_CONFIG, 0x00);
	write_i2c(fd,ACCEL_CONFIG, 0x01);
	return fd;
}
//////////////////////////////////////////
short get_data(int fd,unsigned char REG_Address)
{
	unsigned char H,L;
	H=read_i2c(fd,REG_Address);
	L=read_i2c(fd,REG_Address+1);
	return (H<<8)+L;
}
////////////////////////////////////////////
float xa_read(int fd)
{
short data=get_data(fd,GYRO_XOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return GYRO_RANGE*data/32768;
}
////////////////////////////////////////////
float ya_read(int fd)
{
short data=get_data(fd,GYRO_YOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return GYRO_RANGE*data/32768;
}
////////////////////////////////////////////
float za_read(int fd)
{
short data=get_data(fd,GYRO_ZOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return GYRO_RANGE*data/32768;
}
////////////////////////////////////////////
float xl_read(int fd)
{
short data=get_data(fd,ACCEL_XOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return ACCEL_RANGE*data/32768;
}

////////////////////////////////////////////
float yl_read(int fd)
{
short data=get_data(fd,ACCEL_YOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return ACCEL_RANGE*data/32768;
}

////////////////////////////////////////////
float zl_read(int fd)
{
short data=get_data(fd,ACCEL_ZOUT_H);
if(data&0x8000)
  {
  data=~(data-1);
  data=-data;
  }
return ACCEL_RANGE*data/32768;
}
//////////////////////////////////////////////
void init_dist_sensor(void)
{
	pinMode(4, OUTPUT);  // 设置4号引脚功能为输出
	pinMode(5, INPUT);  // 设置5号引脚功能为输入
	// 大循环不断测距
}
////////////////////////////////////////////
float dist_read(void)
{
	init_dist_sensor();
// 低高低是发射声波的信号，通过4号一脚发出
	digitalWrite(4, LOW);
	digitalWrite(4, HIGH);
	delayMicroseconds(10);
	digitalWrite(4, LOW);
// 通过5号引脚接收信号，判断超声波是否发出

	while(1){
	if (digitalRead(5) == 1)
// 如果是1，即高电平，表示超声波已发出
		break;  // 跳出循环
	}
	struct timeval t1;  // 结构体，可以记录秒和微秒两部分值
	gettimeofday(&t1, NULL);
 // 记录电平变高的时刻，即超声波发出时的时刻
//依然监听5引脚的信号，判断是否收到超声波信号
	while(1){
		if(digitalRead(5) == 0)
  // 如果是0，即低电平，表示超声波已收到
		break;  // 跳出循环
		}
	struct timeval t2;
	gettimeofday(&t2, NULL);
  // 记录电平变低的时刻，即收到反射信号时刻计算时间差
	long start, stop;//换算为微秒
	start = t1.tv_sec * 1000000 + t1.tv_usec; // 开始时刻
	stop = t2.tv_sec * 1000000 + t2.tv_usec;  // 结束时刻计算距离
	float dis=0;
	dis = (float)(stop - start)/1000000*340/2*100; //单位换算成cm
	return dis;
}
//////////////////////////////////////////
/*
void init_interface_pin()
{
wiringPiSetup();
}
*/
