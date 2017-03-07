#include "motor_control.h"
#include "inv_mpu.h"
#include <math.h>
#include <string.h>
#define CHECK_RANGE(a) \
	if(a > 300) a = 300; \
	else if(a < -300) a = -300

Controller pidc;
Controller *pid = &pidc;

int Controller_init(Controller *pid){
	memset(pid,0,sizeof(Controller));
	return 0;
}
//----------------------------------------------------------------------------------------------------
int Controller_react(Controller *pid,float dt){
	float x_out_result,y_out_result,z_out_result;
	float x_in_result,y_in_result,z_in_result;

	//Begin : outer loop (PI)
	pid->x_out_i += (pid->gxTarget - pid->gx)*dt;
	pid->y_out_i += (pid->gyTarget - pid->gy)*dt;
	pid->z_out_i += (pid->gzTarget - pid->gz)*dt;
	CHECK_RANGE(pid->x_out_i);
	CHECK_RANGE(pid->y_out_i);
	CHECK_RANGE(pid->z_out_i);
	x_out_result = (pid->gxTarget - pid->gx)*pid->x_out_kp + pid->x_out_i*pid->x_out_ki;
	y_out_result = (pid->gyTarget - pid->gy)*pid->y_out_kp + pid->y_out_i*pid->y_out_ki;
	z_out_result = (pid->gzTarget - pid->gz)*pid->z_out_kp + pid->z_out_i*pid->z_out_ki;
	//End
	
	//Begin : inner loop (PID)
	pid->x_in_i += (x_out_result - pid->gax)*dt;
	pid->y_in_i += (y_out_result - pid->gay)*dt;
	pid->z_in_i += (z_out_result - pid->gaz)*dt;
	CHECK_RANGE(pid->x_in_i);
	CHECK_RANGE(pid->y_in_i);
	CHECK_RANGE(pid->z_in_i);
	x_in_result = (x_out_result - pid->gax)*pid->x_in_kp + pid->x_in_i*pid->x_in_ki + (x_out_result - pid->gax - pid->x_in_last)/dt*pid->x_in_kd;
	y_in_result = (y_out_result - pid->gay)*pid->y_in_kp + pid->y_in_i*pid->y_in_ki + (y_out_result - pid->gay - pid->y_in_last)/dt*pid->y_in_kd;
	z_in_result = (z_out_result - pid->gaz)*pid->z_in_kp + pid->z_in_i*pid->z_in_ki + (z_out_result - pid->gaz - pid->z_in_last)/dt*pid->z_in_kd;
	pid->x_in_last = x_out_result - pid->gax;
	pid->y_in_last = y_out_result - pid->gay;
	pid->z_in_last = z_out_result - pid->gaz;
	//End
	
	//Begin : control motors
	//printf("x = %f , y = %f , z = %f\n",x_in_result,y_in_result,z_in_result);
	// if(!pid->Motor->enabled){
	// 	x_out_i = y_out_i = z_out_i = x_in_i = y_in_i = z_in_i = 0;
	// }
	float up,down,right,left;
	up    = pid->throttleTarget + (int)round(( x_in_result - y_in_result + z_in_result)*10);
	down  = pid->throttleTarget + (int)round((-x_in_result + y_in_result + z_in_result)*10);
	right = pid->throttleTarget + (int)round((-x_in_result - y_in_result - z_in_result)*10);
	left  = pid->throttleTarget + (int)round(( x_in_result + y_in_result - z_in_result)*10);
	Motor_setVelocities(up,down,left,right);
	//End

	return 0;
}
int Controller_refreshData(Controller *pid){
	short a[3];
	mpu_get_gyro_reg(a,NULL);
	pid->gax = a[0] / 100.0;
	pid->gay = a[1] / 100.0;
	pid->gaz = a[2] / 100.0;
	mpu_dmp_get_data(&pid->gx,&pid->gy,&pid->gz,&pid->ax,&pid->ay,&pid->az);
	return 0;
}