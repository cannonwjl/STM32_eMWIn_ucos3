#ifndef __hanzidisplay_H
#define __hanzidisplay_H	


#include "malloc.h"
#include "GUI.h"
#include "DIALOG.h"
#include "GRAPH.h"
#include "sys.h"
#include "string.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_Fontmudan;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontjieshao;
//extern GUI_CONST_STORAGE GUI_FONT GUI_Fontchinese;

void name_demo(u16 x,u16 y,const char *s);
void time_demo(u16 x,u16 y,const char *s);
void chinese_demo(const char * s,GUI_RECT * Rect,int align,const GUI_FONT * Font,GUI_COLOR Color,char len);


#endif
