#ifndef USART_H
#define USART_H
#include "stm32f10x.h"
#define EN_USART1_RX
#define USART1_RES_LEN 20
//extern u8 usart1_res_buf[USART1_RES_LEN];
struct usart_buf 
{
	char data_buffer[USART1_RES_LEN];
	char* base;
	char* top;
};
void usart_init(u32 baud);
void usart_buf_flush(void);
#endif
