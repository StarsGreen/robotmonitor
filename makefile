TOPDIR=$(shell pwd)
INCDIR=$(TOPDIR)/include
COMPILE=gcc
OBJ=robot
DEP= $(TOPDIR)/common/socket_process.c $(TOPDIR)/common/video_get_thread.c\
	 $(TOPDIR)/common/monitor_process.c\
	$(TOPDIR)/common/video_broadcast_thread.c\
	$(TOPDIR)/common/move_thread.c $(TOPDIR)/common/cmd_process.c\
	$(TOPDIR)/common/cmd.c $(TOPDIR)/common/env.c\
	$(TOPDIR)/drivers/move.c  $(TOPDIR)/drivers/yuyv_to_jpeg.c\
	$(TOPDIR)/drivers/file_wr.c main.c

all :
	$(COMPILE) -Wall $(DEP) -o $(OBJ) -lpthread -ljpeg -g -I $(INCDIR)
clean:
	rm -rf *.o Robot
