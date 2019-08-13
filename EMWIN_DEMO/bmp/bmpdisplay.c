#include "bmpdisplay.h"
#include "Clock_dial .h"

extern GUI_CONST_STORAGE GUI_BITMAP bmwendu;
extern GUI_CONST_STORAGE GUI_BITMAP bmB_map;

/*
函数名：draw_bitmap
输入参数：图片的起始坐标 x,y
输出参数：无
函数功能：显示一张图片
*/
#if Clock_Dial_num == 1  //界面1

void draw_bitmap(int x0, int y0)
{
	GUI_DrawBitmap(&bmwendu,x0,y0);  //绘制ALIENTEK图标	
}
#endif
#if Clock_Dial_num == 2  //界面2

void draw_bitmap(int x0, int y0)
{
	GUI_DrawBitmap(&bmB_map,x0,y0);  //绘制ALIENTEK图标	
}
#endif

