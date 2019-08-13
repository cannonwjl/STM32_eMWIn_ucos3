#include "include.h"

//��ʼ����
#define START_TASK_PRIO				3//�������ȼ�
#define START_STK_SIZE 				128//�����ջ��С	
OS_TCB StartTaskTCB;//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];//�����ջ	
void start_task(void *p_arg);//������

//ϵͳ״̬��ѭ������
#define SYS_TASK_PRIO				4//�����������ȼ�
#define SYS_STK_SIZE				512//�����ջ��С
OS_TCB SYSTaskTCB;//������ƿ�
CPU_STK SYS_TASK_STK[SYS_STK_SIZE];//�����ջ
void sys_task(void *p_arg);//ϵͳ״̬��ѭ������

//���ݷ�������
#define SEND_TASK_PRIO 				16//�����������ȼ�
#define SEND_STK_SIZE				128//�����ջ��С
OS_TCB SendTaskTCB;//������ƿ�
CPU_STK SEND_TASK_STK[SEND_STK_SIZE];//�����ջ
void send_task(void *p_arg);//send����

//���ݲɼ�����
#define COLLECT_TASK_PRIO 				5//�����������ȼ�
#define COLLECT_STK_SIZE				512//�����ջ��С
OS_TCB CollectTaskTCB;//������ƿ�
CPU_STK COLLECT_TASK_STK[COLLECT_STK_SIZE];//�����ջ
void collect_task(void *p_arg);//send����

//ι������
#define FEEDDOGS_TASK_PRIO 				7//�����������ȼ�
#define FEEDDOGS_STK_SIZE				128//�����ջ��С
OS_TCB FeeddogsTaskTCB;//������ƿ�
CPU_STK FEEDDOGS_TASK_STK[FEEDDOGS_STK_SIZE];//�����ջ
void feeddogs_task(void *p_arg);//send����

//EMWINDISPLAY  Һ������ʾ
#define EMWINDISPLAY_TASK_PRIO			6//�����������ȼ�
#define EMWINDISPLAY_STK_SIZE			2048//�����ջ��С
OS_TCB EmwindisplayTaskTCB;//������ƿ�
CPU_STK EMWINDISPLAY_TASK_STK[EMWINDISPLAY_STK_SIZE];//�����ջ
void emwindemo_task(void *p_arg);//emwindemo_task����

//LED
#define LED_TASK_PRIO			9//�����������ȼ�
#define LED_STK_SIZE			128//�����ջ��С
OS_TCB LEDTaskTCB;//������ƿ�
CPU_STK LED_TASK_STK[LED_STK_SIZE];//�����ջ
void LED0_task(void *p_arg);//LED_task����

//DS1302  ʱ������
#define DS1302_TASK_PRIO			10//�����������ȼ�
#define DS1302_STK_SIZE			128//�����ջ��С
OS_TCB DS1302TaskTCB;//������ƿ�
CPU_STK DS1302_TASK_STK[DS1302_STK_SIZE];//�����ջ
void DS1302_task(void *p_arg);//emwindemo_task����

//M25P80  �ڴ�����
#define M25P80_TASK_PRIO			13//�����������ȼ�
#define M25P80_STK_SIZE			128//�����ջ��С
OS_TCB M25P80TaskTCB;//������ƿ�
CPU_STK M25P80_TASK_STK[DS1302_STK_SIZE];//�����ջ
void M25P80_task(void *p_arg);//emwindemo_task����


//u32 address2=256;
//uint8_t ceshitr_buff[100];
//uint8_t ceshire_buff[100];



/* USER CODE BEGIN Includes */
u32 address1=0;
u32 address2=256;
u32 address3=0;

uint8_t *Time;
uint8_t save2[] = "**http://www.eepw.com.cn/ Thank You!!**--**The Inside Eeprom Test**";
uint8_t Buffer2[sizeof(save2)];
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
//extern SPI_HandleTypeDef SpiHandle;
/* Buffer used for transmission */
uint8_t aTxBuffer[10] = {8,9,0,15,2,6,6,6,6};//����
uint8_t  pBuffer7[10];
uint8_t ceshitr_buff[256];
uint8_t ceshire_buff[256];

