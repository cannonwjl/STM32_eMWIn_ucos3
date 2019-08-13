
#include "sx12xxUserfunc.h"
//C库头文件
//#include <stdio.h>
//#include <stdint.h>
//#include <stdbool.h> 
//#include <string.h>
//#include <math.h>

//SX1278
//#include "platform.h"
#include "radio.h"
#include "sx1276-Hal.h"
#include "sx1276-LoRa.h"
#include "sx1276.h"
#include "sys.h"
#include "stdio.h"
#include "delay.h"

//#include "platform.h"
#define BUFFER_SIZE     100                          // Define the payload size here

extern u16 PreambleLength;
u32 symbolTime;
u8 *recebuff1;
u16 lenth1;
u32 RxLowTimeoutTimer = 0;


static u16 BufferSize = BUFFER_SIZE;			// RF buffer size
static u8  Buffer[BUFFER_SIZE];				// RF buffer

//static uint8_t EnableMaster = true; 				// Master/Slave selection


const u8 PingMsg[] = "PINGtyuiopdfgw8_TEST";
const u8 PongMsg[] = "PONG";
const u8 MY_TEST_Msg[] = "wx22_SX1278_TEST";
//tRadioDriver RadioDriver1;


void RF_Init(void)
{
		delay_ms(10);
		printf("RF_Init\r\n");
		SX1276Init();
//    Radio = RadioDriverInit( );	
//    Radio->Init( );//SX1276Init();
}

void RF_StartTX()
{
		printf("SX1278_TX\n");
//    Radio->SetTxPacket( MY_TEST_Msg, 18 );   //RFLR_STATE_RX_INIT
	SX1276SetTxPacket(MY_TEST_Msg, 18);
}
void RF_StartRX()
{
//		printf("SX1278_RX\n");
//    Radio->StartRx( );   //RFLR_STATE_RX_INIT
	SX1276StartRx();
}
void SX1278Sleep(void)
{
	SX1278_SLEEP_Mode();
}

u8 RF_Send_General(u8 *buffer, u16 size)//一般模式发送
{
	u8 flag,ISflag;
	u8 state=RF_IDLE;
//	u32 num=0;
	PreambleLength=8;
	printf("\r\n start send\r\n");
	SX1276SetTxPacket(buffer,size);
	ISflag=1;
	while(ISflag)
	{
//		num++;
		state=SX1276Process();
		if(state == RF_TX_DONE)
		{
			ISflag=0;
			flag=RF_SUCCESS;
			printf("\r\n RF_SUCCESS \r\n");
		}
		 if(state==RF_RX_TIMEOUT)
		{
			ISflag=0;
			flag=RF_TIMEOUT;
			printf("\r\n RF_TIMEOUT \r\n");
		}	
		
	}
	return flag;
}

u8 RF_Send_Rouse(uint8_t *buffer, uint16_t size)//唤醒模式 发送  用于唤醒低功耗接收的模块
{
		u8 flag,ISflag;
	u8 state=RF_IDLE;
	PreambleLength=210;
//	printf("\r\n start send ");
	SX1276SetTxPacket(buffer,size);
//	printf("\r\n Tx ");
	ISflag=1;
	while(ISflag)
	{
//		num++;
//		printf("@");
		state=SX1276Process();
		if(state == RF_TX_DONE)
		{
			ISflag=0;
			flag=RF_SUCCESS;
//			printf("\r\n 1 ");
//			printf(" RF_Send_Rouse_SUCCESS \r\n");
		}
		 if(state==RF_TX_TIMEOUT)
		{
			ISflag=0;
			flag=RF_TIMEOUT;
//			printf("\r\n 2 ");
//			printf(" RF_Send_Rouse_TIMEOUT \r\n");
		}	
	}
	return flag;
}


//			SX1276StartRx();	
void RF_Receive( void *buffer, uint16_t *size )
{
	uint8_t state;
	while(1)
	{
		state=SX1276Process();
		if(  state == RF_RX_DONE)
		{
//			Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );//接收成功
			SX1276GetRxPacket( buffer, size );
//			printf("\r rece: ");
//				for(i=0;i < BufferSize;i++)
//				{
//					printf(" %02X",Buffer[i]);
//				}
//				printf("\n");
			break;
		}
		else if(state == RF_RX_TIMEOUT)
		{
			printf("RF_RX_TIMEOUT\n");
			break;
		}
//		SX1276StartRx();
	}
		
}

