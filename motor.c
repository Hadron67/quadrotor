#include "motor.h"
#include "stm32f10x.h"
#define CHECK_RANGE(a) \
	if(a > 10000) a=10000; \
	else if(a < 0) a=0;
/* motor* -> [0,10000] */

int Motor_init(void){
	GPIO_InitTypeDef gpiotype;
	TIM_TimeBaseInitTypeDef timtype;
	TIM_OCInitTypeDef timoctype;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//Begin : Configure the four GPIOs..
	gpiotype.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpiotype.GPIO_Mode = GPIO_Mode_AF_PP;
	gpiotype.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpiotype);
	gpiotype.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA,&gpiotype);
	//End
	//---------------------------------------------------------------
	//Begin : Configure the timers
	TIM_DeInit(TIM3);
	TIM_InternalClockConfig(TIM3);
	timtype.TIM_Period = 10000;
	timtype.TIM_Prescaler = 1;
	timtype.TIM_ClockDivision = 0;
	timtype.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&timtype);
	//End
	//---------------------------------------------------------------
	//Begin : Configure the output channels of TIM(3)
	timoctype.TIM_OCMode = TIM_OCMode_PWM1;
	timoctype.TIM_OutputState = TIM_OutputState_Enable;
	timoctype.TIM_OCPolarity = TIM_OCPolarity_High;
	timoctype.TIM_Pulse = 0;
	TIM_OC1Init(TIM3,&timoctype);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //right
	TIM_OC2Init(TIM3,&timoctype);
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //left
	TIM_OC3Init(TIM3,&timoctype);
	TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); //down
	TIM_OC4Init(TIM3,&timoctype);
	TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable); //up
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC4);
	//End
	//---------------------------------------------------------------
}
int Motor_start(void){
	TIM_Cmd(TIM3,ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
	return 0;
}
int Motor_stop(void){
	TIM_Cmd(TIM3,DISABLE);
	TIM_CtrlPWMOutputs(TIM3,DISABLE);
	return 0;
}

int Motor_setVelocities(int up,int down,int left,int right){
	CHECK_RANGE(up);
	CHECK_RANGE(down);
	CHECK_RANGE(left);
	CHECK_RANGE(right);
	TIM3->CCR1 = left;
	TIM3->CCR2 = down;
	TIM3->CCR3 = right;
	TIM3->CCR4 = up;
	return 0;
}