unsigned long time1[3]={0};//����ʱ��
/* Buffer used for reception */
/* Private function prototypes -----------------------------------------------*/


u8 WIFI_RX_BUF[100];
u8 WIFI_RX_LEN;
extern u8 USART1_RX_BUF[USART_REC_LEN]; 
extern u16 USART1_RX_STA;       //����״̬���	 
extern u16 USART1_RX_count;
extern u8 save_next_addr;				
extern	u16 save_next_addr_page;		
extern	u8 save_next_addr_sect;  

extern	u8 send_next_addr;			
extern	u16 send_next_addr_page;	
extern	u8 send_next_addr_sect;	
extern  u8  FLASH_full_flag;// 
extern u8 WIFI_or_RF_SendMode;

/*=================================================================================*/
u8 WakeUp_wIWDG_Feed_time=20;//20Sι��һ�Ρ�
extern u16 RetryRegisterTime;//�ϵ��һ��ע��ʧ��(RTC û��ʱ��) ����ע��ʱ�� (12s) ==����֮���ע������ Re_Registration_time
extern u8 HARDWARE_ERROR_sleeptime;//(S)����HARDWARE_ERROR���ߵ�ʱ�� ʱ�䵽�˼���ѹ
 u8 SEND_Finish_WaitTime=8;//������ɵȴ��ر���ʱ�� *100ms 
extern u8 Wait_WIFI_ConnectTime;//�ȴ�WIFI���Ϸ�������ʱ�� ��WIFI����ʱ�䣩 *100ms
extern u8 WIFI_RETRY_SEC;//WIFI���ӷ������ط��Ĵ���
extern uint8_t HCHO_NeedCollectTime;
extern uint8_t COZ_NeedCollectTime;

/*==================================================================================*/

int flag_wwdg;
u8 event;
u16 data;
//u8 sys_state=WAKE_UP;
//SensorEcho se;

//u16 ADC_read_time=ADC_collect_sec_MAX;//ϵͳ�� ADC_collect_sec_MAX*10=50S �ɼ�ADCһ��
u8 clock_adjust_num=0;//ʱ��У׼

//u8 RF_is_rece=RF_RECEIVE_DISABLE;//RF�Ƿ����

u8 FistSend_and_RetrySend_TimeInterval;//��һ�η��ͺ��ط���ʱ����
//u8 Is_register_flag=0;//ע���Ƿ�ɹ���־λ 0���ɹ� 1��ʾ�ɹ�
u8 RF_PACK_IN_FLAG=0;
u8 IS_RTC_TIME_LOST=0;//�ж�RTCʱ���Ƿ���� 1������

u8 USART_RECE_BUF[USART_REC_LEN]; 
u8 USART_RECE_BUF_count=0;

extern u8 RF_Rece_Buff[200];
extern u16 RF_Rece_Lenth;

extern u8 sink_id;//��Ҫ �����Ǹ����ص� ע��֮���ȡҪ����
extern u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
extern u8 USART2_RX_BUF[USART_REC_LEN]; 
extern u16 USART_RX_STA;       //����״̬���	 
extern u16 USART2_RX_STA;       //����״̬���	 
extern u8 USART2_RX_count;
extern u16 USART_RX_count;
extern u16 USARTl1_RX_count;
extern u8 USARTl1_RX_BUF[USART_REC_LEN]; 
extern u8 aRxBuffer[1];//HAL��ʹ�õĴ��ڽ��ջ���
extern u8 aRxBuffer2[1];//HAL��ʹ�õĴ��ڽ��ջ���
extern u8 retry_Register_num;//ע�᲻�ɹ� �ط�ע����Ĵ���
extern u32 timestamp;//���������� --ʱ���
//extern u16  collection_time;		//���ݲɼ����� S
extern float minimum_operate_voltage;
extern float uwADCxConvertedValue;
extern u8  display_mode;

u32 Last_retryRegisterTime;//��һ������ע���ʱ��
u32 Next_retryRegisterTime;//��һ������ע���ʱ��
u32 Last_HARDWARE_ERROR_sleeptime;//��һ��ʧ��֮���ʱ��
u32 Next_HARDWARE_ERROR_sleeptime;//��һ��ʧ��֮���ʱ��

