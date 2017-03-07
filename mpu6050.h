#ifndef __MPU6050_H
#define __MPU6050_H
#include "myiic.h" 
#include "iprintf.h"



u8 MPU_Init(void); 							
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf); 
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf); 
u8 MPU_Write_Byte(u8 reg,u8 data);			
u8 MPU_Read_Byte(u8 reg);					

u8 MPU_Set_Gyro_Fsr(u8 fsr);
u8 MPU_Set_Accel_Fsr(u8 fsr);
u8 MPU_Set_LPF(u16 lpf);
u8 MPU_Set_Rate(u16 rate);
u8 MPU_Set_Fifo(u8 sens);


short MPU_Get_Temperature(void);
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az);

short MPU_getTemperature(void);
int MPU_getGyroscope(short *gx,short *gy,short *gz);
int MPU_getAccelerometer(short *ax,short *ay,short *az);

#endif

