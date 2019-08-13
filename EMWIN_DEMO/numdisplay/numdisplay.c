#include "numdisplay.h"
/*
函数名：str_textxytest
输入参数：字符串   坐标 x,y
输出参数：无
函数功能：创建一个矩形区域并且在改区域里面显示字符串（可以自动换行）
*/

void str_textxytest(char *s,u16 x,u16 y)//创建一个矩形区域并且在改区域里面显示字符串（可以自动换行）
{
	int i;
	GUI_RECT Rect	={200,240,259,299};	//定义矩形显示区域
	GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE,
						  GUI_WRAPMODE_CHAR,
						  GUI_WRAPMODE_WORD};				  
	for(i=0;i<3;i++)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_FillRectEx(&Rect);
		GUI_SetColor(GUI_BLACK);
		GUI_DispStringInRectWrap(s,&Rect,GUI_TA_LEFT,aWm[i]);//在当前窗口指定的矩形区域内显示字符串(并可自动换行)
		Rect.x0 += x;
		Rect.x1 += y;
	}	
}
/*
函数名：numint_demo
输入参数：坐标 x,y 整型数字 长度
输出参数：无
函数功能：显示整型数

*/

void numint_demo(u16 x,u16 y,u16 num,u16 len)
{
	GUI_DispDecAt(num,x,y,len);			//显示数值200
}
/*
函数名：numfloat_demo
输入参数：坐标 x,y 浮点型数字 长度
输出参数：无
函数功能：显示浮点数

*/

void numfloat_demo(u16 x,u16 y,float num,u16 len)
{
	GUI_GotoXY(x,y);					//移动光标
	GUI_DispFloat(num,len);					//显示浮点数49.35
}
/*
函数名：numbin_demo
输入参数：坐标 x,y 数字 长度
输出参数：无
函数功能：显示二进制数

*/

void numbin_demo(u16 x,u16 y,u32 num,u16 len)
{
	GUI_DispBinAt(num,x,y,len);				//显示十进制数8的二进制值	
}
/*
函数名：numhex_demo
输入参数：坐标 x,y 数字 长度
输出参数：无
函数功能：显示十六进制数

*/

void numhex_demo(u16 x,u16 y,u32 num,u16 len)
{
	GUI_GotoXY(x,y);					//移动光标
	GUI_DispHex(num,len);						//显示十进制数100的16进制值
}
/*
函数名：str_demo
输入参数：坐标 x,y 字符串
输出参数：无
函数功能：显示字符串

*/

void str_demo(u16 x,u16 y,const char *s)
{
	GUI_DispStringHCenterAt(s,x,y);			//显示数值200
}
