#include "MainTask.h"


/*
**********************************************************************************************************
							   变量
**********************************************************************************************************
*/
GUI_MEMDEV_Handle _hBkMemDev;


/*
*********************************************************************************************************
*	函 数 名: _CreateBackGround
*	功能说明: 将梯度色绘制到存储设备里面，用于刷新显示屏背景
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _CreateBackGround() 
{
	GUI_MEMDEV_Handle hMemPrev;
	GUI_RECT          r;
	int               xSize;
	int               ySize;

	xSize = LCD_GetXSize();
	ySize = LCD_GetYSize();
	r.x0  = 0;
	r.x1  = xSize - 1;
	r.y0  = 0;
	r.y1  = ySize - 1;
	
	_hBkMemDev = GUI_MEMDEV_CreateEx(r.x0, r.y0, r.x1 + 1, r.y1 + 1, GUI_MEMDEV_NOTRANS);
	hMemPrev = GUI_MEMDEV_Select(_hBkMemDev);
	GUI_DrawBitmap(&bmwendu,r.x0,r.y0);   //绘制位图
	GUI_MEMDEV_Select(hMemPrev);
}

/*
*********************************************************************************************************
*	函 数 名: _cbBkWindow
*	功能说明: 桌面窗口回调函数
*	形    参: pMsg  消息结构体
*	返 回 值: 无
*********************************************************************************************************
*/
static void _cbBkWindow(WM_MESSAGE* pMsg) 
{
	switch (pMsg->MsgId) 
	{
		
		case WM_PAINT:
			GUI_MEMDEV_Write(_hBkMemDev);
			break;
		default:
			WM_DefaultProc(pMsg);
	}
}

/*
*********************************************************************************************************
*	函 数 名: MainTask
*	功能说明: GUI主函数
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
//void MainTask(void) 
//{
//	/*
//	 关于多缓冲和窗口内存设备的设置说明
//	   1. 使能多缓冲是调用的如下函数，用户要在LCDConf_Lin_Template.c文件中配置了多缓冲，调用此函数才有效：
//		  WM_MULTIBUF_Enable(1);
//	   2. 窗口使能使用内存设备是调用函数：WM_SetCreateFlags(WM_CF_MEMDEV);
//	   3. 如果emWin的配置多缓冲和窗口内存设备都支持，二选一即可，且务必优先选择使用多缓冲，实际使用
//		  STM32F429BIT6 + 32位SDRAM + RGB565/RGB888平台测试，多缓冲可以有效的降低窗口移动或者滑动时的撕裂
//		  感，并有效的提高流畅性，通过使能窗口使用内存设备是做不到的。
//	   4. 所有emWin例子默认是开启三缓冲。
//	*/
//	WM_MULTIBUF_Enable(1);
//	
//		
//	/* 显示屏背景 */
//	_CreateBackGround();
//	
//	/* 设置桌面窗口的回调函数 */
//	WM_SetCallback(WM_HBKWIN, &_cbBkWindow);
//	
//	/* 同类的结构体变量可以直接赋值 */
//	_Scale[3] = _Presets[3][2];
////	_Scale[2] = _Presets[3][2];
//	
//	/* 创建表盘窗口 */
//	_CreateScaleWindow(&_Scale[3]);
////	_CreateScaleWindow(&_Scale[2]);
//	
//	while(1) 
//	{
//		/* 转动表盘指针 */
//		_MoveNeedle(&_Needle[3], 3);
//		
////		_MoveNeedle(&_Needle[2], 2);
//		GUI_Delay(1);
//	} 
//	
//	
//	
//}

///***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
