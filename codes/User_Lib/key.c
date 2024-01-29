#include "key.h"

extern Remote_Struct remote_data;

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin=UP_pin|LEFT_pin|RIGHT_pin|DOWN_pin|Switch2_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=KEY1_pin|KEY2_pin|KEY3_pin|KEY4_pin|KEY5_pin|KEY6_pin|Switch1_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin= Stick1_KEY| Stick2_KEY;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

}

void key_scan(FunctionalState NewState)
{
	u16 key_data = 0x0000;

	if(NewState == ENABLE)
	{
		if(!GPIO_ReadInputDataBit(GPIOA,UP_pin))
			key_data |=0x0001<<0;
		else
			key_data &=~(0x0001<<0);
		
		if(!GPIO_ReadInputDataBit(GPIOA,DOWN_pin))
			key_data |=0x0001<<1;
		else
			key_data &=~(0x0001<<1);
		
		if(!GPIO_ReadInputDataBit(GPIOA,LEFT_pin))
			key_data |=0x0001<<2;
		else
			key_data &=~(0x0001<<2);
		
		if(!GPIO_ReadInputDataBit(GPIOA,RIGHT_pin))
			key_data |=0x0001<<3;
		else
			key_data &=~(0x0001<<3);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY1_pin))
			key_data |=0x0001<<4;
		else
			key_data &=~(0x0001<<4);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY2_pin))
			key_data |=0x0001<<5;
		else
			key_data &=~(0x0001<<5);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY3_pin))
			key_data |=0x0001<<6;
		else
			key_data &=~(0x0001<<6);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY4_pin))
			key_data |=0x0001<<7;
		else
			key_data &=~(0x0001<<7);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY5_pin))
			key_data |=0x0001<<8;
		else
			key_data &=~(0x0001<<8);

		if(!GPIO_ReadInputDataBit(GPIOB,KEY6_pin))
			key_data |=0x0001<<9;
		else
			key_data &=~(0x0001<<9);

		if(!GPIO_ReadInputDataBit(GPIOB,Switch1_pin))
			key_data |=0x0001<<10;
		else
			key_data &=~(0x0001<<10);

		if(!GPIO_ReadInputDataBit(GPIOA,Switch2_pin))
			key_data |=0x0001<<11;
		else
			key_data &=~(0x0001<<11);

		if(!GPIO_ReadInputDataBit(GPIOC,Stick1_KEY))
			key_data |=0x0001<<12;
		else
			key_data &=~(0x0001<<12);
		
		if(!GPIO_ReadInputDataBit(GPIOC,Stick2_KEY))
			key_data |=0x0001<<13;
		else
			key_data &=~(0x0001<<13);

		remote_data.KEY = key_data;
	}
	else
		return;
}



