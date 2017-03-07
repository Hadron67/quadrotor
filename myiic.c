#include "myiic.h"
#include "delay.h"
#include "stm32f10x.h"
#include "log.h"

#define TAG "IIC"

static void SDA_IN(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=Pin_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
static void SDA_OUT(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=Pin_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void IIC_NOP(){
	vu32 i, j;  
	vu32 sum = 0;  
	i = 20;  
	while(i--)  
	{  
		for (j = 0; j < 10; j++)  
			sum += i;   
	}  
	sum = i;
}

//初始化IIC
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
  //GPIOB8,B9初始化设置
	GPIO_InitStructure.GPIO_Pin = Pin_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//普通输出模式,推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化*/
	GPIO_InitStructure.GPIO_Pin=Pin_SDA;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=Pin_SCL;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	IIC_SCL_Set_1;
	IIC_SDA_Set_1;
	
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SCL_Set_1;
	IIC_SDA_Set_1;	
	delay_us(5);
	//IIC_NOP();
 	IIC_SDA_Set_0;//START:when CLK is high,DATA change form high to low 
	delay_us(6);
	//IIC_NOP();
	IIC_SCL_Set_0;//钳住I2C总线，准备发送或接收数据
	delay_us(2);
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_Set_0;
	IIC_SDA_Set_0;//STOP:when CLK is high DATA change form low to high
 	IIC_SCL_Set_1;
	delay_us(5);
	//IIC_NOP();
	IIC_SDA_Set_1;//发送I2C总线结束信号
	delay_us(6);							   	
	//IIC_NOP();
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_Set_1;
	delay_us(1);	   
	//IIC_NOP();
	IIC_SCL_Set_1;
	delay_us(1);
	//IIC_NOP();
	while(IIC_SDA_Read)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			LOGE(TAG,"Ack failed.");
			return 1;
		}
	}
	IIC_SCL_Set_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_Set_0;
	SDA_OUT();
	IIC_SDA_Set_0;
	delay_us(2);
	//IIC_NOP();
	IIC_SCL_Set_1;
	delay_us(5);
	//IIC_NOP();
	IIC_SCL_Set_0;
	//delay_us(2);
	//IIC_SDA_Set_1;
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_Set_0;
	SDA_OUT();
	IIC_SDA_Set_1;
	delay_us(2);
	//IIC_NOP();
	IIC_SCL_Set_1;
	delay_us(5);
	//IIC_NOP();
	IIC_SCL_Set_0;
	//delay_us(2);
	//IIC_SDA_Set_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_OUT(); 	    
	IIC_SCL_Set_0;//拉低时钟开始数据传输
	for(t=0;t<8;t++)
	{
		if((txd&0x80)>>7){
			IIC_SDA_Set_1;
		}
		else {
			IIC_SDA_Set_0;
		}
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		//IIC_NOP();
		IIC_SCL_Set_1;
		delay_us(2); 
		//IIC_NOP();
		IIC_SCL_Set_0;	
		delay_us(2);
		//IIC_NOP();
	}	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
	for(i=0;i<8;i++ )
	{
		IIC_SCL_Set_0; 
		delay_us(5);
		//IIC_NOP();
		IIC_SCL_Set_1;
		delay_us(2);
		receive<<=1;
		if(IIC_SDA_Read)receive++;
		delay_us(10);
		//IIC_NOP();		
	}					 
	if (!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK   
	return receive;
}



























