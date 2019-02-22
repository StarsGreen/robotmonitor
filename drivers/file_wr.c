#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "data_structure.h"
////////////////////////////////////////////////
int read_file_to_buff(char* filename,unsigned char* file_buff)
{

	struct stat file_info;
        stat(filename,&file_info);

	int file_size=file_info.st_size;
//        printf("the file is %d byte",file_size);
        FILE* fp;
        fp=fopen(filename,"r");
        if(fp==NULL)
	{
        printf("file is not exist");
	exit(1);
	}
	fread(file_buff,1,file_size,fp);
	fclose(fp);

	return file_size;
}
/////////////////////////////////////////////////////
////////////////////////////////////////////////
int read_buff_to_file(char* filename,unsigned char* file_buff,unsigned int buff_size)
{

	struct stat file_info;
//        stat(filename,&file_info);

//	int file_size=file_info.st_size;
//        printf("the file is %d byte",file_size);
        FILE* fp;
        fp=fopen(filename,"wb+");
        if(fp==NULL)
	{
        printf("file is not exist and can not be created");
	exit(1);
	}
	fwrite(file_buff,1,buff_size,fp);
	fclose(fp);

        stat(filename,&file_info);
        int file_size=file_info.st_size;

	return file_size;
}
///////////////////////////////////////////////////////////
int read_accel_to_file(mn_ptr ptr,char* filename)
{
      FILE* fp;
        fp=fopen(filename,"ab+");
        if(fp==NULL)
        {
        printf("file is not exist and can not be created");
        exit(1);
        }
        fprintf(fp,"xl_accel: %7.5f   ",ptr->accel_info.xl_accel);
        fprintf(fp,"yl_accel: %7.5f   ",ptr->accel_info.yl_accel);
        fprintf(fp,"zl_accel: %7.5f\n ",ptr->accel_info.zl_accel);
return 0;
}
/////////////////////////////////////////////////////
int read_angle_vel_to_file(mn_ptr ptr,char* filename)
{
        FILE* fp;
        fp=fopen(filename,"ab+");
        if(fp==NULL)
        {
        printf("file is not exist and can not be created");
        exit(1);
        }
        fprintf(fp,"xa_vel: %7.5f   ",ptr->vel_info.xa_vel);
        fprintf(fp,"ya_vel: %7.5f   ",ptr->vel_info.ya_vel);
        fprintf(fp,"za_vel: %7.5f\n",ptr->vel_info.za_vel);
return 0;
}
///////////////////////////////////////////////////////////////
int read_value_to_file(mn_ptr ptr,char* filename)
{
	struct stat file_info;
//        float value =32.2f;
        FILE* fp;
        fp=fopen(filename,"ab+");
        if(fp==NULL)
	{
        printf("file is not exist and can not be created");
	exit(1);
	}
//	fwrite(ptr,size,1,fp);
//	fwrite(&value,sizeof(value),1,fp);
        fprintf(fp,"xl_accel: %7.5f   ",ptr->accel_info.xl_accel);
        fprintf(fp,"yl_accel: %7.5f   ",ptr->accel_info.yl_accel);
        fprintf(fp,"zl_accel: %7.5f\n",ptr->accel_info.zl_accel);

        fprintf(fp,"xl_vel: %7.5f   ",ptr->vel_info.xl_vel);
        fprintf(fp,"yl_vel: %7.5f   ",ptr->vel_info.yl_vel);
        fprintf(fp,"zl_vel: %7.5f\n",ptr->vel_info.zl_vel);

        fprintf(fp,"xl: %7.5f   ",ptr->jour_info.xl);
        fprintf(fp,"yl: %7.5f   ",ptr->jour_info.yl);
        fprintf(fp,"zl: %7.5f\n",ptr->jour_info.zl);

        fprintf(fp,"xa_vel: %7.5f   ",ptr->vel_info.xa_vel);
        fprintf(fp,"ya_vel: %7.5f   ",ptr->vel_info.ya_vel);
        fprintf(fp,"za_vel: %7.5f\n",ptr->vel_info.za_vel);

        fprintf(fp,"xa: %7.5f   ",ptr->jour_info.xa);
        fprintf(fp,"ya: %7.5f   ",ptr->jour_info.ya);
        fprintf(fp,"za: %7.5f\n\n",ptr->jour_info.za);

	fclose(fp);

       read_accel_to_file(ptr,"accel_data");

       read_angle_vel_to_file(ptr,"angle_vel_data");

        stat(filename,&file_info);
        int file_size=file_info.st_size;
	return file_size;
}

