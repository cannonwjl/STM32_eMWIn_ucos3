#include "hanzidisplay.h"
void name_demo(u16 x,u16 y,const char *s)
{
	GUI_UC_SetEncodeUTF8();//ʹ��UTF-8����
	GUI_SetFont(&GUI_Fontmudan); //��������
	GUI_SetColor(GUI_BLACK);       //����ǰ��ɫ(���ı������ߵ���ɫ)
	GUI_DispStringHCenterAt(s ,x,y);
}

void time_demo(u16 x,u16 y,const char *s)
{
	GUI_UC_SetEncodeUTF8();//ʹ��UTF-8����
	GUI_SetFont(&GUI_Fontjieshao); //��������
	GUI_SetColor(GUI_BLACK);       //����ǰ��ɫ(���ı������ߵ���ɫ)
	GUI_DispStringHCenterAt(s ,x,y);//��λ ����
}
void chinese_demo(const char * s,GUI_RECT * Rect,int align,const GUI_FONT * Font,GUI_COLOR Color,char len)
{
	GUI_UC_SetEncodeUTF8();//ʹ��UTF-8����
	len=strlen(s);
	if(len>=8)  GUI_DispNextLine();
	if(Font==&GUI_Fontmudan)
	{
		GUI_SetColor(Color);       //������ɫ
		GUI_SetFont(Font); //��������
		GUI_DispStringInRect(s,Rect,align);//��ָ���ľ�����������ʾ�ַ���  ��λ������
	}
	if(Font==&GUI_Fontjieshao)
	{
		GUI_SetColor(Color);       //������ɫ
		GUI_SetFont(Font); //��������
		GUI_DispStringInRect(s,Rect,align);//��ָ���ľ�����������ʾ�ַ���
	}
	
}
//���ֿ�����ʾ��ֻ��  ���� ����  ��ɫ  λ��  ÿһ�п�����ʾ�ĺ�����

