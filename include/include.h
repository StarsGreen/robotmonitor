//defined some func inerface for outer call

#ifndef __INCLUDE_H__
#define __INCLUDE_H_
#endif

//extern void move(short cmd_type,short angle,short vel);
/*
extern void* socket_thread(void);
extern void* move_thread(void);
extern void* video_send_thread(void);
extern void* video_get_thread(void);
*/

extern void *monitor_process(void);
extern void *cmd_process(void);
extern void *socket_process(void);
extern void *sensor_process(void);
