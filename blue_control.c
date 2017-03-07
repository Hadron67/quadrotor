#include <stdlib.h>
#include <stdint.h>
#include "command.h"
#include "log.h"
#include "stm32f10x.h"
#include "blue_control.h"
#include "motor_control.h"
#include "motor.h"

#define TAG "BlueTooth"

static CommandDFA dfa;
static uint8_t status = 0;

static int Bluetooth_write(const void *src,size_t size){
    size_t i;
    const char *t = (const char *)src;
    for(i = 0;i < size;i++){
        while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		    USART_SendData(USART2,t[i]);
    }
    return 0;
}
static void Bluetooth_return(uint8_t r){
    Bluetooth_write(&r,sizeof(uint8_t));
}
static void onSetFloatParameter(uint8_t addr,float val){
    if(addr <= 15){
        //LOGI(TAG,"set parameter %d to %f.",addr,val);
        *(&pid->x_in_kp + addr) = val;
        status = 0;
    }
    else {
        LOGE(TAG,"parameter address '%d' is out of range",addr);
        status = -1;
    }
}

static void onSetMotor(char c){
    if(c){
        //LOGI(TAG,"starting motor");
        Motor_start();
        status = 0;
    }
    else {
        //LOGI(TAG,"stoping motor");
        Motor_stop();
        status = 0;
    }
}
static void onSetThrottle(int t){
    if(t >= 0 && t<= 10000){
       // LOGI(TAG,"setting throttle to %d",t);
        pid->throttleTarget = t;
        status = 0;
    }
    else {
        //LOGW(TAG,"throttle value '%d' is out of range",t);
        status = -1;
    }
}
static void onSetGyro(uint8_t which,float val){
    if(which >= 0 && which <= 2){
        //LOGI(TAG,"setting gyto '%d' to %f",which,val);
        *(&pid->gxTarget + which) = val;
        status = 0;
    }
    else {
        LOGW(TAG,"gyro index '%d' is out of range",which);
        status = -1;
    }
}
static void onGetStatus(){
    Bluetooth_return(status);
}

int bluetooth_usart_init(int baud){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

    GPIO_InitTypeDef gtype;
    gtype.GPIO_Speed = GPIO_Speed_50MHz;
    gtype.GPIO_Pin = GPIO_Pin_2;
    gtype.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gtype);
    gtype.GPIO_Pin = GPIO_Pin_3;
    gtype.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gtype);

    USART_InitTypeDef utype = {
        .USART_BaudRate = baud,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Rx | USART_Mode_Tx
    };
    USART_Init(USART2, &utype);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	
    USART_ClearFlag(USART2, USART_FLAG_TC);	
    USART_Cmd(USART2, ENABLE);	

    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel = USART2_IRQn,
        .NVIC_IRQChannelPreemptionPriority = 1,
        .NVIC_IRQChannelSubPriority = 1,
        .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStructure);

    CommandDFA_init(&dfa);
    dfa.onSetFloatParametre = onSetFloatParameter;
    dfa.onSetMotor = onSetMotor;
    dfa.onSetThrottle = onSetThrottle;
    dfa.onSetGyro = onSetGyro;
    dfa.onGetStatus = onGetStatus;
    return 0;
}

void USART2_IRQHandler(void){
	char res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		res = USART_ReceiveData(USART2);
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        if(CommandDFA_accept(&dfa,res)){
            LOGW(TAG,"data syntax error,unexpected '%c'",res);
            status = -1;
        }
	}
}