#include "bmpdisplay.h"
#include "Clock_dial .h"

extern GUI_CONST_STORAGE GUI_BITMAP bmwendu;
extern GUI_CONST_STORAGE GUI_BITMAP bmB_map;

/*
��������draw_bitmap
���������ͼƬ����ʼ���� x,y
�����������
�������ܣ���ʾһ��ͼƬ
*/
#if Clock_Dial_num == 1  //����1

void draw_bitmap(int x0, int y0)
{
	GUI_DrawBitmap(&bmwendu,x0,y0);  //����ALIENTEKͼ��	
}
#endif
#if Clock_Dial_num == 2  //����2

void draw_bitmap(int x0, int y0)
{
	GUI_DrawBitmap(&bmB_map,x0,y0);  //����ALIENTEKͼ��	
}
#endif

