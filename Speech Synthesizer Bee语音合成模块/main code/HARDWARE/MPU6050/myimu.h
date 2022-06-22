#ifndef __MYIMU_H
#define __MYIMU_H
#include "stm32f10x.h"
#include "I2C_MPU6050.h"
#include "math.h"
#include "usart.h"

extern float AngleOffset_Rol,AngleOffset_Pit; 

void Accel_Con(Int16_xyz *acc_in,Int16_xyz *acc_out);

void IMUupdate(Int16_xyz *gyr, Int16_xyz *acc, Float_angle *angle);


#endif
