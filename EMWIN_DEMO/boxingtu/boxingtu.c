#include "boxingtu.h" 
/*********************************************************************
		graph�����ڻ����κζ���֮ǰ��֮������������
*/
static void _UserDraw(WM_HWIN hWin, int Stage)
{
  if (Stage == GRAPH_DRAW_LAST) {
    char acText[] = "Temperature";
    GUI_RECT Rect;
    GUI_RECT RectInvalid;
    int FontSizeY;

    GUI_SetFont(&GUI_Font13_ASCII);//����������ʽ
    FontSizeY = GUI_GetFontSizeY();//���ص�ǰ��ѡ����ĸ߶����أ�Y ��С����
    WM_GetInsideRect(&Rect);//���ؿͻ��������꣬�������ɻС���߳ߴ��ȥ�߽�ߴ�ȷ��
    WM_GetInvalidRect(hWin, &RectInvalid);//���ظ������ڵ���Ч����
    Rect.x1 = Rect.x0 + FontSizeY;
    GUI_SetColor(GUI_YELLOW);//����������ɫ
    GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);//��ָ���ľ�����������ʾ�ַ���
  }
}

/*********************************************************************
����������/��ʾ����ťͼ�͹�����wid֮������д���
*/

static void _ForEach(WM_HWIN hWin, void * pData) 
{
  int Id;
  int FullScreenMode;
  FullScreenMode = *(int *)pData;
  Id = WM_GetId(hWin);
  if ((Id == GUI_ID_GRAPH0) || (Id == GUI_ID_BUTTON0) || (Id == GUI_ID_VSCROLL) || (Id == GUI_ID_HSCROLL)) 
	{
    return;
  }
  if (FullScreenMode) 
	{
    WM_HideWindow(hWin);//���ش���
  } 
	else 
	{
    WM_ShowWindow(hWin);//��ʾ����
  }
}

/*********************************************************************
������ͨ�����ػ���ʾ
�Ի���Ĵ���С�������Ŵ�/��Сͼ�δ���С�������޸�����һЩ����С����
�Ի���С���������ԡ�
*/
static void _ToggleFullScreenMode(WM_HWIN hDlg) 
{
  static int FullScreenMode;
  static GUI_RECT Rect;
  static unsigned ScalePos;
  WM_HWIN hGraph;
  WM_HWIN hButton;
  WM_HWIN hClient;
  GUI_RECT RectInside;
  int xPos, yPos;

  hGraph  = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//����GUI_ID_GRAPH0�Ĵ��ھ����
  hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);//����GUI_ID_BUTTON0�Ĵ��ھ����
  FullScreenMode ^= 1;
  if (FullScreenMode) 
	{       
    // Enter the full screen mode
    //����ȫ��ģʽ
    hClient = WM_GetClientWindow(hDlg);//���ؿͻ��˴��ڵľ����
    BUTTON_SetText(hButton, "Back");//���ð�ť������ı�
    WM_MoveWindow(hButton, 0, 11);//��ָ�������ƶ�ĳ�ξ��롣
    FRAMEWIN_SetTitleVis(hDlg, 0);//���ñ������Ŀɼ��Ա��
    WM_GetInsideRectEx(hClient, &RectInside);//���ش��ڼ�ȥ�߽��ĳߴ硣
    WM_GetWindowRectEx(hGraph, &Rect);//���ش��������������е����ꡣ
    WM_ForEachDesc(hClient, _ForEach, &FullScreenMode); // �������к��
    xPos = WM_GetWindowOrgX(hClient);
    yPos = WM_GetWindowOrgY(hClient);
    WM_SetWindowPos(hGraph, xPos, yPos, RectInside.x1, RectInside.y1);//���ô��ڵĳߴ��λ��
    ScalePos = GRAPH_SCALE_SetPos(_hScaleH, RectInside.y1 - 20);//���ÿ̶ȵ�ˮƽ��ֱλ�á�
  } 
	else 
	{
    // Return to normal mode
    //�˳�ȫ��ģʽ
    BUTTON_SetText(hButton, "Full Screen");//���ð�ť������ı�
    WM_MoveWindow(hButton, 0, -11);////��ָ�������ƶ�ĳ�ξ���
    WM_ForEachDesc(WM_GetClientWindow(hDlg), _ForEach, &FullScreenMode); // Show all descendants��ʾ���к��
    WM_SetWindowPos(hGraph, Rect.x0, Rect.y0, Rect.x1 - Rect.x0 + 1, Rect.y1 - Rect.y0 + 1);//���ô��ڵĳߴ��λ��
    FRAMEWIN_SetTitleVis(hDlg, 1);//���ñ������Ŀɼ��Ա��
    GRAPH_SCALE_SetPos(_hScaleH, ScalePos);//���ÿ̶ȵ�ˮƽ��ֱλ�á�
  }
}

