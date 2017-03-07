#ifndef __MYIIC_H
#define __MYIIC_H
//#include "sys.h"
#include "delay.h"
#include "stm32f10x.h"

//define SCL dns SDA pins:
#define Pin_SCL GPIO_Pin_8
#define Pin_SDA GPIO_Pin_9
//
//#define SDA_IN() GPIO_SetBits(GPIOB,Pin_SDA)
//#define SDA_OUT() 

//returns
#define IIC_ACK 0
#define IIC_READY 0
#define IIC_NACK 1
#define IIC_BUS_BUSY 2
#define IIC_BUS_ERROR 3
#define IIC_RETRY_COUNT 3 //times of retrying
//all the IIC operating functios
#define IIC_SCL_Set_0 GPIO_ResetBits(GPIOB,Pin_SCL)
#define IIC_SCL_Set_1 GPIO_SetBits(GPIOB,Pin_SCL)
#define IIC_SDA_Set_0 GPIO_ResetBits(GPIOB,Pin_SDA)
#define IIC_SDA_Set_1 GPIO_SetBits(GPIOB,Pin_SDA)
#define IIC_SDA_Read (GPIO_ReadInputDataBit(GPIOB,Pin_SDA))


typedef struct IIC{
    GPIO_TypeDef *scl,*sda;
    uint16_t sclPin,sdaPin;
};
void IIC_Init(void);                //Initialize IIC IO Pins				 
void IIC_Start(void);				//Send start signal of IIC
void IIC_Stop(void);	  			//Send stop signal of IIC
void IIC_Send_Byte(u8 txd);			//Send one byte via IIC
u8 IIC_Read_Byte(unsigned char ack);//Read one byte via IIC
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















