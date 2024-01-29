#include "led.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin=Power_LED_pin|Connect_LED_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GROUPA_gpio,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GROUPB_gpio,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //�����������ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_SetBits(GROUPA_gpio,Power_LED_pin);
	GPIO_ResetBits(GROUPA_gpio,Connect_LED_pin);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

/**************************************************************************
�������ܣ�����ָʾ����ʾ
��ڲ�������
����  ֵ����
**************************************************************************/

void Power_Indicate(FunctionalState NewState)
{
	float Voltage;//��ص�ѹ
	static int Voltage_Count = 0;
	static int Voltage_Temp = 0;
	static int Voltage_All = 0;
	static int times = 100;
	if(NewState == ENABLE)
	{
		Voltage_Temp=Get_battery_volt();		                                //=====��ȡ��ص�ѹ		
		Voltage_Count++;                                                    //=====ƽ��ֵ������
		Voltage_All+=Voltage_Temp;                                          //=====��β����ۻ�
		
		if(Voltage_Count==times) 
			Voltage=Voltage_All/times, Voltage_All=0, Voltage_Count=0;						//=====��ƽ��ֵ		

		if(Voltage < WARN_VOLT)
			GPIO_SetBits(GPIOA,Power_LED_pin);
		else
			GPIO_ResetBits(GPIOA,Power_LED_pin);
	}
	else
		return;

}



