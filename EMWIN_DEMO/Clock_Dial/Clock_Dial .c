#include "Clock_dial .h"

/*
    颜色设置出问题 不过可以显示所需要的颜色 就是有些麻烦，需要一个一个的试试
    
*/
u8 tite[]="彩陶罐子";
u8 hang1[]="作者：作者作者作者作";
u8 hang2[]="年份：年份年份年份年";
u8 hang3[]="尺寸：尺寸尺寸尺寸尺";
u8 hang4[]="质量：质量质量质量质";
char http[]="http://www.zhongketianhe.com.cn";
u32  tite_color=0x79F8F800;
u32  text_color=0x00FFFFFF;

GUI_HMEM hMem;
/*
*	函数名：map
*	输入参数：待转换的数据 fromStart fromEnd toStart  toEnd
*	返回值：在toEnd-toStart该范围里面的数据 
*	函数功能：数据的范围映射  
*	例子：j=map(shi,0,100,230,555);  在0到100范围之内的数据shi映射到范围230到555之间，最后将函数的返回值赋值给变量j
*/

float map(float value,float fromStart,float fromEnd,float toStart,float toEnd)//将value的值从fromEnd-fromStart 映射到 toEnd-toStart
{
	float num;
	num = (value*(toEnd - toStart)) / (fromEnd - fromStart);
	return num;
}
/*
*	函数名：_Draw
*	函数功能：绘制背景和指针 以及显示中文 
*/

#if Clock_Dial_num == 1  //界面1

static void _Draw(void * p) 
{
	PARAM * pParam;
	pParam = (PARAM *)p;
	if(pParam->AutoDevInfo.DrawFixed) 
	{
		GUI_ClearRect(15,160,314,160);//前两个个参数是要显示图片初始位置的图片，后面两个参数是图片的尺寸+初始位置的坐标
		draw_bitmap(0,0);   //绘制位图
	}	
	GUI_SetColor(0x0f9309);
	GUI_AA_FillPolygon(pParam->aPoints, countof(_aNeedle), MAG*168,  MAG *109 );//绘制实心无锯齿多边形
	GUI_Delay(5);//这一条语句很有必要存在
	GUI_SetColor(0x22AD38);
	GUI_AA_FillPolygon(pParam->aPoints1, countof(_aNeedle1), MAG*407, MAG * 102);//绘制实心无锯齿多边形
}

/*
*	函数名：Creat_device
*	输入参数：传感器的温度以及湿度数据
*	函数功能：指针的旋转 ，温湿度数据的显示
*/
void Creat_device(float wen,float shi) 
{
	int i,j,t=0;
	GUI_AUTODEV AutoDev;      /* 内存设备 */
	PARAM       Param;        /* PARAM类型变量 */
	GUI_MEMDEV_CreateAuto(&AutoDev);//创建自动设备对象
	for(t=0;t<=1;++t)
	{
		if(wen>0&&wen<=80)  i=(map(wen,0,80,350,520))-10; //0到240是整个的范围   0到82是 -40到0的数据映射范围  82到240是0到80的数据映射
		else       i=(map(wen,-40,0,265,350))-8; 
		if(shi>=100) shi=100;
		j=map(shi,0,100,230,555);   //范围 0-360
		Param.Angle= i;
		Param.Angle1=j+230;
		Param.Angle =((210-Param.Angle)* 3.1415926f) / 180;
		Param.Angle1 =((210-Param.Angle1)* 3.1415926f) / 180;
		GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);//按指定角度旋转多边形
		GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);//按指定角度旋转多边形
        GUI_SetFont(&GUI_Font32B_ASCII);
        GUI_SetColor(GUI_GREEN);		
        GUI_SetTextMode(GUI_TM_TRANS);
		if(wen<0)
		{
			GUI_GotoXY(65,20);					//移动光标
		    GUI_DispFloat(wen,4);			//在这里添加的显示数字
		}
		else
		{
			GUI_GotoXY(67,20);					//移动光标
		    GUI_DispFloat(wen,5);			//在这里添加的显示数字
		}
        GUI_SetTextMode(GUI_TM_TRANS);
		GUI_GotoXY(300,20);					//移动光标
		GUI_DispFloat(shi,5);			//在这里添加的显示数字
		GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &_Draw, &Param);//使用GUI_AUTODEV 对象进行绘制	
	}
}

void MainTask(void)
{
	wenshi_display();
	Creat_device(tem,hum);//指针的旋转 ，温湿度数据的显示
	printf("Maintask a=%f  b=%f  \r\n",a,b);
}

