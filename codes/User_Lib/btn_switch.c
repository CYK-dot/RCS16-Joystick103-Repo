#include "btn_switch.h"

/*******************************************************************************
* �� �� ��         : SWITCH_Init
* ��������		   : ������ʼ��
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void SWITCH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		
	GPIO_InitStructure.GPIO_Pin = Switch1_pin|Switch2_pin|Switch3_pin;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GROUPB_gpio,&GPIO_InitStructure);

}
