#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>



#define  BUFSIZE  128
///////////////////////////////////////////
int read_temper()
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
    temp = (int)atoi(tempBuf) / 100;  //将字符串转换为浮点型温度数据
//    printf("%.3f C\n",temp);
    close(fd);
	return temp;
}
////////////////////////////////////////////
int accel_read()
{





}
////////////////////////////////////////////
int dist_read()
{






}

