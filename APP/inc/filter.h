#ifndef __filter_H
#define __filter_H
#include <public.h>
extern float angle, angle_dot; 	
float Kalman_Filter(float Accel,float Gyro);		
float Yijielvbo(float angle_m, float gyro_m);

#endif
