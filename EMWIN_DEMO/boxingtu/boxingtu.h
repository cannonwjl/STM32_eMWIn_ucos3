#ifndef _boxingtu_H
#define _boxingtu_H

#include <stdlib.h>
#include <string.h>
#include "DIALOG.h"
#include "GRAPH.h"

#define MAX_VALUE 180

/*

����ͼ�ǻ��ڶԻ�������Ƶ�
���������һЩ���ò���ͼ��һЩ��ť֮��Ķ���
������ߵ�ʱ����Ҫ�ĵĲ���
static GRAPH_DATA_Handle  _ahData[];�ı�����Ĵ�С
static I16 _aValue[4];�ı�����Ĵ�С
static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE}; ������߶�Ӧ����ɫ
ɾ��������Ҫ�ĵĲ���.
static GRAPH_DATA_Handle  _ahData[];�ı�����Ĵ�С
static I16 _aValue[4];�ı�����Ĵ�С
static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE};ɾ�����߶�Ӧ����ɫ
�޸����߶�Ӧ������
static void _AddValues(void) �޸�������� ���д���������ֵ�
static void _cbCallback(WM_MESSAGE * pMsg)�ص������д����Լ����輸������
    for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
	  {
      _aValue[i] = rand() % 100;//�������ݴ浽��Ӧ������
      _ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 500, 0, 0);// �����������ߴ浽��Ӧ����������
      GRAPH_AttachData(hItem, _ahData[i]);//��������ͬʱ��ʾ����˼���Լ���⣩
    }
��Ҫ���������������Ҫ�޸�


*/

static GRAPH_DATA_Handle  _ahData[4]; // Array of handles for the GRAPH_DATA objectsͼ�����ݶ���ľ������
static GRAPH_SCALE_Handle _hScaleV;   // Handle of vertical scale��ֱ�̶��ֱ�
static GRAPH_SCALE_Handle _hScaleH;   // Handle of horizontal scaleˮƽ�̶��ֱ�

static I16 _aValue[4];
static int _Stop = 0;

static GUI_COLOR _aColor[] = {GUI_RED, GUI_GREEN, GUI_YELLOW,GUI_DARKBLUE}; // �������ߵ���ɫ

//
// Dialog ressource�Ի�����Դ
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
