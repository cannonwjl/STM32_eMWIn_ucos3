#include "boxingtu.h" 
/*********************************************************************
		graph对象在绘制任何东西之前和之后调用这个例程
*/
static void _UserDraw(WM_HWIN hWin, int Stage)
{
  if (Stage == GRAPH_DRAW_LAST) {
    char acText[] = "Temperature";
    GUI_RECT Rect;
    GUI_RECT RectInvalid;
    int FontSizeY;

    GUI_SetFont(&GUI_Font13_ASCII);//设置字体样式
    FontSizeY = GUI_GetFontSizeY();//返回当前所选字体的高度像素（Y 大小）。
    WM_GetInsideRect(&Rect);//返回客户区的坐标，该区域由活动小工具尺寸减去边界尺寸确定
    WM_GetInvalidRect(hWin, &RectInvalid);//返回给定窗口的无效矩形
    Rect.x1 = Rect.x0 + FontSizeY;
    GUI_SetColor(GUI_YELLOW);//设置字体颜色
    GUI_DispStringInRectEx(acText, &Rect, GUI_TA_HCENTER, strlen(acText), GUI_ROTATE_CCW);//在指定的矩形区域中显示字符串
  }
}

/*********************************************************************
此例程隐藏/显示除按钮图和滚动条wid之外的所有窗口
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
    WM_HideWindow(hWin);//隐藏窗口
  } 
	else 
	{
    WM_ShowWindow(hWin);//显示窗口
  }
}

/*********************************************************************
此例程通过隐藏或显示
对话框的窗口小部件，放大/缩小图形窗口小部件和修改其他一些窗口小部件
对话框小部件的属性。
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

  hGraph  = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//返回GUI_ID_GRAPH0的窗口句柄。
  hButton = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);//返回GUI_ID_BUTTON0的窗口句柄。
  FullScreenMode ^= 1;
  if (FullScreenMode) 
	{       
    // Enter the full screen mode
    //进入全屏模式
    hClient = WM_GetClientWindow(hDlg);//返回客户端窗口的句柄。
    BUTTON_SetText(hButton, "Back");//设置按钮上面的文本
    WM_MoveWindow(hButton, 0, 11);//将指定窗口移动某段距离。
    FRAMEWIN_SetTitleVis(hDlg, 0);//设置标题栏的可见性标记
    WM_GetInsideRectEx(hClient, &RectInside);//返回窗口减去边界后的尺寸。
    WM_GetWindowRectEx(hGraph, &Rect);//返回窗口在桌面坐标中的坐标。
    WM_ForEachDesc(hClient, _ForEach, &FullScreenMode); // 隐藏所有后代
    xPos = WM_GetWindowOrgX(hClient);
    yPos = WM_GetWindowOrgY(hClient);
    WM_SetWindowPos(hGraph, xPos, yPos, RectInside.x1, RectInside.y1);//设置窗口的尺寸和位置
    ScalePos = GRAPH_SCALE_SetPos(_hScaleH, RectInside.y1 - 20);//设置刻度的水平或垂直位置。
  } 
	else 
	{
    // Return to normal mode
    //退出全屏模式
    BUTTON_SetText(hButton, "Full Screen");//设置按钮上面的文本
    WM_MoveWindow(hButton, 0, -11);////将指定窗口移动某段距离
    WM_ForEachDesc(WM_GetClientWindow(hDlg), _ForEach, &FullScreenMode); // Show all descendants显示所有后代
    WM_SetWindowPos(hGraph, Rect.x0, Rect.y0, Rect.x1 - Rect.x0 + 1, Rect.y1 - Rect.y0 + 1);//设置窗口的尺寸和位置
    FRAMEWIN_SetTitleVis(hDlg, 1);//设置标题栏的可见性标记
    GRAPH_SCALE_SetPos(_hScaleH, ScalePos);//设置刻度的水平或垂直位置。
  }
}

/*********************************************************************
对话框的回调函数
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
					hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//返回对话框项目（小工具）的窗口句柄
					//
					// Add graphs添加图表
					//
					for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
					{
						_ahData[i] = GRAPH_DATA_YT_Create(_aColor[i], 500, 0, 0);// 创建几条曲线存到对应的数组里面
						GRAPH_AttachData(hItem, _ahData[i]);//几条曲线同时显示的意思（自己理解）
					}
					//
					// Set graph attributes设置图形属性
					//
					GRAPH_SetGridDistY(hItem, 10);//设置垂直网格间距。
					GRAPH_SetGridVis(hItem, 1);//启用网格绘制。
					GRAPH_SetGridFixedX(hItem, 1);//固定X 轴方向的网格。
					GRAPH_SetUserDraw(hItem, _UserDraw);//设置用户回调函数。此函数由小工具在绘制过程中调用，使应用程序能够绘制用户定义的数据。
					//
					// Create and add vertical scale创建和添加垂直比例
					//
					_hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);//创建水平刻度对象。
					GRAPH_SCALE_SetTextColor(_hScaleV, GUI_YELLOW);//设置刻度对象的颜色
					GRAPH_AttachScale(hItem, _hScaleV);//将刻度对象附加到GRAPH 小工具。
					//
					// Create and add horizontal scale创建和添加水平比例
					//
					_hScaleH = GRAPH_SCALE_Create(155, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);//创建垂直刻度对象。
					GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);//设置刻度对象的颜色
					GRAPH_AttachScale(hItem, _hScaleH);//将刻度对象附加到GRAPH 小工具。
					//
					// Init check boxes初始复选框
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK2);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "L");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK3);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "T");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK4);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "R");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK5);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "B");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK0);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "Stop");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK1);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "Grid");
					CHECKBOX_SetState(hItem, 1);
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK6);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "HScroll");
					CHECKBOX_SetState(hItem, 1);
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK7);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "VScroll");
					hItem = WM_GetDialogItem(hDlg, GUI_ID_CHECK8);//返回对话框项目（小工具）的窗口句柄。
					CHECKBOX_SetText(hItem, "MirrorX");
					//
					// Init slider widgets初始化滑块小部件
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
					// Init radio widget初始化无线电小部件
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_RADIO0);
					RADIO_SetText(hItem, "3D", 0);
					RADIO_SetText(hItem, "flat", 1);
					RADIO_SetText(hItem, "-", 2);
					//
					// Init button widget初始化按钮小部件
					//
					hItem = WM_GetDialogItem(hDlg, GUI_ID_BUTTON0);
					WM_SetStayOnTop(hItem, 1);
			break;
		case WM_NOTIFY_PARENT:
				Id    = WM_GetId(pMsg->hWinSrc);      // Id of widget小部件ID
				NCode = pMsg->Data.v;                 // Notification code通知代码
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
							// Toggle stop mode切换停止模式
							//
							_Stop ^= 1;
							break;
						case GUI_ID_CHECK1:
							//
							// Toggle grid切换网格
							//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								GRAPH_SetGridVis(hItem, CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK1)));
								break;
						case GUI_ID_CHECK2:
						case GUI_ID_CHECK3:
						case GUI_ID_CHECK4:
						case GUI_ID_CHECK5:
							//
							// Toggle border切换边界
							//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									GRAPH_SetBorder(hItem, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK2)) * 10, //切换图表四个边框的大小
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK3)) * 5, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK4)) * 5, 
																	CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK5)) * 5);
									break;
						case GUI_ID_SLIDER0:
								//
								// Set horizontal grid spacing设置水平网格间距
								//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								Value = SLIDER_GetValue(pMsg->hWinSrc) * 10;//返回滑动条的当前值。
								GRAPH_SetGridDistX(hItem, Value);//设置水平网格间距。
								GRAPH_SCALE_SetTickDist(_hScaleH, Value);//设置刻度线之间的像素距离。
							break;
						case GUI_ID_SLIDER1:
								//
								// Set vertical grid spacing设置垂直网格间距
								//
								hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
								Value = SLIDER_GetValue(pMsg->hWinSrc) * 5;//返回滑动条的当前值。
								GRAPH_SetGridDistY(hItem, Value);//设置垂直网格间距。
								GRAPH_SCALE_SetTickDist(_hScaleV, Value);//设置刻度线之间的像素距离
							break;
						case GUI_ID_RADIO0:
							//
							// Set the widget effect设置小部件效果
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
									// Toggle horizontal scroll bar切换水平滚动条
									//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK6))) 
									{
										GRAPH_SetVSizeX(hItem, 300);//设置GRAPH 小工具的水平范围。
									} 
									else 
									{
										GRAPH_SetVSizeX(hItem, 0);
									}
									break;
						case GUI_ID_CHECK7:
									//
									// Toggle vertical scroll bar切换垂直滚动条
									//
									hItem = WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);
									if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK7))) 
									{
										GRAPH_SetVSizeY(hItem, 300);//设置GRAPH 小工具的垂直范围。
									} 
									else 
									{
										GRAPH_SetVSizeY(hItem, 0);
									}
									break;
						case GUI_ID_CHECK8:
									//
									// Toggle alignment切换对齐方式
									//
									WM_GetDialogItem(hDlg, GUI_ID_GRAPH0);//返回对话框项目（小工具）的窗口句柄。
									for (i = 0; i < GUI_COUNTOF(_aColor); i++) 
									{
										if (CHECKBOX_IsChecked(WM_GetDialogItem(hDlg, GUI_ID_CHECK8))) //返回复选框的当前状态（选中或未选中）。
										{
											GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_LEFT);//数据左对齐。
											GRAPH_DATA_YT_MirrorX (_ahData[i], 1);
										}
										else 
										{
											GRAPH_DATA_YT_SetAlign(_ahData[i], GRAPH_ALIGN_RIGHT);//数据右对齐（默认）
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

  WM_SetDesktopColor(GUI_WHITE);//设置桌面窗口颜色。
  #if GUI_SUPPORT_MEMDEV//使用这句可以很好地去掉屏幕闪烁的问题
    WM_SetCreateFlags(WM_CF_MEMDEV);//设置在创建新窗口时用作默认的标记。
  #endif
  hDlg = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, 0, 0, 0);//创建并执行对话框
  while (1) 
	{
		a=rand()%180;
		b=rand()%120;
		c=rand()%80;
		GRAPH_DATA_YT_AddValue(_ahData[0], a);//向GRAPH_DATA_YT 对象添加新数据项。
		GRAPH_DATA_YT_AddValue(_ahData[1], b);//向GRAPH_DATA_YT 对象添加新数据项。
		GRAPH_DATA_YT_AddValue(_ahData[2], c);//向GRAPH_DATA_YT 对象添加新数据项。
    GUI_Exec();//执行回调函数（所有任务）这个很重要
  }
}

