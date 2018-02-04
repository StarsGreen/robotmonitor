#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <setjmp.h>
#include "jpeglib.h"
#include "global_data.h"

static void yuv422_to_rgb24(unsigned char *yuv422,unsigned char *rgb24,
int width, int height)
{
    int x=0,y=0;  
    unsigned char *yuv444;  
    yuv444=(unsigned char *)malloc(sizeof(unsigned char)*width*height*3);  
    for(x = 0;x < width*height*2;x+=4)  
    {
        yuv444[y] = yuv422[x];  
        yuv444[y+1] = yuv422[x+1];  
        yuv444[y+2] = yuv422[x+3];  
        yuv444[y+3] = yuv422[x+2];  
        yuv444[y+4] = yuv422[x+1];  
        yuv444[y+5] = yuv422[x+3];
/*
        yuv444[y] = yuv422[y]*0.8588+16;  
        yuv444[y+1] = yuv422[y+1]*0.8784+16;  
        yuv444[y+2] = yuv422[y+2]*0.8784+16;  
        yuv444[y+3] = yuv422[y+3]*0.8588+16;  
        yuv444[y+4] = yuv422[y+4]*0.8784+16;  
        yuv444[y+5] = yuv422[y+5]*0.8784+16;
*/
	y=y+6;
    }
    for(x = 0;x < width*height*3;x+=3)  
    {  
        rgb24[x+2] = yuv444[x] + 1.779*(yuv444[x+1] - 128);  
        rgb24[x+1] = yuv444[x]-0.3455*(yuv444[x+1]-128)-0.7169*(yuv444[x+2]-128);  
        rgb24[x] = yuv444[x] + 1.4075*(yuv444[x+2] - 128);  
       

/*	rgb24[x+2] = yuv444[x]*1.164 + 2.017*yuv444[x+1] -276.8;  
     rgb24[x+1] = yuv444[x]*1.164-0.392*yuv444[x+1]-0.823*yuv444[x+2]+135.6;  
        rgb24[x] = yuv444[x]*1.164 + 1.596*yuv444[x+2] - 222.9;  
  */     
	 if (rgb24[x]>255)rgb24[x]=255;  
        if (rgb24[x]<0)rgb24[x]=0;  
        if (rgb24[x+1]>255)rgb24[x+1]=255;  
        if (rgb24[x+1]<0)rgb24[x+1]=0;  
        if (rgb24[x+2]>255)rgb24[x+2]=255;  
        if (rgb24[x+2]<0)rgb24[x+2]=0;  
    }
    free(yuv444);
}
//////////////////////////////////////////////////////
static void rgb24_to_jpeg(unsigned char *rgb24,unsigned char** j_buff,
unsigned long *j_size,int width, int height,int quality )
{
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
 
//  FILE * outfile;	/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
  int row_stride;		/* physical row width in image buffer */

  cinfo.err = jpeg_std_error(&jerr);
  /* Now we can initialize the JPEG compression object. */
   jpeg_create_compress(&cinfo);



   unsigned char* image_buff=NULL;
   unsigned long image_size=0;
   jpeg_mem_dest(&cinfo,&image_buff,&image_size);	




//  if ((outfile = fopen(FILENAME, "wb")) == NULL) {
//    fprintf(stderr, "can't open %s\n", FILENAME);
//    exit(1);
//	}
//  jpeg_stdio_dest(&cinfo, outfile);

  cinfo.image_width = width; 	/* image width and height, in pixels */
  cinfo.image_height = height;
  cinfo.input_components = 3;		/* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	/* colorspace of input image */

  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  jpeg_start_compress(&cinfo, TRUE);

  row_stride = width * 3;


  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = & rgb24[cinfo.next_scanline * row_stride];
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
//  fclose(outfile);

    if(NULL!=*j_buff)
        free(*j_buff);
    *j_buff=(unsigned char*)malloc(image_size);
 
    memset(*j_buff,0,image_size);
    memcpy(*j_buff,image_buff,image_size);

//printf("image size is %ld",image_size);
    free(image_buff);

    *j_size=image_size;
  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);

}

////////////////////////////////////////////////////////
int yuyv_to_jpeg(unsigned char* yuv422,unsigned char**jpeg_buff,
unsigned long *jpeg_size,int width, int height,int quality)
{	
	unsigned char* rgb24=(unsigned char *)malloc(width*height*3);
 	yuv422_to_rgb24(yuv422,rgb24,width,height);
 
	rgb24_to_jpeg(rgb24,jpeg_buff,jpeg_size,width,height,quality);

 	free(rgb24);

	return 0;
}
