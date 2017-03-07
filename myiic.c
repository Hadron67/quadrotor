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

//��ʼ��IIC
void IIC_Init(void)
{			
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/*RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  //GPIOB8,B9��ʼ������
	GPIO_InitStructure.GPIO_Pin = Pin_SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//��ͨ���ģʽ,�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��*/
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
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SCL_Set_1;
	IIC_SDA_Set_1;	
	delay_us(5);
	//IIC_NOP();
 	IIC_SDA_Set_0;//START:when CLK is high,DATA change form high to low 
	delay_us(6);
	//IIC_NOP();
	IIC_SCL_Set_0;//ǯסI2C���ߣ�׼�����ͻ��������
	delay_us(2);
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_Set_0;
	IIC_SDA_Set_0;//STOP:when CLK is high DATA change form low to high
 	IIC_SCL_Set_1;
	delay_us(5);
	//IIC_NOP();
	IIC_SDA_Set_1;//����I2C���߽����ź�
	delay_us(6);							   	
	//IIC_NOP();
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL_Set_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
	u8 t;   
	SDA_OUT(); 	    
	IIC_SCL_Set_0;//����ʱ�ӿ�ʼ���ݴ���
	for(t=0;t<8;t++)
	{
		if((txd&0x80)>>7){
			IIC_SDA_Set_1;
		}
		else {
			IIC_SDA_Set_0;
		}
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		//IIC_NOP();
		IIC_SCL_Set_1;
		delay_us(2); 
		//IIC_NOP();
		IIC_SCL_Set_0;	
		delay_us(2);
		//IIC_NOP();
	}	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
		IIC_NAck();//����nACK
	else
		IIC_Ack(); //����ACK   
	return receive;
}



























