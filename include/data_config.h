#define MYPORT  50000
#define QUEUE   4
#define BUFFER_SIZE 1024
#define ARRAY_SIZE 1024

#define    MAXLINE        1024*1024
#define    PORT         51000
#define    BROADCAST_IP "224.0.1.100"
#define    AIMED_IP     "192.168.1.12"
#define    BACKLOG      4
#define    FILENAME     "video.jpg"

#define SMPLRT_DIV      0x19
//陀螺仪采样率，典型值：0x07(125Hz)
#define CONFIG          0x1A
//低通滤波频率，典型值：0x06(5Hz)
#define GYRO_CONFIG     0xe0  //陀螺仪开启自检，量程为正负250deg/s
//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG    0xe1//加速度计开启自检，量程为正负2gdeg/s
//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define ACCEL_RANGE     2*9.8

#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40

#define TEMP_OUT_H              0x41
#define TEMP_OUT_L              0x42
#define GYRO_RANGE      	250.0f
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48

#define PWR_MGMT_1              0x6B    //电源管理，典型值：0x00(正常启用)
#define WHO_AM_I                0x75    //IIC地址寄存器(默认数值0x68，只读)

//IIC写入时的地址字节数据，+1为读取
#define   DEVIIC_ID     0x68

#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 240
#define QUALITY 80


#define M_NODE_SIZE sizeof(M_Node)//motion structure size
#define S_NODE_SIZE sizeof(Sock_Node)//motion structure size
#define MAX_NODE_NUM 200


#define MOVE_LL_KEY 1004
#define SOCK_LL_KEY 1001
#define CTRL_CMD_KEY 1002
#define MOVE_CMD_KEY 1003
#define MOVE_INFO_KEY 1005

#define MOVE_LL_SIZE sizeof(mll)
#define SOCK_LL_SIZE sizeof(sll)

#define CTRL_CMD_SIZE sizeof(Ctrl_Cmd)
#define CMD_INFO_SIZE sizeof(Cmd_Info)
#define MOVE_CMD_SIZE sizeof(move_cmd)
#define MOVE_INFO_SIZE sizeof(motion_node)


#define ST 0.01f //sensor sample tima
#define GRAVITY 9.80f

#define  ACCEL_MAX_LIMITS 2.0f
#define  ACCEL_MIN_LIMITS 0.1f

#define  ANGLE_MAX_LIMITS 90.0f
#define  ANGLE_MIN_LIMITS 1.0f

#define XL_ACCEL_FACTOR 1.0f
#define YL_ACCEL_FACTOR 1.0f
#define ZL_ACCEL_FACTOR 1.0f

#define XA_VEL_FACTOR 1.0f
#define YA_VEL_FACTOR 1.0f
#define ZA_VEL_FACTOR 1.0f


#define SHOW_LINEAR_ACCEL 0

#define SHOW_ANGLE_VEL 0

#define SHOW_VALID_LINEAR_ACCEL 0

#define SHOW_VALID_ANGLE_VEL 1

#define SHOW_VALID_MOVE_INFO 0

#define ACCEL_WINDOW 0.5
#define ANGLE_WINDOW 35