//u32 Last_CollectTime;//�ϴ�һ�δ������ɼ�ʱ���
//u32 Next_CollectTime;//��һ�δ������ɼ�ʱ���

//u32 Last_SendpacketTime;//��һ�η���ʱ��
//u32 Next_SendpacketTime;//��һ�η���ʱ��
//u32 Last_LcdrefreshTime;//��һ��Һ��ˢ��ʱ��
//u32 Next_LcdrefreshTime;//��һ��Һ��ˢ��ʱ��

//u32 Last_LcdCollectTime;
//u32 Next_LcdCollectTime;

//u32 Last_HchoCollectTime;
//u32 Next_HchoCollectTime;
//u8 HCHO_start_collect_flag=0;
//u32 HCHO_startCollect_time=0;

//u32 Last_CozCollectTime;
//u32 Next_CozCollectTime;
//u8 COZ_start_collect_flag=0;
//u32 COZ_startCollect_time=0;

//u8 LCD_DISPLAY_SWITCH_FLAG=0;

extern u16  Re_Registration_time;

//extern u32  Last_CCS811Collect_time;
//extern u32  Next_CCS811Collect_time;
//extern u32  CCS811_collection_time;
//extern u8 CCS811_State;
//extern u32  CCS811_startCollect_time;
//extern u16 ccs811_collect_need_time;
//extern u8 CCS811_start_collect_flag;
//extern u8  CCS811_startCollect_Init_flag;
u32 Next_ReRegistratTime;//��һ������ע��ʱ��
u32 Last_GettimeEcho_timestamp;//�°�Э���ȡʱ���ʱ���
u32 Next_GettimeEcho_timestamp;
extern u32 GettimeEcho_time;  //�°�Э���ȡʱ��ע�������
u8  GettimeEcho_time_flag=0;

//extern	u8 HCHO_State; //HCHO�Ƿ���
//extern	u32 HCHO_collection_time;
//	
//extern	u8 COZ_State; //COZ�Ƿ���
//extern	u32 COZ_collection_time;
	

u32 Last_FeedTime;//��һ��ι��ʱ��
u32 Next_FeedTime;//��һ��ι��ʱ��
//u32 GatewayTime;//���صõ���ʱ���
extern uint8_t CurrentYear,CurrentDate,CurrentMonth,CurrentHour,CurrentMinute,CurrentSecond;//Gettime ֮��ʱ����ı���
extern uint8_t RTC_CurrentYear,RTC_CurrentDate,RTC_CurrentMonth,RTC_CurrentHour,RTC_CurrentMinute,RTC_CurrentSecond;//RTC_TimeShow();֮���ϵͳʱ��
u8 EVENT_TIMER_num=0;
u8 EVENT_TIMER_num2=0;
u8 GATEWAY_REGISTER_SEC=0;//GATEWAY_REGISTER�ȴ���ע���ʱ�� 
u8 GATEWAY_REGISTER_retrynum=0;//�ط�ע�������
u8 RF_SENDING_sec;//rf�����ݰ��ȴ��ļ���
u8 WIFI_SENDING_sec;//�ȴ�WIFI���ϵļ���
u8 WIFI_SENDING_sec;//�ȴ�WIFI���ϵļ���
u8 WIFI_RETRY_sec;//WIFI���µȴ�����

u8 RETRY_REGISTER_retrynum=0;//�ط�ע�������
u8 RETRY_REGISTER_sec=0;//r
u8 RF_SENDING_retry_num;//rf�����ݰ��ȴ���ʱ��
extern	u8 retry_senddata_num;//�������ݲ��ɹ� ���·��͵Ĵ���
u16 vel=0;
u8 last_time_send_fail=0;//�ϴη��Ͳ��ɹ�
u8  RF_firstSend_flag;//433����ʱ��һ�η��ͱ�־λ  ��һ�η���ֱ�ӷ��� ���ȴ� �Ժ�ȴ������ٳ���
u8  RF_firstSendecho_flag;

u8 RF_config_flag=0;//RF���ñ�־��1����ʼ���� ��ʱֻ�ж��ַ���
u8 RF_configP[6]={0xc0,0x00,0x00,0x1A,0x02,0x44};
char rf_read_buff[10];	
	
