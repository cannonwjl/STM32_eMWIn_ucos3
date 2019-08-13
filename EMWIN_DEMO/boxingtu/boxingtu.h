#ifndef _boxingtu_H
#define _boxingtu_H

#include <stdlib.h>
#include <string.h>
#include "DIALOG.h"
#include "GRAPH.h"

#define MAX_VALUE 180

/*

波形图是基于对话框来设计的
这个里面有一些常用波形图的一些按钮之类的东西
添加曲线的时候需要改的参数
static GRAPH_DATA_Handle  _ahData[];改变数组的大小
static I16 _aValue[4];改变数组的大小
static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE}; 添加曲线对应的颜色
删除曲线需要改的参数.
static GRAPH_DATA_Handle  _ahData[];改变数组的大小
static I16 _aValue[4];改变数组的大小
static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE};删除曲线对应的颜色
修改曲线对应的数据
static void _AddValues(void) 修改这个函数 最好写成数组那种的
static void _cbCallback(WM_MESSAGE * pMsg)回调函数中创建自己所需几条曲线
    for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
	  {
      _aValue[i] = rand() % 100;//创建数据存到对应的数组
      _ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 500, 0, 0);// 创建几条曲线存到对应的数组里面
      GRAPH_AttachData(hItem, _ahData[i]);//几条曲线同时显示的意思（自己理解）
    }
主要是这里面的数据需要修改


*/

static GRAPH_DATA_Handle  _ahData[4]; // Array of handles for the GRAPH_DATA objects图形数据对象的句柄数组
static GRAPH_SCALE_Handle _hScaleV;   // Handle of vertical scale垂直刻度手柄
static GRAPH_SCALE_Handle _hScaleH;   // Handle of horizontal scale水平刻度手柄

static I16 _aValue[4];
static int _Stop = 0;

static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE}; // 四条曲线的颜色

//
// Dialog ressource对话框资源
//
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "Graph widget demo",  0                ,   0,   0, 320, 240, FRAMEWIN_CF_MOVEABLE },
  { GRAPH_CreateIndirect,     0,                   GUI_ID_GRAPH0    ,   5,   5, 270, 175 },
  { TEXT_CreateIndirect,      "Spacing X:",        0                ,  10, 185,  50,  20 },
  { TEXT_CreateIndirect,      "Spacing Y:",        0                ,  10, 205,  50,  20 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER0   ,  60, 185,  60,  16 },
  { SLIDER_CreateIndirect,    0,                   GUI_ID_SLIDER1   ,  60, 205,  60,  16 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK0    , 130, 185,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK1    , 130, 205,  50,   0 },
  { TEXT_CreateIndirect,      "Border",            0                , 280,   5,  35,  15 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK2    , 280,  20,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK3    , 280,  40,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK4    , 280,  60,  35,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK5    , 280,  80,  35,   0 },
  { TEXT_CreateIndirect,      "Effect",            0                , 280, 100,  35,  15 },
  { RADIO_CreateIndirect,     0,                   GUI_ID_RADIO0    , 275, 115,  35,   0, 0, 3 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK6    , 180, 185,  50,   0 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK7    , 180, 205,  50,   0 },
  { BUTTON_CreateIndirect,    "Full Screen",       GUI_ID_BUTTON0   , 235, 185,  65,  18 },
  { CHECKBOX_CreateIndirect,  0,                   GUI_ID_CHECK8    , 235, 205,  70,   0 },
};


void MainTask2(void) ;

#endif
