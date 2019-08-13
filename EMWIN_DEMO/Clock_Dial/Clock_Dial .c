#include "Clock_dial .h"

/*
    ��ɫ���ó����� ����������ʾ����Ҫ����ɫ ������Щ�鷳����Ҫһ��һ��������
    
*/
u8 tite[]="���չ���";
u8 hang1[]="���ߣ���������������";
u8 hang2[]="��ݣ������������";
u8 hang3[]="�ߴ磺�ߴ�ߴ�ߴ��";
u8 hang4[]="��������������������";
char http[]="http://www.zhongketianhe.com.cn";
u32  tite_color=0x79F8F800;
u32  text_color=0x00FFFFFF;

GUI_HMEM hMem;
/*
*	��������map
*	�����������ת�������� fromStart fromEnd toStart  toEnd
*	����ֵ����toEnd-toStart�÷�Χ��������� 
*	�������ܣ����ݵķ�Χӳ��  
*	���ӣ�j=map(shi,0,100,230,555);  ��0��100��Χ֮�ڵ�����shiӳ�䵽��Χ230��555֮�䣬��󽫺����ķ���ֵ��ֵ������j
*/

float map(float value,float fromStart,float fromEnd,float toStart,float toEnd)//��value��ֵ��fromEnd-fromStart ӳ�䵽 toEnd-toStart
{
	float num;
	num = (value*(toEnd - toStart)) / (fromEnd - fromStart);
	return num;
}
/*
*	��������_Draw
*	�������ܣ����Ʊ�����ָ�� �Լ���ʾ���� 
*/

#if Clock_Dial_num == 1  //����1

static void _Draw(void * p) 
{
	PARAM * pParam;
	pParam = (PARAM *)p;
	if(pParam->AutoDevInfo.DrawFixed) 
	{
		GUI_ClearRect(15,160,314,160);//ǰ������������Ҫ��ʾͼƬ��ʼλ�õ�ͼƬ����������������ͼƬ�ĳߴ�+��ʼλ�õ�����
		draw_bitmap(0,0);   //����λͼ
	}	
	GUI_SetColor(0x0f9309);
	GUI_AA_FillPolygon(pParam->aPoints, countof(_aNeedle), MAG*168,  MAG *109 );//����ʵ���޾�ݶ����
	GUI_Delay(5);//��һ�������б�Ҫ����
	GUI_SetColor(0x22AD38);
	GUI_AA_FillPolygon(pParam->aPoints1, countof(_aNeedle1), MAG*407, MAG * 102);//����ʵ���޾�ݶ����
}

/*
*	��������Creat_device
*	������������������¶��Լ�ʪ������
*	�������ܣ�ָ�����ת ����ʪ�����ݵ���ʾ
*/
void Creat_device(float wen,float shi) 
{
	int i,j,t=0;
	GUI_AUTODEV AutoDev;      /* �ڴ��豸 */
	PARAM       Param;        /* PARAM���ͱ��� */
	GUI_MEMDEV_CreateAuto(&AutoDev);//�����Զ��豸����
	for(t=0;t<=1;++t)
	{
		if(wen>0&&wen<=80)  i=(map(wen,0,80,350,520))-10; //0��240�������ķ�Χ   0��82�� -40��0������ӳ�䷶Χ  82��240��0��80������ӳ��
		else       i=(map(wen,-40,0,265,350))-8; 
		if(shi>=100) shi=100;
		j=map(shi,0,100,230,555);   //��Χ 0-360
		Param.Angle= i;
		Param.Angle1=j+230;
		Param.Angle =((210-Param.Angle)* 3.1415926f) / 180;
		Param.Angle1 =((210-Param.Angle1)* 3.1415926f) / 180;
		GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);//��ָ���Ƕ���ת�����
		GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);//��ָ���Ƕ���ת�����
        GUI_SetFont(&GUI_Font32B_ASCII);
        GUI_SetColor(GUI_GREEN);		
        GUI_SetTextMode(GUI_TM_TRANS);
		if(wen<0)
		{
			GUI_GotoXY(65,20);					//�ƶ����
		    GUI_DispFloat(wen,4);			//��������ӵ���ʾ����
		}
		else
		{
			GUI_GotoXY(67,20);					//�ƶ����
		    GUI_DispFloat(wen,5);			//��������ӵ���ʾ����
		}
        GUI_SetTextMode(GUI_TM_TRANS);
		GUI_GotoXY(300,20);					//�ƶ����
		GUI_DispFloat(shi,5);			//��������ӵ���ʾ����
		GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &_Draw, &Param);//ʹ��GUI_AUTODEV ������л���	
	}
}

void MainTask(void)
{
	wenshi_display();
	Creat_device(tem,hum);//ָ�����ת ����ʪ�����ݵ���ʾ
	printf("Maintask a=%f  b=%f  \r\n",a,b);
}

