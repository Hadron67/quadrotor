#include "delay.h"
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*CRISTAL_FREQUENCY/8; // 		 
	SysTick->VAL=0x00;        //
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //
	do {
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //
	SysTick->VAL =0X00;       //
}
void delay_ms(u32 nms){
	u32 temp;
	SysTick->LOAD=nms*1000*CRISTAL_FREQUENCY/8; // 		 
	SysTick->VAL=0x00;        //
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //
	do {
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //
	SysTick->VAL =0X00; 
}