char uart_rx_buf[50]; //���ý��ջ���
int uart_rx_cnt; //��¼�����ֽ���
	
void Delay_S(u16 S);
	int RF_ReadParametr(void);//��RF���� ���Զ�� �ɹ�����
	int RF_ConfigureParameter(void);//дRF���� ����10�� �ɹ�����
extern u32 tim_time;//��ʱ��ʱ��
u32 wakeup_timestamp;
u8 first_wakeup_flag;//�������������ı�־λ
u8 first_wakeup_flag1;//�������������ı�־λ
u8  Start_collect_flag=0;
extern u8 need_collect_flag;
u8 NETWORK_ERROR_sleep_time;
u16 RF_RX_LEN;
u8 rfrece_flag;
u8 Next_WakeUp_need_echo_flag=0;//�´�������Ҫ����ע��

u16 rece_seqnum_V2;//�°�Э���յ�����
u16 send_seqnum_V2;
u8 sys_first_start=0;
extern u8 SysStart_mode_flag;
extern u16 ConfigureMode_timeout;//��������ģʽ ��ʱʱ�� ��λS
extern u16 ConfigureMode_IWDG_Feed_time;
u8 *Chip_ID;
extern u32 Address_ID;
//extern  uint8_t  CCS811_HaveOrNot_State;
extern u8 Need_COnfig_WIFI_Flag;
u8 WIFI_rece_mode_flag=0;//WIFI���ܵ����������ã����ò���������������
extern uint8_t WIFI_Send_TIME_Range[4];
u8 Last_WIFI_WorkState=0;//�ϴ�WIFI������״̬
//u8 check_wifi_send_or_not_flag;//WIFI�Ƿ�����־λ
u8 sleep_flag=0;
extern u8 Config_By_WifiOrUart;//������ͨ������ ����WIFI
extern u16 Config_timeS;
u8 NETWORK_ERROR_num=0;
u8 NETWORK_ERROR_flag=0;
u8 NETWORK_ERROR_finsh_flag=0;
//extern uint16_t co2v_flash;
extern uint16_t co2v;
void Collect_or_Scan(void);
//u8 CCS811_Collect_num=0;
//u8 CCS811_CollectOK_num=0;
//u16 CCS811_e[4];
//u16 CCS811_t[4];
extern uint16_t tvocv,eco2v;



int main(void)
{	
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init();	    	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		/*CRC��emWinû�й�ϵ��ֻ������Ϊ�˿�ı��������ģ�����STemWin�Ŀ�ֻ������ST��оƬ���棬���оƬ���޷�ʹ�õġ� */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//��֪��Ϊʲôһ��Ҫ��crcʱ��	
	uart1_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	//LED_Init();
	LCD_GPIOInit();
	LCD_ShowString(30,40,210,24,24,"STM32F103VGT6 ^_^"); 
	//printf(" running here!! ");
	FSMC_SRAM_Init();		//��ʼ��SRAM
	my_mem_init(SRAMIN); 	//��ʼ���ڲ��ڴ��
//	my_mem_init(SRAMEX);  	//��ʼ���ⲿ�ڴ��
  //parameter_init();
//  sensor_init();
//	DS1302_Init();
//	SPI_FLASH_Init();
//    M25P80_PWEN_ON;
//	SHT3X_Init(0x44); 
	//wenshi_display();
   // SpiInit( );
//	 LCD_GPIOInit();
	//OSInit(&err);
	//��ʱ��ʹ��WIFI ���ҳ���Ῠס  WIFI_or_RF_SendMode����û�ҵ�
//	
//   M25P80_SLEEP();
//	 if(WIFI_or_RF_SendMode==WIFI_SEND)
//	{
//	//	MX_USART4_UART_Init();
//		WIFI_PWEN_ON;
//		printf(" WIFIInit ");
//	}
////	LED_GREEN_OFF;
//		Last_LcdrefreshTime=timestamp;
//		Next_LcdrefreshTime=Last_LcdrefreshTime+28;//������´�ˢ������
//		Last_LcdCollectTime=timestamp;
//		Next_LcdCollectTime=Last_LcdCollectTime+lcd_time;//������´�ˢ������
	
	delay_ms(1000);//������ һ��Ҫ��	 һ��Ҫ��write_startmode_normal_eeprom();֮ǰ
	write_startmode_normal_eeprom();

	SysStart_mode_flag=0;
    
    OSInit(&err);			//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���
	//	printf(" running here!! ");
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ

							//	 printf(" running here1!! ");

								 OS_CRITICAL_EXIT();	//�˳��ٽ���

								// printf(" running here2---exit!! ");	
								 
								 OSStart(&err);  //����UCOSIII  ���� ����������ִ�� �������ӡ  running here3--osstart!! 

								 	printf(" running osstart!! ");
								 
								 while(1);
		

}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
	
	printf(" running here-CPU_Init��!! ");
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	OS_CRITICAL_ENTER();	//�����ٽ���
#if LCD_show > 0u  //�Ƿ�ʹ����ʾ�� 
	
	//LCD_Init();
	