void Draw_Qr_code(void)
{
	GUI_HMEM hMem;
	GUI_DrawGradientH(324,164,450,290,GUI_GRAY,GUI_GRAY);
//	hMem = GUI_QR_Create(http, 5, GUI_QR_ECLEVEL_L, 2);
	//GUI_QR_Draw(hMem, 325, 165);
//	GUI_QR_Delete(hMem);//�ͷŴ洢��ά����ڴ�ռ�
	Show_StrAndChinese(25,160,tite_color,0,tite,32,1);	
	Show_StrAndChinese(60,200,text_color,0,hang1,24,1);
	Show_StrAndChinese(60,230,text_color,0,hang2,24,1);
	Show_StrAndChinese(60,260,text_color,0,hang3,24,1);
	Show_StrAndChinese(60,290,text_color,0,hang4,24,1);
	GUI_AA_EnableHiRes();//���ø߷ֱ�������
	GUI_AA_SetFactor(MAG);//���õ�ǰ���������

}
#endif

#if Clock_Dial_num == 2  //����2  ��Ҫд��
static void _Draw(void * p) 
{
	PARAM * pParam;
	pParam = (PARAM *)p;
	if(pParam->AutoDevInfo.DrawFixed) 
	{
		GUI_ClearRect(15,160,314,160);//ǰ������������Ҫ��ʾͼƬ��ʼλ�õ�ͼƬ����������������ͼƬ�ĳߴ�+��ʼλ�õ�����
		draw_bitmap(0,0);   //����λͼ
	}	
	GUI_SetColor(0x0f9309);
	GUI_AA_FillPolygon(pParam->aPoints, countof(_aNeedle), MAG*168,  MAG *109 );//����ʵ���޾�ݶ����
//	GUI_Delay(5);//��һ�������б�Ҫ����
	GUI_SetColor(0x22AD38);
	GUI_AA_FillPolygon(pParam->aPoints1, countof(_aNeedle1), MAG*407, MAG * 102);//����ʵ���޾�ݶ����
}

/*
*	��������Creat_device
*	������������������¶��Լ�ʪ������
*	�������ܣ�ָ�����ת ����ʪ�����ݵ���ʾ
*/
void Creat_device(float wen,float shi) 
{
	int i,j,t=0;
	GUI_AUTODEV AutoDev;      /* �ڴ��豸 */
	PARAM       Param;        /* PARAM���ͱ��� */
	GUI_MEMDEV_CreateAuto(&AutoDev);//�����Զ��豸����
	for(t=0;t<=1;++t)
	{
		if(wen>0&&wen<=80)  i=(map(wen,0,80,350,520))-10; //0��240�������ķ�Χ   0��82�� -40��0������ӳ�䷶Χ  82��240��0��80������ӳ��
		else       i=(map(wen,-40,0,265,350))-8; 
		if(shi>=100) shi=100;
		j=map(shi,0,100,230,555);   //��Χ 0-360
		Param.Angle= i;
		Param.Angle1=j+230;
		Param.Angle =((210-Param.Angle)* 3.1415926f) / 180;
		Param.Angle1 =((210-Param.Angle1)* 3.1415926f) / 180;
		GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);//��ָ���Ƕ���ת�����
		GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);//��ָ���Ƕ���ת�����
        GUI_SetFont(&GUI_Font32B_ASCII);
        GUI_SetColor(GUI_GREEN);		
        GUI_SetTextMode(GUI_TM_TRANS);
		if(wen<0)
		{
			GUI_GotoXY(65,20);					//�ƶ����
		    GUI_DispFloat(wen,4);			//��������ӵ���ʾ����
		}
		else
		{
			GUI_GotoXY(67,20);					//�ƶ����
		    GUI_DispFloat(wen,5);			//��������ӵ���ʾ����
		}
        GUI_SetTextMode(GUI_TM_TRANS);
		GUI_GotoXY(300,20);					//�ƶ����
		GUI_DispFloat(shi,5);			//��������ӵ���ʾ����
		GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &_Draw, &Param);//ʹ��GUI_AUTODEV ������л���	
	}
}

void MainTask(void)
{
	wenshi_display();
	Creat_device(tem,hum);//ָ�����ת ����ʪ�����ݵ���ʾ
	printf("Maintask a=%f  b=%f  \r\n",tem,hum);
}

void Draw_Qr_code(void)
{
	GUI_HMEM hMem;
	GUI_DrawGradientH(324,164,450,290,GUI_GRAY,GUI_GRAY);
//	hMem = GUI_QR_Create(http, 5, GUI_QR_ECLEVEL_L, 2);
	//GUI_QR_Draw(hMem, 325, 165);
 // GUI_QR_Delete(hMem);//�ͷŴ洢��ά����ڴ�ռ�
//	GUI_AA_EnableHiRes();//���ø߷ֱ�������
//	GUI_AA_SetFactor(MAG);//���õ�ǰ���������
}
#endif



void Time_Show(void)
{
    char buf[30];//    char buf2[30];
    sprintf(buf, "%0.2d/%0.2d/%0.2d  %0.2d:%0.2d:%0.2d",calendar.w_year,calendar.w_month,calendar.w_date, calendar.hour, calendar.min,calendar.sec);
    GUI_SetFont(&GUI_Font20_ASCII);
	GUI_SetColor(GUI_YELLOW);
	GUI_DispStringHCenterAt(buf,390,300);
    
}



