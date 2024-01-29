#include "led.h"

/*******************************************************************************
* 函 数 名         : LED_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin=Power_LED_pin|Connect_LED_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GROUPA_gpio,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GROUPB_gpio,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //设置推挽输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GROUPA_gpio,Power_LED_pin);
	GPIO_ResetBits(GROUPA_gpio,Connect_LED_pin);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

/**************************************************************************
函数功能：电量指示灯显示
入口参数：无
返回  值：无
**************************************************************************/

void Power_Indicate(FunctionalState NewState)
{
	float Voltage;//电池电压
	static int Voltage_Count = 0;
	static int Voltage_Temp = 0;
	static int Voltage_All = 0;
	static int times = 100;
	if(NewState == ENABLE)
	{
		Voltage_Temp=Get_battery_volt();		                                //=====读取电池电压		
		Voltage_Count++;                                                    //=====平均值计数器
		Voltage_All+=Voltage_Temp;                                          //=====多次采样累积
		
		if(Voltage_Count==times) 
			Voltage=Voltage_All/times, Voltage_All=0, Voltage_Count=0;						//=====求平均值		

		if(Voltage < WARN_VOLT)
			GPIO_SetBits(GPIOA,Power_LED_pin);
		else
			GPIO_ResetBits(GPIOA,Power_LED_pin);
	}
	else
		return;

}



