TOPDIR=$(shell pwd)
INCDIR=$(TOPDIR)/include
COMPILE=gcc
OBJ=robot
DEP=$(TOPDIR)/common/socket_process.c\
$(TOPDIR)/common/info_con_thread.c\
$(TOPDIR)/common/monitor_process.c\
$(TOPDIR)/common/video_get_thread.c\
$(TOPDIR)/common/video_broadcast_thread.c\
$(TOPDIR)/common/move_ctl_thread.c\
$(TOPDIR)/common/sensor_process.c\
$(TOPDIR)/common/info_get_thread.c\
$(TOPDIR)/common/cmd_process.c\
$(TOPDIR)/common/cmd.c\
$(TOPDIR)/common/env.c\
$(TOPDIR)/drivers/yuyv_to_jpeg.c\
$(TOPDIR)/drivers/file_wr.c $(TOPDIR)/drivers/caculate.c\
$(TOPDIR)/drivers/linklist.c $(TOPDIR)/drivers/sensor_read.c\
$(TOPDIR)/drivers/move.c main.c
all :
	$(COMPILE) -Wall $(DEP) -o $(OBJ) -lpthread -lm -ljpeg -lwiringPi -g -I $(INCDIR)
clean:
	rm -rf *.o Robot
