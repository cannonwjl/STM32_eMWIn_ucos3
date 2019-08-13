#include "Platform_Configure.h"
#include "system_platform.h"
//#include "gpio.h"
#include "stm32f10x.h"
#include "api.h"
#include "ggsensor.h"

u8 NB_Strat_IS_Register_Flag=ERROR;//NB ��������Ƿ�ע��ɹ���־
extern SensorEchoV2 se2;
u8 COMMUNICATION_MODULE_STATE=DISABLE;
#ifdef EZ267
u8 NB_Just_Send_Flag=0;//NB�շ������־  �������һ�η��ͺ�ȴ�һ���  ȷ��������ɣ��������ֲ�û�лر���BUG
#endif
u8 COMMUNICATION_MODULE_Start(void)//ģ������
{
	if(COMMUNICATION_MODULE_STATE==ENABLE)
	{
//		printf("\n MODULE already start ");
		return SUCCESS;
	}
	
	#ifdef EZ265
	if(WIFI_or_RF_SendMode==RF_SEND)
	{
		LoRaPWEN_ON();
		vTaskDelay(50);
		RF_Init();
	}	
	#endif
	
	#ifdef EZ267
		NB_Strat_IS_Register_Flag=NB_Strat_UDP();
	#endif
	COMMUNICATION_MODULE_STATE=ENABLE;
	return SUCCESS;
}
u8 COMMUNICATION_MODULE_CLOSE(void)//ģ��ر�
{
	if(COMMUNICATION_MODULE_STATE==ENABLE)
	{
		printf(" MODULE close ");
		#ifdef EZ265
		if(WIFI_or_RF_SendMode==RF_SEND)
		{
			LoRaPWEN_OFF();
		}	
		#endif
		
		#ifdef EZ267
		  NB_UART_DeInit();
			vTaskDelay(5);
			NBPWEN_OFF();
		#endif
		COMMUNICATION_MODULE_STATE=DISABLE;
	}
	
	return SUCCESS;
}

u8 COMMUNICATION_MODULE_Send(uint8_t *buffer, u8 size)//ģ�鷢�ͺ���  ����ֵΪ�յ�����״̬ 0û��  1ע���  2���ݰ�
{
	int k;
	u8 RecePackage_state=RECEBack_ERROR;
	printf(" ---send---");
		for( k=0; k<size; k++)
			printf(" %02X", buffer[k]);
//	u8 sendbuf[30]={0x7E,0x46 ,0x48 ,0xEA ,0xC1 ,0x7C ,0xFF ,0xFF ,0xFF ,0xFF ,0x10 ,0x00 ,0x0B ,0x48 ,0xEA ,0xC1 ,0x7C ,0x00 ,0x58 ,0x00 ,0x20 ,0xFF ,0x00 ,0x01 ,0xEE ,0xC6 ,0x7E};
	#ifdef EZ265
	u8 rfrece_flag2=5,rfrece_flag1=5;
	if(WIFI_or_RF_SendMode==RF_SEND)
	{
		RecePackage_state=0;
		FLASH_Start_Busy();
		rfrece_flag2=RF_Send_Rouse(buffer,size);
		if(rfrece_flag2==RF_TIMEOUT)
		{
			
			Reset_sx1276();
		}
		else
		{
			RF_StartRX();//���óɽ��� 
			rfrece_flag1= RF_Receive_general(RF_Rece_Buff,&RF_Rece_Lenth);
		}
		if(rfrece_flag1==RF_SUCCESS)
		{
//				printf(" Read_RSSI=%f",Read_RSSI()); 
	//		printf("\r rece: ");
	//		for(i=0;i < RF_Rece_Lenth;i++)
	//		{
	//			printf(" %02X",RF_Rece_Buff[i]);
	//		}
	//		printf("\n");
			RecePackage_state=LowRF_input((char*)RF_Rece_Buff,RF_Rece_Lenth);
		}
		FLASH_Free();
	}	
	#endif
	//EZ267
	#ifdef EZ267
	if(NB_Send(buffer,size)==1)
	{
		RecePackage_state=RECEBack_Data;
	}
	
	#endif
//	printf("RecePackage_state=%d",RecePackage_state);

	return RecePackage_state;
}

