#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
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
