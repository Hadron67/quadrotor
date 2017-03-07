#ifndef __MOTOR_H
#define __MOTOR_H

int Motor_init(void);
int Motor_start(void);
int Motor_stop(void);
int Motor_setVelocities(int up,int down,int left,int right);
#endif