//	LCD_Init9486();			//LCD��ʼ��
	GUI_Init();  			//STemWin��ʼ��
	printf(" running LCD_Init9486��!! GUI_Init value:%d ",GUI_Init());
	OSTaskCreate((OS_TCB*     )&EmwindisplayTaskTCB,		//Note_WX ���ж�
				 (CPU_CHAR*   )"Emwindisplay task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDISPLAY_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDISPLAY_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDISPLAY_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDISPLAY_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);

#endif

	//ϵͳ״̬��ѭ������
	OSTaskCreate((OS_TCB*     )&SYSTaskTCB,		
				 (CPU_CHAR*   )"SYS task", 		
                 (OS_TASK_PTR )sys_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )SYS_TASK_PRIO,     
                 (CPU_STK*    )&SYS_TASK_STK[0],	
                 (CPU_STK_SIZE)SYS_STK_SIZE/10,	
                 (CPU_STK_SIZE)SYS_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//send����
	OSTaskCreate((OS_TCB*     )&SendTaskTCB,		
				 (CPU_CHAR*   )"send task", 		
                 (OS_TASK_PTR )send_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )SEND_TASK_PRIO,     
                 (CPU_STK*    )&SEND_TASK_STK[0],	
                 (CPU_STK_SIZE)SEND_STK_SIZE/10,	
                 (CPU_STK_SIZE)SEND_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
								 
		//�ɼ�����
	OSTaskCreate((OS_TCB*     )&CollectTaskTCB,		
				 (CPU_CHAR*   )"send task", 		
                 (OS_TASK_PTR )collect_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )COLLECT_TASK_PRIO,     
                 (CPU_STK*    )&COLLECT_TASK_STK[0],	
                 (CPU_STK_SIZE)COLLECT_STK_SIZE/10,	
                 (CPU_STK_SIZE)COLLECT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//ι������
	OSTaskCreate((OS_TCB*     )&FeeddogsTaskTCB,		
				 (CPU_CHAR*   )"send task", 		
                 (OS_TASK_PTR )feeddogs_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )FEEDDOGS_TASK_PRIO,     
                 (CPU_STK*    )&FEEDDOGS_TASK_STK[0],	
                 (CPU_STK_SIZE)FEEDDOGS_STK_SIZE/10,	
                 (CPU_STK_SIZE)FEEDDOGS_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
	

////DS1302  ʱ������
//#define DS1302_TASK_PRIO			10//�����������ȼ�
//#define DS1302_STK_SIZE			128//�����ջ��С
//OS_TCB DS1302TaskTCB;//������ƿ�
//CPU_STK DS1302_TASK_STK[DS1302_STK_SIZE];//�����ջ
//void DS1302_task(void *p_arg);//emwindemo_task����								 
								 //DS1302����
	OSTaskCreate((OS_TCB*     )&DS1302TaskTCB,		
				 (CPU_CHAR*   )"DS1302 task", 		
                 (OS_TASK_PTR )DS1302_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )DS1302_TASK_PRIO,     
                 (CPU_STK*    )&DS1302_TASK_STK[0],	
                 (CPU_STK_SIZE)DS1302_STK_SIZE/10,	
                 (CPU_STK_SIZE)DS1302_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	
								 
								 //EMWINDISPLAY  Һ������ʾ
//#define LED_TASK_PRIO			8//�����������ȼ�
//#define LED_STK_SIZE			128//�����ջ��С
//OS_TCB LEDTaskTCB;//������ƿ�
//CPU_STK LED_TASK_STK[LED_STK_SIZE];//�����ջ
//void LED0_task(void *p_arg);//emwindemo_task����

	//LED0����
	OSTaskCreate((OS_TCB*     )&LEDTaskTCB,		
				 (CPU_CHAR*   )"LED0 task", 		
                 (OS_TASK_PTR )LED0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK*    )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);									 
				printf("\r\ here LED0_create-------------------------main.c\r\n");		
				printf("\r\ --------------------------------------------\r\n");	
				printf("\r\ --------------------------------------------\r\n");	
									 
		//M25P80  �ڴ�����
