#ifndef __OLED_H
#define __OLED_H

#include "inc.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
/******OLED�����ú���******/
void OLED_Init(void);						//oled��ʼ��
void OLED_Refresh_Gram(void);		//oled��Ļˢ��
void OLED_Clear(void);					//oled����
void OLED_ShowString(u8 x,u8 y,const u8 *p);	//��ʾ�ַ���

#endif  
	 