/*********************************************************************
�Ի���Ļص�����
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
  unsigned i;
  int      NCode;
  int      Id;
  int      Value;
  WM_HWIN  hDlg;
  WM_HWIN  hItem;
  hDlg = pMsg->hWin;
  switch (pMsg->MsgId) 
	{
		case WM_INIT_DIALOG:
					hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ��
					//
					// Add graphs���ͼ��
					//
					for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
					{
						_ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 500, 0, 0);// �����������ߴ浽��Ӧ����������
						GRAPH_AttachData(hItem, _ahData[i]);//��������ͬʱ��ʾ����˼���Լ���⣩
					}
					//
					// Set graph attributes����ͼ������
					//
					GRAPH_SetGridDistY(hItem, 10);//���ô�ֱ�����ࡣ
					GRAPH_SetGridVis(hItem, 1);//����������ơ�
					GRAPH_SetGridFixedX(hItem, 1);//�̶�X �᷽�������
					GRAPH_SetUserDraw(hItem, _UserDraw);//�����û��ص��������˺�����С�����ڻ��ƹ����е��ã�ʹӦ�ó����ܹ������û���������ݡ�
					//
					// Create and add vertical scale��������Ӵ�ֱ����
					//
					_hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);//����ˮƽ�̶ȶ���
					GRAPH_SCALE_SetTextColor(_hScaleV, GUI_YELLOW);//���ÿ̶ȶ������ɫ
					GRAPH_AttachScale(hItem, _hScaleV);//���̶ȶ��󸽼ӵ�GRAPH С���ߡ�
					//
					// Create and add horizontal scale���������ˮƽ����
					//
					_hScaleH = GRAPH_SCALE_Create(155, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);//������ֱ�̶ȶ���
					GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);//���ÿ̶ȶ������ɫ
					GRAPH_AttachScale(hItem, _hScaleH);//���̶ȶ��󸽼ӵ�GRAPH С���ߡ�
					//
					// Init check boxes��ʼ��ѡ��
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK2);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "L");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK3);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "T");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK4);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "R");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK5);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "B");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "Stop");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "Grid");
					CHECKBOX_SetState(hItem, 1);
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK6);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "HScroll");
					CHECKBOX_SetState(hItem, 1);
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK7);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "VScroll");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK8);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
					CHECKBOX_SetText(hItem, "MirrorX");
					//
					// Init slider widgets��ʼ������С����
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER0);
					SLIDER_SetRange(hItem, 0, 10);
					SLIDER_SetValue(hItem, 5);
					SLIDER_SetNumTicks(hItem, 6);
					hItem = WM_GetDialogItem(hDlg, GUI_ID_SLIDER1);
					SLIDER_SetRange(hItem, 0, 20);
					SLIDER_SetValue(hItem, 5);
					SLIDER_SetNumTicks(hItem, 6);
					//
					// Init radio widget��ʼ�����ߵ�С����
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
					RADIO_SetText(hItem, "3D", 0);
					RADIO_SetText(hItem, "flat", 1);
					RADIO_SetText(hItem, "-", 2);
					//
					// Init button widget��ʼ����ťС����
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
					WM_SetStayOnTop(hItem, 1);
			break;
		case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);      // Id of widgetС����ID
				NCode = pMsg->Data.v;                 // Notification code֪ͨ����
			switch (NCode) 
			{
				case WM_NOTIFICATION_CLICKED:
					switch (Id) 
					{
						case GUI_ID_BUTTON0:
						     _ToggleFullScreenMode(hDlg);
						break;
					}
					break;
				case WM_NOTIFICATION_VALUE_CHANGED:
					switch (Id) 
					{
						case GUI_ID_CHECK0:
							//
							// Toggle stop mode�л�ֹͣģʽ
							//
							_Stop ^= 1;
							break;
						case GUI_ID_CHECK1:
							//
							// Toggle grid�л�����
							//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								GRAPH_SetGridVis(hItem, CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK1)));
								break;
						case GUI_ID_CHECK2:
						case GUI_ID_CHECK3:
						case GUI_ID_CHECK4:
						case GUI_ID_CHECK5:
							//
							// Toggle border�л��߽�
							//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									GRAPH_SetBorder(hItem, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK2)) * 10, //�л�ͼ���ĸ��߿�Ĵ�С
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK3)) * 5, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK4)) * 5, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK5)) * 5);
									break;
						case GUI_ID_SLIDER0:
								//
								// Set horizontal grid spacing����ˮƽ������
								//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								Value = SLIDER_GetValue(pMsg->hWinSrc) * 10;//���ػ������ĵ�ǰֵ��
								GRAPH_SetGridDistX(hItem, Value);//����ˮƽ�����ࡣ
								GRAPH_SCALE_SetTickDist(_hScaleH, Value);//���ÿ̶���֮������ؾ��롣
							break;
						case GUI_ID_SLIDER1:
								//
								// Set vertical grid spacing���ô�ֱ������
								//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								Value = SLIDER_GetValue(pMsg->hWinSrc) * 5;//���ػ������ĵ�ǰֵ��
								GRAPH_SetGridDistY(hItem, Value);//���ô�ֱ�����ࡣ
								GRAPH_SCALE_SetTickDist(_hScaleV, Value);//���ÿ̶���֮������ؾ���
							break;
						case GUI_ID_RADIO0:
							//
							// Set the widget effect����С����Ч��
							//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								switch (RADIO_GetValue(pMsg->hWinSrc))
								{
									case 0:
										WIDGET_SetEffect(hItem, &WIDGET_Effect_3D);
										break;
									case 1:
										WIDGET_SetEffect(hItem, &WIDGET_Effect_Simple);
										break;
									case 2:
										WIDGET_SetEffect(hItem, &WIDGET_Effect_None);
										break;
								}
								break;
						case GUI_ID_CHECK6:
									//
									// Toggle horizontal scroll bar�л�ˮƽ������
									//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK6))) 
									{
										GRAPH_SetVSizeX(hItem, 300);//����GRAPH С���ߵ�ˮƽ��Χ��
									} 
									else 
									{
										GRAPH_SetVSizeX(hItem, 0);
									}
									break;
						case GUI_ID_CHECK7:
									//
									// Toggle vertical scroll bar�л���ֱ������
									//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK7))) 
									{
										GRAPH_SetVSizeY(hItem, 300);//����GRAPH С���ߵĴ�ֱ��Χ��
									} 
									else 
									{
										GRAPH_SetVSizeY(hItem, 0);
									}
									break;
						case GUI_ID_CHECK8:
									//
									// Toggle alignment�л����뷽ʽ
									//
									WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//���ضԻ�����Ŀ��С���ߣ��Ĵ��ھ����
									for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
									{
										if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK8))) //���ظ�ѡ��ĵ�ǰ״̬��ѡ�л�δѡ�У���
										{
											GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_LEFT);//��������롣
											GRAPH_DATA_YT_MirrorX (_ahData[i], 1);
										}
										else 
										{
											GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_RIGHT);//�����Ҷ��루Ĭ�ϣ�
											GRAPH_DATA_YT_MirrorX (_ahData[i], 0);
										}
									}
									break;
					}
					break;
			}
			break;
		default:
			WM_DefaultProc(pMsg);
  }
}

void MainTask2() 
{
	WM_HWIN hDlg;
	int a,b,c;

  WM_SetDesktopColor(GUI_WHITE);//�������洰����ɫ��
  #if GUI_SUPPORT_MEMDEV//ʹ�������Ժܺõ�ȥ����Ļ��˸������
    WM_SetCreateFlags(WM_CF_MEMDEV);//�����ڴ����´���ʱ����Ĭ�ϵı�ǡ�
  #endif
  hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);//������ִ�жԻ���
  while (1) 
	{
		a=rand()%180;
		b=rand()%120;
		c=rand()%80;
		GRAPH_DATA_YT_AddValue(_ahData[0], a);//��GRAPH_DATA_YT ��������������
		GRAPH_DATA_YT_AddValue(_ahData[1], b);//��GRAPH_DATA_YT ��������������
		GRAPH_DATA_YT_AddValue(_ahData[2], c);//��GRAPH_DATA_YT ��������������
    GUI_Exec();//ִ�лص����������������������Ҫ
  }
}

