#include "mpu6050.h"  
//////////////////////////////////////////////////////////////////////////////////	 
#include "log.h"
#define TAG "mpu6050"


//#define MPU_ACCEL_OFFS_REG		0X06	//accel_offs�Ĵ���,�ɶ�ȡ�汾��,�Ĵ����ֲ�δ�ᵽ
//#define MPU_PROD_ID_REG			0X0C	//prod id�Ĵ���,�ڼĴ����ֲ�δ�ᵽ
#define MPU_SELF_TESTX_REG		0X0D	//�Լ��Ĵ���X
#define MPU_SELF_TESTY_REG		0X0E	//�Լ��Ĵ���Y
#define MPU_SELF_TESTZ_REG		0X0F	//�Լ��Ĵ���Z
#define MPU_SELF_TESTA_REG		0X10	//�Լ��Ĵ���A
#define MPU_SAMPLE_RATE_REG		0X19	//����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG				0X1A	//���üĴ���
#define MPU_GYRO_CFG_REG		0X1B	//���������üĴ���
#define MPU_ACCEL_CFG_REG		0X1C	//���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG		0X1F	//�˶����ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG			0X23	//FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG		0X24	//IIC�������ƼĴ���
#define MPU_I2CSLV0_ADDR_REG	0X25	//IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG			0X26	//IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG	0X27	//IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG	0X28	//IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG			0X29	//IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG	0X2A	//IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG	0X2B	//IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG			0X2C	//IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG	0X2D	//IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG	0X2E	//IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG			0X2F	//IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG	0X30	//IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG	0X31	//IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG			0X32	//IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG		0X33	//IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG	0X34	//IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG		0X35	//IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG		0X36	//IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG		0X37	//�ж�/��·���üĴ���
#define MPU_INT_EN_REG			0X38	//�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG			0X3A	//�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG		0X3B	//���ٶ�ֵ,X����8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG		0X3C	//���ٶ�ֵ,X����8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG		0X3D	//���ٶ�ֵ,Y����8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG		0X3E	//���ٶ�ֵ,Y����8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG		0X3F	//���ٶ�ֵ,Z����8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG		0X40	//���ٶ�ֵ,Z����8λ�Ĵ���

#define MPU_TEMP_OUTH_REG		0X41	//�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG		0X42	//�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG		0X43	//������ֵ,X����8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG		0X44	//������ֵ,X����8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG		0X45	//������ֵ,Y����8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG		0X46	//������ֵ,Y����8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG		0X47	//������ֵ,Z����8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG		0X48	//������ֵ,Z����8λ�Ĵ���

#define MPU_I2CSLV0_DO_REG		0X63	//IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG		0X64	//IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG		0X65	//IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG		0X66	//IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG	0X67	//IIC������ʱ�����Ĵ���
#define MPU_SIGPATH_RST_REG		0X68	//�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG	0X69	//�˶��������ƼĴ���
#define MPU_USER_CTRL_REG		0X6A	//�û����ƼĴ���
#define MPU_PWR_MGMT1_REG		0X6B	//��Դ�����Ĵ���1
#define MPU_PWR_MGMT2_REG		0X6C	//��Դ�����Ĵ���2 
#define MPU_FIFO_CNTH_REG		0X72	//FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG		0X73	//FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG			0X74	//FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG		0X75	//����ID�Ĵ���
 
#define MPU_ADDR				0x68

u8 MPU_Init(void)
{ 
	u8 res;
	IIC_Init();
	if(MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80)){
		return 2;
	}
	delay_ms(100);
	if(MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00)){
		return 3;
	}
	if(MPU_Set_Gyro_Fsr(3)){
		return 4;
	}
	if(MPU_Set_Accel_Fsr(0)){
		return 5;
	}				
	if(MPU_Set_Rate(50)){
		return 6;
	}					
	if(MPU_Write_Byte(MPU_INT_EN_REG,0X00)){
		return 7;
	}
	if(MPU_Write_Byte(MPU_USER_CTRL_REG,0X00)){
		return 8;
	}
	if(MPU_Write_Byte(MPU_FIFO_EN_REG,0X00)){
		return 9;
	}	
	if(MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80)){
		return 10;
	}	
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR){
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);
		MPU_Set_Rate(50);
 	}else {
		LOGE(TAG,"address error: %d",res);
		return 1;
	};
	return 0;
}

u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);
}

u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);
}

u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);
}

u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);
 	return MPU_Set_LPF(rate/2);
}

short MPU_getTemperature(void){
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;
}

int MPU_getGyroscope(short *gx,short *gy,short *gz){
    u8 buf[6],res;  
	res = MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0){
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;
}

int MPU_getAccelerometer(short *ax,short *ay,short *az){
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);
		if(IIC_Wait_Ack())
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);
    IIC_Wait_Ack();	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);
		else *buf=IIC_Read_Byte(1);	
		len--;
		buf++; 
	}    
    IIC_Stop();	
	return 0;	
}

u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
	IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);
	if(IIC_Wait_Ack())
	{
		LOGE(TAG,"in MPU_Write_Byte: failed at 1,reg=%X,data=%X",reg,data);
		IIC_Stop();		 
		return 1;		
	}
	IIC_Send_Byte(reg);
	if(IIC_Wait_Ack()){
		LOGE(TAG,"in MPU_Write_Byte: failed at 2,reg=%X,data=%X",reg,data);
		IIC_Stop();
		return 1;
	}	
	IIC_Send_Byte(data);
	if(IIC_Wait_Ack())
	{
		LOGE(TAG,"in MPU_Write_Byte: failed at 3,reg=%X,data=%X",reg,data);
		IIC_Stop();	 
		return 1;		 
	}
	IIC_Stop();	 
	return 0;
}

u8 MPU_Read_Byte(u8 reg)
{
	u8 res=0;
	IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);
	IIC_Wait_Ack();		
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();	
	IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)|1);
	IIC_Wait_Ack();	
	res=IIC_Read_Byte(0);
	IIC_Stop();		
	return res;		
}


