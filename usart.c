#include "log.h"
#include "usart.h"
#include "delay.h"
#include "loopqueue.h"
#include "command.h"

#define TAG "USRAT1"

static Queue usart1_buf;
void usart_init(u32 baud){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //ISART1_TX PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //??????
    GPIO_Init(GPIOA, &GPIO_InitStructure); //??? GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_RX PA.10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //????
    GPIO_Init(GPIOA, &GPIO_InitStructure); //??? GPIOA.10

    USART_InitTypeDef utype = {
        .USART_BaudRate = baud,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Rx | USART_Mode_Tx
    };
    USART_DeInit(USART1); //???? 1
    USART_Init(USART1,&utype);
    USART_Cmd(USART1,ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);

    #ifdef EN_USART1_RX
    //initialize NVIC
    usart_buf_flush();
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_InitTypeDef NVIC_InitStructure = {
        .NVIC_IRQChannel = USART1_IRQn,
        .NVIC_IRQChannelPreemptionPriority=3, //????? 3
        .NVIC_IRQChannelSubPriority = 3, //???? 3
        .NVIC_IRQChannelCmd = ENABLE //IRQ ????
    };
    NVIC_Init(&NVIC_InitStructure); //????????
    #endif
    //?????
    
}
void usart_buf_flush(void){
	queue_init(&usart1_buf);
}

int ifputc (int ch){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		USART_SendData(USART1,ch);
	return 0;
}

void USART1_IRQHandler(void){
	char res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		res = USART_ReceiveData(USART1);
		LOGI(TAG,"data recieved : 0x%X (%c)",res,res);
	}
}




//data operations