u8 COMMUNICATION_MODULE_REGISTER(void)//ͨѶģ��ע��  ����ֵΪ�Ƿ��հ��ɹ� SUCCESS ��  ERROR
{
	#ifdef EZ281
	assemble_echo_pkt2(&se2);
	send_se2(se2);
	if(COMMUNICATION_MODULE_Send(Gateway_echodata_buf, Gateway_echodata_len)==RECEBack_Echo)
		return SUCCESS;
	else
		return ERROR;
	#endif
	
	#ifdef EZ267
	 if(NB_Strat_IS_Register_Flag==SUCCESS)
		 return SUCCESS;
	 else
		 return ERROR;
	#endif
	 
}

u8 COMMUNICATION_MODULE_SENDPACKET(void)//ͨѶģ�鷢���ݰ�  ����ֵΪ SUCCESS ��  ERROR
{
	#ifdef EZ281
	if(WIFI_or_RF_SendMode==RF_SEND)
	{
		RF_Send_flash_Data();
		if(COMMUNICATION_MODULE_Send(Gateway_sensordata_buf, Gateway_sensordata_len)==RECEBack_Data)
		return SUCCESS;
	else
		return ERROR;
	}
	else if(WIFI_or_RF_SendMode==WIFI_SEND)
	{
		WIFI_send_data2();
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
	#endif
	
	#ifdef EZ267
	if(NB_node_or_weather_mode==WEATHER_STATION)//����վ�Ĳɼ��洢
	{
			NB_send_data_weather();
	}
	else
	{
		WIFI_send_data2();
	}

	if(COMMUNICATION_MODULE_Send(Gateway_sensordata_buf, Gateway_sensordata_len)==RECEBack_Data)
	{
		NB_Just_Send_Flag=1;
		return SUCCESS;
	}
		
	else
		return ERROR;

	#endif
	
}

u8 COMMUNICATION_MODULE_REDO_REGISTER(void)//ͨѶģ��ע��ֱ���ɹ�  ����ֵΪ SUCCESS ��  ERROR
{
	u8 num=retry_Register_num;
	u8 returnflag=ERROR;
	COMMUNICATION_MODULE_Start();
	while(num)
	{
		num--;
		if(COMMUNICATION_MODULE_REGISTER()==SUCCESS)
		{
			returnflag=SUCCESS;
			break;
		}
		
	}
	
	#ifdef EZ267
	if(returnflag==ERROR)
	{
		COMMUNICATION_MODULE_CLOSE();
		vTaskDelay(100);
		COMMUNICATION_MODULE_Start();
		num=retry_Register_num;
		while(num)
		{
			num--;
			if(COMMUNICATION_MODULE_REGISTER()==SUCCESS)
			{
				returnflag=SUCCESS;
				break;
			}
			
		}
		
	}
	#endif
	
	return returnflag;
}

u8 COMMUNICATION_MODULE_REDO_SENDPACKET(void)//ͨѶģ�鷢���ݰ�ֱ���ɹ� ����ֵΪ SUCCESS ��  ERROR
{
	u8 num=retry_Register_num;
	u8 returnflag=ERROR;
	COMMUNICATION_MODULE_Start();
	while(num)
	{
		num--;
		if(COMMUNICATION_MODULE_SENDPACKET()==SUCCESS)
		{
			returnflag=SUCCESS;
			break;
		}
		
	}
	#ifdef EZ267
	if(returnflag==ERROR)
	{
		COMMUNICATION_MODULE_CLOSE();
		vTaskDelay(100);
		COMMUNICATION_MODULE_Start();
		num=retry_Register_num;
		while(num)
		{
			num--;
			if(COMMUNICATION_MODULE_SENDPACKET()==SUCCESS)
			{
				returnflag=SUCCESS;
				break;
			}
			
		}
		
	}
	#endif
	
	return returnflag;
}
