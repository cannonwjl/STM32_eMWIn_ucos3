#ifndef _TEXTDISPLAY_H
#define _TEXTDISPLAY_H

#include "sys.h"

#include "GUI.h"
#include <stddef.h>
#include <stdio.h>
#include "WM.h"
#include "FRAMEWIN.h"
#define MAX_WINDOWS   50
static WM_HWIN _ahWin[MAX_WINDOWS];

void str_textxytest(char *s,u16 x,u16 y);
void numint_demo(u16 x,u16 y,u16 num,u16 len);
void numfloat_demo(u16 x,u16 y,float num,u16 len);
void numbin_demo(u16 x,u16 y,u32 num,u16 len);
void numhex_demo(u16 x,u16 y,u32 num,u16 len);
void str_demo(u16 x,u16 y,const char *s);


#endif
