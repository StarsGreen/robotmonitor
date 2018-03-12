#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/time.h>

///////////////////////////////////////////
#define  BUFSIZE  128

///////////////////////////////////////////
float temper_read()
{
    float temp;
    int i, j;
    int fd;
    int ret;
    char buf[BUFSIZE];
    char tempBuf[5];
    fd = open("/sys/bus/w1/devices/28-00000495db35/w1_slave", O_RDONLY);
    if(-1 == fd){
        perror("open device file error");
        return 1;
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
    for(i=0;i<sizeof(buf);i++){
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
int read_i2c(int fd ,int data)
{
	int result;
	result=wiringPiI2CReadReg8(fd,date);
	return result;
}
//////////////////////////////////////////
void write_i2c(int fd ,int,reg,int data)
{
	wiringPiI2CWriteReg8(fd,reg,data);
}
///////////////////////////////////////////
int init_MPU6050()
{
	int fd = wiringPiI2CSetup(DEVIIC_ID);
        if (fd < 0) {
                printf("Error setup I2C device %b\n", devId);
                exit(1);
        }
	write_i2c(fd,PWR_MGMT_1, 0x00);	//解除休眠状态
	write_i2c(fd,SMPLRT_DIV, 0x07);
	write_i2c(fd,CONFIG, 0x06);
	write_i2c(fd,GYRO_CONFIG, 0x18);
	write_i2c(fd,ACCEL_CONFIG, 0x01);

	return fd;
}
////////////////////////////////////////////
int get_data(int fd,unsigned char REG_Address)
{
	unsigned char H,L;
	H=read_i2c(fd,REG_Address);
	L=read_i2c(fd,REG_Address+1);
	return ((H<<8)+L);   //合成数据
}
////////////////////////////////////////////
float xa_read(int fd,int reg)
{



}
////////////////////////////////////////////

////////////////////////////////////////////
float xa_read()
{





}
////////////////////////////////////////////
float ya_read()
{
}
////////////////////////////////////////////
float za_read()
{
}

////////////////////////////////////////////
float xl_read()
{
}

////////////////////////////////////////////
float yl_read()
{
}

////////////////////////////////////////////
float zl_read()
{
}
////////////////////////////////////////////
float dist_read()
{
// 低高低是发射声波的信号，通过15号一脚发出
digitalWrite(15, LOW);
digitalWrite(15, HIGH);
delayMicroseconds(10);
digitalWrite(15, LOW);// 通过16号引脚接收信号，判断超声波是否发出
while(1){
if (digitalRead(16) == 1){ // 如果是1，即高电平，表示超声波已发出
	break;  // 跳出循环
}
struct timeval t1;  // 结构体，可以记录秒和微秒两部分值
gettimeofday(&t1, NULL);  // 记录电平变高的时刻，即超声波发出时的时刻
//依然监听16引脚的信号，判断是否收到超声波信号
while(1){
	if(digitalRead(16) == 0){  // 如果是0，即低电平，表示超声波已收到
	break;  // 跳出循环
	}
	}
struct timeval t2;
gettimeofday(&t2, NULL);  // 记录电平变低的时刻，即收到反射信号时刻计算时间差
long start, stop;//换算为微秒
start = t1.tv_sec * 1000000 + t1.tv_usec; // 开始时刻
stop = t2.tv_sec * 1000000 + t2.tv_usec;  // 结束时刻计算距离
float dis;
dis = (float)(stop - start) / 1000000 * 340 / 2 * 100; //单位换算成cm
// 一直循环，每隔2s进行一次测距
	return dis;
}
//////////////////////////////////////////
void init_sensor()
{
	wiringPiSetup();  // 初始化库
	pinMode(15, OUTPUT);  // 设置15号引脚功能为输出
	pinMode(16, INPUT);  // 设置16号引脚功能为输入
	// 大循环不断测距





}