//#define M25P80_TASK_PRIO			10//�����������ȼ�
//#define M25P80_STK_SIZE			512//�����ջ��С
//OS_TCB M25P80TaskTCB;//������ƿ�
//CPU_STK M25P80_TASK_STK[DS1302_STK_SIZE];//�����ջ
//void M25P80_task(void *p_arg);//emwindemo_task����
						 
	//M25P80����
	OSTaskCreate((OS_TCB*     )&M25P80TaskTCB,		
				 (CPU_CHAR*   )"M25P80 task", 		
                 (OS_TASK_PTR )M25P80_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )M25P80_TASK_PRIO,     
                 (CPU_STK*    )&M25P80_TASK_STK[0],	
                 (CPU_STK_SIZE)M25P80_STK_SIZE/10,	
                 (CPU_STK_SIZE)M25P80_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);									 
			
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//EMWINDISPLAY����
void emwindemo_task(void *p_arg)
{
	OS_ERR err;
	u8 emwindemo_num=0;//������¼����ѭ������
	p_arg = p_arg;
	//Draw_Qr_code();��
	//����
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	//printf("\r\ here is emwindemo_task-------------------------main.c\r\n");
	while(1)
	{
		
		display_2d();
		GUI_Delay(2000);
		alpha_display();
		GUI_Delay(2000);
		draw_polygon();
		GUI_Delay(2000);
		
		
//		LCD_ShowString(430,40,210,24,24,"emwindemo_task running!!");
//    LCD_ShowxNum(650,40,emwindemo_num,5,24,0);			
//		LCD_ShowString(30,40,24,"STM32F103VGT6 ^_^",1); 
//		LCD_ShowString(30,150,16,"123",0);
		//void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u8 mode)
	//	MainTask();//���ԶԲ���
	//	Draw_Qr_code();
		emwindemo_num++;
		printf("\r\nemwindemo_task=%d\r\n",emwindemo_num);
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}


void LED0_task(void *p_arg)
{
	OS_ERR err;
	u8 LED_num=0;   //������¼����ѭ������
	p_arg = p_arg;
	
	while(1)
	{

		LED_num++;
//		LCD_ShowString(430,70,210,24,24,"LED0_task running!!");
//    LCD_ShowxNum(650,70,LED_num,5,24,0);	
		LED0=!LED0;
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_DLY,&err);//��ʱ500ms
		printf("----------------LED---------------main.c \t \n");
	}
}

void sys_task(void *p_arg)//ϵͳ״̬��ѭ������
{
	OS_ERR err;
    u8 t=0;	
	u8 sys_task_num=0; //������¼����ѭ������
	p_arg = p_arg;
//    DS1302_Init();
    t=RTC_Init();
    printf("\r\n SYS_task \r\n");
	while(1)
	{
    
//		LCD_ShowString(430,110,210,24,24,"SYS_task running!!");
//    LCD_ShowxNum(650,110,sys_task_num,5,24,0);	
		sys_task_num++;
   //     Time_Show();
		printf("\r\nsys_task=%d\r\n",sys_task_num);
//		if(t!=calendar.sec)
//		{
//			t=calendar.sec;
//            printf("\r\n year= %d  yue=%d  ri=%d\r\n",calendar.w_year,calendar.w_month,calendar.w_date);
//            printf("\r\n hour= %d  min=%d  sec=%d\r\n",calendar.hour,calendar.min,calendar.sec);
//		}	       
  // LED0=!LED0;  
		OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_PERIODIC,&err);//��ʱ5ms
	}
}


