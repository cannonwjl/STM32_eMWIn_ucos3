#include "2ddisplay.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103战舰开发板
//STemWin 2D绘图 
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/4/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

const GUI_POINT aPoints[]={
	{40,20},
	{0,20},
	{20,0}
};

const GUI_POINT aPoints1[]={
	{0,20},
	{40,20},
	{20,0}
};

const GUI_POINT aPointArrow[]={
	{ 0, -5},
	{-40, -35},
	{-10, -25},
	{-10, -85},
	{ 10, -85},
	{ 10, -25},
	{ 40, -35},
};

GUI_POINT aEnlargedPoints[GUI_COUNTOF(aPoints)]; 	//多边形放大后坐标
GUI_POINT aMagnifiedPoints[GUI_COUNTOF(aPoints1)];

//2D图形绘制例程
void display_2d(void)
{
////	draw_graph();
}

//Alpha混合显示
void alpha_display(void)
{
	GUI_EnableAlpha(1);				//使能Alpha功能
	GUI_SetBkColor(GUI_WHITE);		//设置背景颜色
	GUI_Clear();
	
	GUI_SetTextMode(GUI_TM_TRANS);	//文本透明显示
	GUI_SetColor(GUI_BLACK);
	GUI_SetFont(&GUI_Font32_1);
	GUI_DispStringHCenterAt("ALIENTEK ALPHA TEST",400,220);
	
	GUI_SetAlpha(0x40);						//设置alpha值0x00-0XFF
	GUI_SetColor(GUI_RED);	
	GUI_FillRect(100,50,300,250);			//绘制红色矩形
	
	GUI_SetAlpha(0x80);	
	GUI_SetColor(GUI_GREEN);
	GUI_FillRect(200,150,400,350);			//绘制绿色矩形
	
	GUI_SetAlpha(0xC0);
	GUI_SetColor(GUI_BLUE);
	GUI_FillRect(300,250,500,450);			//绘制蓝色矩形
	
	GUI_SetAlpha(0x80);
	GUI_SetColor(GUI_YELLOW);				
	GUI_FillRect(400,150,600,350);			//绘制黄色矩形
	
	GUI_SetAlpha(0x40);			
	GUI_SetColor(GUI_BROWN);				
	GUI_FillRect(500,50,700,250);			//绘制棕色矩形
	GUI_SetAlpha(0);						//恢复alpha通道
}

//绘制弧线 绘制简易仪表盘
void drawarcscale(void) 
{
	int x0 = 630;
	int y0 = 380;
	int i;
	char ac[4];
	GUI_SetPenSize( 5 );
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_FontComic18B_ASCII);
	GUI_SetColor(GUI_BLACK);
	GUI_DrawArc( x0,y0,150, 150,-30, 210 );
	for (i=0; i<= 23; i++) {
		float a = (-30+i*10)*3.1415926/180;
		int x = -141*cos(a)+x0;
		int y = -141*sin(a)+y0;
		if (i%2 == 0)
		GUI_SetPenSize( 5 );
		else
		GUI_SetPenSize( 4 );
		GUI_DrawPoint(x,y);
		if (i%2 == 0) {
			x = -123*cos(a)+x0;
			y = -130*sin(a)+y0;
			sprintf(ac, "%d", 10*i);
			GUI_SetTextAlign(GUI_TA_VCENTER);
			GUI_DispStringHCenterAt(ac,x,y);
		}
	}
}

//绘制线图
void draw_graph(void)
{
	I16 aY[400];
	int i;
	for(i=0;i<GUI_COUNTOF(aY);i++)	aY[i] = rand()%100;
	GUI_ClearRect(0,300,400,400);   //每次显示前先清除显示区域
	GUI_DrawGraph(aY,GUI_COUNTOF(aY),0,300);
}

//绘制多边形
void draw_polygon(void)
{
	int i, Mag, magy = 50, Count = 4;
	GUI_SetBkColor(GUI_BLUE);
	GUI_SetColor(GUI_YELLOW);
	GUI_Clear();
	
	GUI_SetTextMode(GUI_TM_TRANS);	//透明显示
	GUI_SetFont(&GUI_Font24_ASCII);	//设置字体
	GUI_DispStringHCenterAt("ALIENTEK PLAYGON DISPLAY",400,10);

	GUI_SetColor(GUI_WHITE);
	GUI_SetDrawMode(GUI_DM_XOR);	//设置绘制模式
	GUI_FillPolygon(aPoints,GUI_COUNTOF(aPoints),140,110);
	for(i=1;i<10;i++)
	{
		GUI_EnlargePolygon(aEnlargedPoints,aPoints,GUI_COUNTOF(aPoints),i * 5); 	//放大坐标
		GUI_FillPolygon(aEnlargedPoints,GUI_COUNTOF(aPoints),140,110);				//绘制放大坐标后的多边形
	}
	GUI_SetDrawMode(GUI_DM_NORMAL);	//恢复默认的绘制模式
	
	GUI_SetColor(GUI_GREEN);
	for (Mag = 1; Mag <= 4; Mag *= 2, Count /= 2) 
	{
		int magx = 600;
		GUI_MagnifyPolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Mag); 	//按指定系数放大多边形
		for (i = Count; i > 0; i--, magx += 40 * Mag) 
		{
			GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), magx, magy);			//绘制多边形
		}
		magy += 20 * Mag;
	}
	
	GUI_SetFont(&GUI_Font8x16);
	GUI_DispStringAt("Polygons of arbitrary shape ", 100, 300);
	GUI_DispStringAt("in any color", 220, 320);
	GUI_SetColor(GUI_DARKRED);
	GUI_FillPolygon (&aPointArrow[0],7,200,400);		//绘制多边形
}


