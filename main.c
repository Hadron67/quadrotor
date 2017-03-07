/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "log.h"
#include "motor.h"
#include "mpu6050.h"
#include "motor_control.h"
#include "blue_control.h"

#define TAG "main"

int main(){
    int errcode;
    usart_init(9600);
    bluetooth_usart_init(9600);
    LOGI(TAG,"init successfull");

    Motor_init();
    Controller_init(pid);
    delay_ms(90);
    while((errcode = mpu_dmp_init()) != 0){
        LOGE(TAG,"failed to init mpu6050: %d",errcode);
    }
    while(1){
        if(Controller_refreshData(pid)){
            //LOGE(TAG,"failed to read data from mpu6050");
        }
        else{
            Controller_react(pid,1);
            delay_ms(1);
        }
            //LOGI(TAG,"%f %f %f %d %d %d",pid->gx,pid->gy,pid->gz,pid->gax,pid->gay,pid->gaz);
    }
    return 0;
}