//send����  ���ݷ���
void send_task(void *p_arg)
{
	OS_ERR err;
	u8 send_task_num=0;
	p_arg = p_arg;
	while(1)
	{
//		LCD_ShowString(430,140,210,24,24,"send_task running!!");
//    LCD_ShowxNum(650,140,send_task_num,5,24,0);	
		send_task_num++;
		printf("\r\nsend_task=%d\r\n",send_task_num);
		OSTimeDlyHMSM(0,0,0,300,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}
//collect����  ���ݲɼ��洢
extern float tem,hum;
void collect_task(void *p_arg)
{
//	u32 id; 
	OS_ERR err;
//	float *tem,*hum;
	char result2[15],result3[15];
	u8 collect_num=0;
////	p_arg = p_arg;
	SHT3X_Init(0x44); // Address: 0x44 = Sensor on EvalBoard connector   0x45 = Sensor on EvalBoard
////	SPI_FLASH_Init();

////		printf("\r\n running here----- collect_task\r\n");
//////	id=SPI_FLASH_ReadID();
////	M25P80_Bulk_Erase();
////     parameter_init();
	while(1)
	{
//        M25P80_task();
   //   getFlashtypeID();
//		LCD_ShowString(430,170,210,24,24,"collect_task running!!");
//    LCD_ShowxNum(650,170,collect_num,5,24,0);
		
			wenshi_display();
		//SHT3X_ReadMeasurementBuffer(tem,hum);
			sprintf(result2,"tem:=%f",tem);
	sprintf(result3,"hum:=%f",hum);
//	 LCD_ShowString(0,130,100,24,24,result2);
//	 LCD_ShowString(150,130,100,24,24,result3);
		collect_num++;
		printf("\r\ncollect_task=%d\r\n",collect_num);
		OSTimeDlyHMSM(0,0,1,000,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}
//feeddogsι������  
void feeddogs_task(void *p_arg)
{
	OS_ERR err;
	u8 feeddogs_num=0;
	p_arg = p_arg;
	while(1)
	{
//		LCD_ShowString(430,200,210,24,24,"feeddogs_task running!!");
//    LCD_ShowxNum(650,200,feeddogs_num,5,24,0);
		feeddogs_num++;
		printf("\r\nfeeddogs_task=%d\r\n",feeddogs_num);
		OSTimeDlyHMSM(0,0,0,600,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}
//DS1302����
void DS1302_task(void *p_arg)
{
	u8 time[15];
	int hour;
	char result[15],result1[15];
	OS_ERR err;
	u8 DS1302_num=0;
	p_arg = p_arg;
while(1)
	{
		
		
		DS1302_num++;
//		LCD_ShowString(430,230,210,24,24,"DS1302_task running!!");
//    LCD_ShowxNum(650,230,DS1302_num,5,24,0);
		DS1302_Get_Time(time);
		hour=(time[9]*10+time[10])%12; if(hour==0) hour=12; //�򵥵�ȡģ ��24Сʱ�Ƹ�12Сʱ
		sprintf(result, "%d%d%d%d-%d%d-%d%d",time[0],time[1],time[2],time[3],time[4],time[5],time[6],time[7]);  //year
		sprintf(result1, "%d-%d%d-%d%d\n",hour,time[11],time[12],time[13],time[14]);
		LCD_ShowString(30,160,210,24,24,"time!!!"); 
		LCD_ShowString(30,70,210,24,24,result); 
		LCD_ShowString(30,100,210,24,24,result1);
		printf("%d%d%d%d��%d%d��%d%d�� ", time[0],time[1],time[2],time[3],time[4],time[5],time[6],time[7]);
		printf("%d%dʱ%d%d��%d%d�� ����%d\n", time[9],time[10],time[11],time[12],time[13],time[14],time[8]);
		OSTimeDlyHMSM(0,0,0,700,OS_OPT_TIME_PERIODIC,&err);//��ʱ500ms
	}
}



u8 TEXT_READ_Buffer[30];
uint8_t Tx_Buffer[5] = {1,2,3,4,5};
uint8_t Rx_Buffer[5] = {0};
//uint8_t Tx_Buffer1[30]={0};
/* ��ȡ�������ĳ��� */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)

#define  BufferSize (countof(Tx_Buffer)-1)
#define  FLASH_WriteAddress     0x2000  //ע������ĵ�ַ��Ҫ���� 

#define  FLASH_ReadAddress      FLASH_WriteAddress 
#define  FLASH_SectorToErase    FLASH_WriteAddress


void M25P80_task(void *p_arg)
{

	OS_ERR err;
	u16 id;
	u8 M25P80_read[10];	
	u8 M25P80_num=0;   //������¼����ѭ������
	uint8_t k,M25P80_ON_OFF_flag;
	p_arg = p_arg;
	M25P80_ON_OFF_flag=0;
	while(1)
	{

		
			if(M25P80_ON_OFF_flag==0)
	{
		 LCD_ShowString(30,240,210,24,24,"M25P80_OFF");
	}
	else
	{
	//M25P80_RES();
		//M25P80_DEEP_SLEEP();
//	M25P80_Section_Erase(FLASH_WriteAddress);
	M25P80_Bulk_Erase();
	for(k=0; k<10; k++ )
	delay_ms(1000);
	id=SPI_FLASH_ReadID();
	printf("\r\n IDΪ��%x \r\n", id);
	M25P80_Write_Bytes(FLASH_WriteAddress,Tx_Buffer,BufferSize);
	for( k=0; k<5; k++ )
	{
      printf("\r\n д�������Ϊ��%d \r\n", Tx_Buffer[k]);
	}	
  delay_ms(1000);	
	
	M25P80_Read_Bytes(FLASH_WriteAddress,Rx_Buffer, BufferSize);
   for( k=0; k<5; k++ )
	{
      printf("\r\n ����������Ϊ��%d \r\n", Rx_Buffer[k]);
		sprintf((char*)M25P80_read,"%d,",Rx_Buffer[k]);
	//	strcat(M25P80_read,M25P80_read);
		  LCD_ShowString(30+k*30,240,210,24,24,M25P80_read); 
	}
 }
		
		
//������ʾ������		
		M25P80_num++;
//		LCD_ShowString(430,260,210,24,24,"M25P80_task running!!");
//    LCD_ShowxNum(650,260,M25P80_num,5,24,0);	
		//LED0=!LED0;
		OSTimeDlyHMSM(0,0,0,800,OS_OPT_TIME_DLY,&err);//��ʱ500ms
		printf("---------------M25P80_task running--------------main.c \t \n");
	}
}

//void M25P80_task(void)
//{
//	int k,i;
////   	M25P80_Bulk_Erase();	
//	for( i=0;i<100;i++)
//	{
//		ceshitr_buff[i]=67;
//	}
//		M25P80_Write_Bytes(address2*256*0,ceshitr_buff,256);
//		for( i=0;i<100;i++)
//	{
//		ceshitr_buff[i]=89;
//	}
//		M25P80_Write_Bytes(address2*256*2,ceshitr_buff,256);
//		for( i=0;i<100;i++)
//	{
//		ceshitr_buff[i]=33;
//	}
//		M25P80_Write_Bytes(address2*256*3,ceshitr_buff,256);
//	
//		M25P80_Read_Bytes(address2*256*0,ceshire_buff,256);
//		printf("\r\n deep sleep read flash dizhi: %X \r\n",address2*256*0);
//		for( k=0; k<100; k++ )
//		printf("	%d", ceshire_buff[k]);
//	

//		M25P80_Read_Bytes(address2*256*2,ceshire_buff,256);
//		printf("\r\n deep sleep read flash dizhi: %X \r\n",address2*256*2);
//		for( k=0; k<100; k++ )
//		printf("	%d", ceshire_buff[k]);
//		
//		
//		M25P80_Read_Bytes(address2*256*3,ceshire_buff,256);
//		printf("\r\n deep sleep read flash dizhi: %X \r\n",address2*256*3);
//		for( k=0; k<100; k++ )
//		printf("	%d", ceshire_buff[k]);
//		
//}

