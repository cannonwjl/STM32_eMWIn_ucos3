#include "hanzidisplay.h"
void name_demo(u16 x,u16 y,const char *s)
{
	GUI_UC_SetEncodeUTF8();//使能UTF-8编码
	GUI_SetFont(&GUI_Fontmudan); //设置字体
	GUI_SetColor(GUI_BLACK);       //设置前景色(如文本，画线等颜色)
	GUI_DispStringHCenterAt(s ,x,y);
}

void time_demo(u16 x,u16 y,const char *s)
{
	GUI_UC_SetEncodeUTF8();//使能UTF-8编码
	GUI_SetFont(&GUI_Fontjieshao); //设置字体
	GUI_SetColor(GUI_BLACK);       //设置前景色(如文本，画线等颜色)
	GUI_DispStringHCenterAt(s ,x,y);//单位 像素
}
void chinese_demo(const char * s,GUI_RECT * Rect,int align,const GUI_FONT * Font,GUI_COLOR Color,char len)
{
	GUI_UC_SetEncodeUTF8();//使能UTF-8编码
	len=strlen(s);
	if(len>=8)  GUI_DispNextLine();
	if(Font==&GUI_Fontmudan)
	{
		GUI_SetColor(Color);       //设置颜色
		GUI_SetFont(Font); //设置字体
		GUI_DispStringInRect(s,Rect,align);//在指定的矩形区域中显示字符串  单位是像素
	}
	if(Font==&GUI_Fontjieshao)
	{
		GUI_SetColor(Color);       //设置颜色
		GUI_SetFont(Font); //设置字体
		GUI_DispStringInRect(s,Rect,align);//在指定的矩形区域中显示字符串
	}
	
}
//汉字可以显示的只有  内容 字体  颜色  位置  每一行可以显示的汉字数

