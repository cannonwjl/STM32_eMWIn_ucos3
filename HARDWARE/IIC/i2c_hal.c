//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.c (V1.1)
// Author    :  RFU
// Date      :  6-Mai-2015
// Controller:  STM32F100RB
// IDE       :  �Vision V5.12.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer
//=============================================================================

//-- Includes ------------------------------------------------------------------
#include "i2c_hal.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
//-- Defines ------------------------------------------------------------------
// I2C IO-Pins                        /* -- adapt the defines for your uC -- */

// SDA on port B, bit 14
//#define SDA_LOW()  (GPIOB->BSRR = 0x40000000) // set SDA to low
//#define SDA_OPEN() (GPIOB->BSRR = 0x00004000) // set SDA to open-drain
//#define SDA_READ   (GPIOB->IDR  & 0x4000)     // read SDA

//// SCL on port B, bit 13              /* -- adapt the defines for your uC -- */
//#define SCL_LOW()  (GPIOB->BSRR = 0x20000000) // set SCL to low
//#define SCL_OPEN() (GPIOB->BSRR = 0x00002000) // set SCL to open-drain
//#define SCL_READ   (GPIOB->IDR  & 0x2000)     // read SCL


// SDA on port B, bit 7 ����
#define SDA_LOW()  (GPIOB->BSRR = 0x00800000) // set SDA to low
#define SDA_OPEN() (GPIOB->BSRR = 0x00000080) // set SDA to open-drain
#define SDA_READ   (GPIOB->IDR  & 0x0080)     // read SDA

// SCL on port B, bit 6 ����             /* -- adapt the defines for your uC -- */
#define SCL_LOW()  (GPIOB->BSRR = 0x00400000) // set SCL to low
#define SCL_OPEN() (GPIOB->BSRR = 0x00000040) // set SCL to open-drain
#define SCL_READ   (GPIOB->IDR  & 0x0040)     // read SCL


#define  SCL_H    SCL_OPEN()
#define  SCL_L    SCL_LOW() 

#define  SCL     SCL_READ
#define  SDA     SDA_READ

#define SDA_H    SDA_OPEN()
#define SDA_L    SDA_LOW()
//-- Static function prototypes -----------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout);

//-----------------------------------------------------------------------------
//void I2c_Init(void)                      /* -- adapt the init for your uC -- */
//{
//  RCC->APB2ENR |= 1<<3;  // I/O port B clock enabled
//  
//  SDA_OPEN();                  // I2C-bus idle mode SDA released
//  SCL_OPEN();                  // I2C-bus idle mode SCL released
//  
//  // SDA on port B, bit 14
//  // SCL on port B, bit 13
//  GPIOB->CRH   &= 0xF00FFFFF;  // set open-drain output for SDA and SCL
//  GPIOB->CRH   |= 0x05500000;  // 
//}
void I2c_Init(void)                      /* -- adapt the init for your uC -- */
{
  RCC->APB2ENR |= 1<<3;  // I/O port B clock enabled
  SDA_OPEN();                  // I2C-bus idle mode SDA released
  SCL_OPEN();                  // I2C-bus idle mode SCL released
   // SDA on port B, bit 7
  // SCL on port B, bit 6
  GPIOB->CRL   &= 0x00FFFFFF;  // set open-drain output for SDA and SCL
  GPIOB->CRL   |= 0x55000000;  // 
   
}

//-----------------------------------------------------------------------------
void I2c_StartCondition(void)
{
  SDA_OPEN();
  DelayMicroSeconds(1);
  SCL_OPEN();
  DelayMicroSeconds(1);
  SDA_LOW();
  DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
  SCL_LOW();
  DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(void)
{
  SCL_LOW();
  DelayMicroSeconds(1);
  SDA_LOW();
  DelayMicroSeconds(1);
  SCL_OPEN();
  DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
  SDA_OPEN();
  DelayMicroSeconds(10);
}

//-----------------------------------------------------------------------------
etError I2c_WriteByte(u8t txByte)
{
  etError error = NO_ERROR;
  u8t     mask;
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) SDA_LOW(); // masking txByte, write bit to SDA-Line
    else                     SDA_OPEN();
    DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
    SCL_OPEN();                         // generate clock pulse on SCL
    DelayMicroSeconds(5);               // SCL high time (t_HIGH)
    SCL_LOW();
    DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
  }
  SDA_OPEN();                           // release SDA-line
  SCL_OPEN();                           // clk #9 for ack
  DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
  if(SDA_READ) error = ACK_ERROR;       // check ack from i2c slave
  SCL_LOW();
  DelayMicroSeconds(20);                // wait to see byte package on scope
  return error;                         // return error code
}