u8 RF_Receive_general( void *buffer, uint16_t *size )
{
	uint8_t state,reflag;
	while(1)
	{
		state=SX1276Process();
		if(  state == RF_RX_DONE)
		{
//			Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );//接收成功
			SX1276GetRxPacket( buffer, size );
//			printf("RF_Receive_general_SUCCESS\n");
			reflag=RF_SUCCESS;
			break;
		}
		else if(state == RF_RX_TIMEOUT)
		{
			reflag=RF_TIMEOUT;
//			printf("RF_Receive_general_TIMEOUT\n");
			break;
		}
//		SX1276StartRx();
	}
	return reflag;
		
}

void RF_Receive_Rouse()
{
	uint8_t state;
	RF_StartRX();
	while(1)
	{
		state=SX1276Process();
		if(  state == RF_RX_DONE)
		{
//			Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );//接收成功
			SX1276GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
		
			
			break;
		}
		else if(state == RF_RX_TIMEOUT)
		{
			printf("RF_RX_TIMEOUT\n");
			break;
		}
//		SX1276StartRx();
	}
		
}


void RF_Receive_LowPower( void *buffer, uint16_t *size )
{
	uint8_t i;
		u8 state;
//	symbolTime =	( ( pow( (float)2, (float)7 ) ) + 32 ) / 250000; // SF7 and BW = 250 KHz
//	symbolTime = symbolTime * 1000000;  =640
		while(1)
		{	
				
			SX1276_start_CAD();
//			Radio->Process( );
      SX1276Process();
			delay_ms(2);//1ms 
			
			while(SX1276Process()== RF_CHANNEL_ACTIVITY_DETECTED )
			{	
				 SX1278_RxMode();
//				 RxLowTimeoutTimer = GET_TICK_COUNT( );
					while(1)
					{
//						state=Radio->Process( );
						state=SX1276Process();
						if(state == RF_RX_DONE)
						{
//							Radio->GetRxPacket( Buffer, ( uint16_t* )&BufferSize );//接收成功
							SX1276GetRxPacket( Buffer, ( uint16_t* )&BufferSize );
							
								for(i=0;i < BufferSize;i++)
								{
//									USART_putchar(USART1,Buffer[i]);
									printf("%c",Buffer[i]);
								}
								printf("\n");
								for(i=0;i<BUFFER_SIZE;i++)
									Buffer[i] = 0;
								break;
						}
//						if(state == RF_RX_TIMEOUT)
//						{
//							printf("RF_RX_TIMEOUT \n");
//								for(i=0;i<BUFFER_SIZE;i++)
//									Buffer[i] = 0;
//								break;
//						}		
					}
			}
		}
}

u8 RF_Receive_LowPower_interrupt( void *buffer, uint16_t *size )//低功耗模式接收 发送方必须是唤醒模式 在定时器中断中循环
{
//	uint8_t i;
	u8 state,rece_state;
	rece_state=RF_FAILURE;
//	printf("1");
		while(SX1276Process()== RF_CHANNEL_ACTIVITY_DETECTED )
		{
			 SX1278_RxMode();
				while(1)
				{
//					printf("+");
					state=SX1276Process();	
					if(state == RF_RX_DONE)
					{
//						printf("D");
//						printf("start receive \n");
						SX1276GetRxPacket( buffer, size );//接收成功
						rece_state=RF_SUCCESS;
//							for(i=0;i < BufferSize;i++)
//							{
//								printf(" %02X",Buffer[i]);
//							}
//							printf("\n");
//							for(i=0;i<BUFFER_SIZE;i++)
//								Buffer[i] = 0;
							break;
					}
					
					if(state == RF_RX_TIMEOUT)
					{
//						printf("T");
//						printf("RF_RX_TIMEOUT \n");
//							for(i=0;i<BUFFER_SIZE;i++)
//								Buffer[i] = 0;
						rece_state=RF_TIMEOUT;
							break;
					}
					
				}
		}
		return rece_state;
}
double Read_RSSI(void)
{
	return SX1276LoRaReadRssi();
}
void Reset_sx1276(void)
{
//		SX1278_SLEEP_Mode();
//		SX1276ReInit();
	printf("\r\nSX1278_RESET");
	RF_Init();
		SX1278Sleep();//用wifi也得让SX1278 休眠
}
