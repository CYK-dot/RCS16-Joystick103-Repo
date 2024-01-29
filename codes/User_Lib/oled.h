#ifndef __OLED_H
#define __OLED_H

#include "inc.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
/******OLED控制用函数******/
void OLED_Init(void);						//oled初始化
void OLED_Refresh_Gram(void);		//oled屏幕刷新
void OLED_Clear(void);					//oled清屏
void OLED_ShowString(u8 x,u8 y,const u8 *p);	//显示字符串

#endif  
	 