//-----------------------------------------------------------------------------
etError I2c_ReadByte(u8t *rxByte, etI2cAck ack, u8t timeout)
{
  etError error = NO_ERROR;
  u8t mask;
  *rxByte = 0x00;
  SDA_OPEN();                            // release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    SCL_OPEN();                          // start clock on SCL-line
    DelayMicroSeconds(1);                // clock set-up time (t_SU;CLK)
    error = I2c_WaitWhileClockStreching(timeout);// wait while clock streching
    DelayMicroSeconds(3);                // SCL high time (t_HIGH)
    if(SDA_READ) *rxByte |= mask;        // read bit
    SCL_LOW();
    DelayMicroSeconds(1);                // data hold time(t_HD;DAT)
  }
  if(ack == ACK) SDA_LOW();              // send acknowledge if necessary
  else           SDA_OPEN();
  DelayMicroSeconds(1);                  // data set-up time (t_SU;DAT)
  SCL_OPEN();                            // clk #9 for ack
  DelayMicroSeconds(5);                  // SCL high time (t_HIGH)
  SCL_LOW();
  SDA_OPEN();                            // release SDA-line
  DelayMicroSeconds(20);                 // wait to see byte package on scope
  
  return error;                          // return with no error
}

//-----------------------------------------------------------------------------
etError I2c_GeneralCallReset(void)
{
  etError error;
  
  I2c_StartCondition();
                        error = I2c_WriteByte(0x00);
  if(error == NO_ERROR) error = I2c_WriteByte(0x06);
  
  return error;
}

//-----------------------------------------------------------------------------
static etError I2c_WaitWhileClockStreching(u8t timeout)
{
  etError error = NO_ERROR;
  
  while(SCL_READ == 0)
  {
    if(timeout-- == 0) return TIMEOUT_ERROR;
    DelayMicroSeconds(1000);
  }
  
  return error;
}

void delay_1us(u8 x)//������ʱ,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
u8 I2C_Start(void)
{
		SDA_H; 
		delay_1us(5);	//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		SCL_H;
		delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		if(!SDA) return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA_L;   //SCL���ڸߵ�ƽ��ʱ��SDA����
		delay_1us(5);
	  if(SDA) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
    SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2c_SendACK(u8 i)
{
    if(1==i)
			SDA_H;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
			SDA_L;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	  SCL_H;                    //����ʱ����
    delay_1us(5);                 //��ʱ
    SCL_L ;                  //����ʱ����
    delay_1us(5);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	uint16_t i=0;
	SDA_H;	        //�ͷ�SDA
	SCL_H;         //SCL���߽��в���
	while(SDA)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==500)//��ʱ����ѭ��
		break;
	}
	if(SDA)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL_L; 
		return RESET;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
  delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SCL_L;
	delay_1us(5); //��ʱ��֤ʱ��Ƶ�ʵ���40K��
	return SET;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL_L;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
		if(dat&0x80)//SDA׼��
		SDA_H;  
		else 
		SDA_L;
    SCL_H;                //����ʱ�ӣ����ӻ�����
    delay_1us(5);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL_L;                //����ʱ�ӣ���SDA׼��
    delay_1us(5); 		  //��ʱ����IICʱ��Ƶ��
		dat <<= 1;          //�Ƴ����ݵ����λ  
  }					 
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA_H;//�ͷ�SDA�����ӻ�ʹ��
    delay_1us(1);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		  dat <<= 1;
			
      SCL_H;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA) //������    
		   dat |=0x01;      
       delay_1us(5);     //��ʱ����IICʱ��Ƶ��		
       SCL_L;           //����ʱ���ߣ�������յ�������
       delay_1us(5);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}

//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
u8 Single_WriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 data)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();   //����ֹͣ�ź�
		
		return SET;
}

u8 Single_MWriteI2C_byte(u8 Slave_Address,u8 REG_Address,u8 const *data,u8 length)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
	while(length)
	{
		I2C_SendByte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(!I2C_WaitAck()){I2C_Stop(); return RESET;}           //Ӧ��
		length--;
	}
	//	I2C_SendByte(*data);       //�ڲ��Ĵ������ݣ�
 	//	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		I2C_Stop();   //����ֹͣ�ź�		
		return SET;
}

//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2c_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2c_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return SET;
}
