#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "motor.h"
typedef struct _Controller Controller;

struct _Controller {
	//inner PID parametres
	float x_in_kp;
	float x_in_ki;
	float x_in_kd;
	float y_in_kp;
	float y_in_ki;
	float y_in_kd;
	float z_in_kp;
	float z_in_ki;
	float z_in_kd;
	//outer PID parametres
	float x_out_kp;
	float x_out_ki;
	float y_out_kp;
	float y_out_ki;
	float z_out_kp;
	float z_out_ki;
	//controlling prametres
	float gxTarget;
	float gyTarget;
	float gzTarget;
	int throttleTarget;
	//
	float x_out_i,y_out_i,z_out_i,x_in_i,y_in_i,z_in_i;
	float x_in_last,y_in_last,z_in_last;
	float gx,gy,gz,gax,gay,gaz;
	short ax,ay,az;
};
extern Controller *pid;

int Controller_init(Controller *pid);
int Controller_refreshData(Controller *pid);
int Controller_react(Controller *pid,float dt);
#endif
