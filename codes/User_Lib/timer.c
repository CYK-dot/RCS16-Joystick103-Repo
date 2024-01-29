#include "timer.h"

u8 receive_flag = 0;
static u8 over_flag = 0;
static u8 connected_flag = 0;

static uint32_t Get_RCC_APBxPeriph_TIM(TIM_TypeDef* TIMx);

static uint8_t My_Get_TIM_IRQn(TIM_TypeDef* TIMx);
/*******************************************************************************
* �� �� ��         : TIM4_Init
* ��������		   : TIM4��ʼ������
* ��    ��         : 	count:��������   (0-65535)
											_div:��Ƶϵ��    (0-65535)    �����ʱʱ��Ϊ��count * _div / 72 000 000��
* ��    ��         : ��
*******************************************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 count,u16 _div, u8 pri)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(Get_RCC_APBxPeriph_TIM(TIMx),ENABLE);//ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = count - 1;   //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = _div - 1; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE); //������ʱ���ж�
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = My_Get_TIM_IRQn(TIMx);//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pri;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIMx,ENABLE); //ʹ�ܶ�ʱ��	
}

/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		over_flag = 1;
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update))
	{
		if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13))
				GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		else
			GPIOC->BSRR = GPIO_Pin_13 << 0;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
}

/*******************************************************************************
* �� �� ��         : Connect_Indicate
* ��������		   : ����ָʾ�ƹ���
* ��    ��         : ״̬��ENABLE or DISABLE��
* ��    ��         : ��
*******************************************************************************/

void Connect_Indicate(FunctionalState NewState)
{
	static u8 count = 0;
	static u8 times = 10;

	count++;
	
	if(NewState == ENABLE)
	{
		if(receive_flag == 0)
			connected_flag = 0;
		if((receive_flag==1)&&(over_flag==0))
			connected_flag = 1;
		if(over_flag == 1)
		{
			receive_flag = 0;
			over_flag = 0;
		}
		
		if(connected_flag)
			GPIO_SetBits(GROUPA_gpio,Connect_LED_pin);     //������������ָʾ�Ƴ���
		else
		{
			if(count <= times)
				GPIO_SetBits(GROUPA_gpio,Connect_LED_pin);
			else if(count <= (2*times))
			{
				GPIO_ResetBits(GROUPA_gpio,Connect_LED_pin);
				if(count == 2*times)
					count = 0;																	 //δ������������ָʾ����˸��˸ʱ����Ϊtimes*������delay
			}
		}
	}
	else
		return;
}

static uint32_t Get_RCC_APBxPeriph_TIM(TIM_TypeDef* TIMx)
{
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	if (TIMx == TIM1)		return RCC_APB2Periph_TIM1;
	else if (TIMx == TIM2)	return RCC_APB1Periph_TIM2;
	else if (TIMx == TIM3)	return RCC_APB1Periph_TIM3;
	else if (TIMx == TIM4)	return RCC_APB1Periph_TIM4;
	return NULL;
}

static uint8_t My_Get_TIM_IRQn(TIM_TypeDef* TIMx)
{
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	if (TIMx == TIM1)		return TIM1_UP_IRQn;
	else if (TIMx == TIM2)	return TIM2_IRQn;
	else if (TIMx == TIM3)	return TIM3_IRQn;
	else if (TIMx == TIM4)	return TIM4_IRQn;
	return NULL;
}