void Draw_Qr_code(void)
{
	GUI_HMEM hMem;
	GUI_DrawGradientH(324,164,450,290,GUI_GRAY,GUI_GRAY);
//	hMem = GUI_QR_Create(http, 5, GUI_QR_ECLEVEL_L, 2);
	//GUI_QR_Draw(hMem, 325, 165);
//	GUI_QR_Delete(hMem);//释放存储二维码的内存空间
	Show_StrAndChinese(25,160,tite_color,0,tite,32,1);	
	Show_StrAndChinese(60,200,text_color,0,hang1,24,1);
	Show_StrAndChinese(60,230,text_color,0,hang2,24,1);
	Show_StrAndChinese(60,260,text_color,0,hang3,24,1);
	Show_StrAndChinese(60,290,text_color,0,hang4,24,1);
	GUI_AA_EnableHiRes();//启用高分辨率坐标
	GUI_AA_SetFactor(MAG);//设置当前抗锯齿因子

}
#endif

#if Clock_Dial_num == 2  //界面2  需要写的
static void _Draw(void * p) 
{
	PARAM * pParam;
	pParam = (PARAM *)p;
	if(pParam->AutoDevInfo.DrawFixed) 
	{
		GUI_ClearRect(15,160,314,160);//前两个个参数是要显示图片初始位置的图片，后面两个参数是图片的尺寸+初始位置的坐标
		draw_bitmap(0,0);   //绘制位图
	}	
	GUI_SetColor(0x0f9309);
	GUI_AA_FillPolygon(pParam->aPoints, countof(_aNeedle), MAG*168,  MAG *109 );//绘制实心无锯齿多边形
//	GUI_Delay(5);//这一条语句很有必要存在
	GUI_SetColor(0x22AD38);
	GUI_AA_FillPolygon(pParam->aPoints1, countof(_aNeedle1), MAG*407, MAG * 102);//绘制实心无锯齿多边形
}

/*
*	函数名：Creat_device
*	输入参数：传感器的温度以及湿度数据
*	函数功能：指针的旋转 ，温湿度数据的显示
*/
void Creat_device(float wen,float shi) 
{
	int i,j,t=0;
	GUI_AUTODEV AutoDev;      /* 内存设备 */
	PARAM       Param;        /* PARAM类型变量 */
	GUI_MEMDEV_CreateAuto(&AutoDev);//创建自动设备对象
	for(t=0;t<=1;++t)
	{
		if(wen>0&&wen<=80)  i=(map(wen,0,80,350,520))-10; //0到240是整个的范围   0到82是 -40到0的数据映射范围  82到240是0到80的数据映射
		else       i=(map(wen,-40,0,265,350))-8; 
		if(shi>=100) shi=100;
		j=map(shi,0,100,230,555);   //范围 0-360
		Param.Angle= i;
		Param.Angle1=j+230;
		Param.Angle =((210-Param.Angle)* 3.1415926f) / 180;
		Param.Angle1 =((210-Param.Angle1)* 3.1415926f) / 180;
		GUI_RotatePolygon(Param.aPoints, _aNeedle, countof(_aNeedle), Param.Angle);//按指定角度旋转多边形
		GUI_RotatePolygon(Param.aPoints1, _aNeedle1, countof(_aNeedle1), Param.Angle1);//按指定角度旋转多边形
        GUI_SetFont(&GUI_Font32B_ASCII);
        GUI_SetColor(GUI_GREEN);		
        GUI_SetTextMode(GUI_TM_TRANS);
		if(wen<0)
		{
			GUI_GotoXY(65,20);					//移动光标
		    GUI_DispFloat(wen,4);			//在这里添加的显示数字
		}
		else
		{
			GUI_GotoXY(67,20);					//移动光标
		    GUI_DispFloat(wen,5);			//在这里添加的显示数字
		}
        GUI_SetTextMode(GUI_TM_TRANS);
		GUI_GotoXY(300,20);					//移动光标
		GUI_DispFloat(shi,5);			//在这里添加的显示数字
		GUI_MEMDEV_DrawAuto(&AutoDev, &Param.AutoDevInfo, &_Draw, &Param);//使用GUI_AUTODEV 对象进行绘制	
	}
}

void MainTask(void)
{
	wenshi_display();
	Creat_device(tem,hum);//指针的旋转 ，温湿度数据的显示
	printf("Maintask a=%f  b=%f  \r\n",tem,hum);
}

void Draw_Qr_code(void)
{
	GUI_HMEM hMem;
	GUI_DrawGradientH(324,164,450,290,GUI_GRAY,GUI_GRAY);
//	hMem = GUI_QR_Create(http, 5, GUI_QR_ECLEVEL_L, 2);
	//GUI_QR_Draw(hMem, 325, 165);
 // GUI_QR_Delete(hMem);//释放存储二维码的内存空间
//	GUI_AA_EnableHiRes();//启用高分辨率坐标
//	GUI_AA_SetFactor(MAG);//设置当前抗锯齿因子
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



