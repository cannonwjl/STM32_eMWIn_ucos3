#include "numdisplay.h"
/*
��������str_textxytest
����������ַ���   ���� x,y
�����������
�������ܣ�����һ�������������ڸ�����������ʾ�ַ����������Զ����У�
*/

void str_textxytest(char *s,u16 x,u16 y)//����һ�������������ڸ�����������ʾ�ַ����������Զ����У�
{
	int i;
	GUI_RECT Rect	={200,240,259,299};	//���������ʾ����
	GUI_WRAPMODE aWm[] = {GUI_WRAPMODE_NONE,
						  GUI_WRAPMODE_CHAR,
						  GUI_WRAPMODE_WORD};				  
	for(i=0;i<3;i++)
	{
		GUI_SetColor(GUI_WHITE);
		GUI_FillRectEx(&Rect);
		GUI_SetColor(GUI_BLACK);
		GUI_DispStringInRectWrap(s,&Rect,GUI_TA_LEFT,aWm[i]);//�ڵ�ǰ����ָ���ľ�����������ʾ�ַ���(�����Զ�����)
		Rect.x0 += x;
		Rect.x1 += y;
	}	
}
/*
��������numint_demo
������������� x,y �������� ����
�����������
�������ܣ���ʾ������

*/

void numint_demo(u16 x,u16 y,u16 num,u16 len)
{
	GUI_DispDecAt(num,x,y,len);			//��ʾ��ֵ200
}
/*
��������numfloat_demo
������������� x,y ���������� ����
�����������
�������ܣ���ʾ������

*/

void numfloat_demo(u16 x,u16 y,float num,u16 len)
{
	GUI_GotoXY(x,y);					//�ƶ����
	GUI_DispFloat(num,len);					//��ʾ������49.35
}
/*
��������numbin_demo
������������� x,y ���� ����
�����������
�������ܣ���ʾ��������

*/

void numbin_demo(u16 x,u16 y,u32 num,u16 len)
{
	GUI_DispBinAt(num,x,y,len);				//��ʾʮ������8�Ķ�����ֵ	
}
/*
��������numhex_demo
������������� x,y ���� ����
�����������
�������ܣ���ʾʮ��������

*/

void numhex_demo(u16 x,u16 y,u32 num,u16 len)
{
	GUI_GotoXY(x,y);					//�ƶ����
	GUI_DispHex(num,len);						//��ʾʮ������100��16����ֵ
}
/*
��������str_demo
������������� x,y �ַ���
�����������
�������ܣ���ʾ�ַ���

*/

void str_demo(u16 x,u16 y,const char *s)
{
	GUI_DispStringHCenterAt(s,x,y);			//��ʾ��ֵ200
}
