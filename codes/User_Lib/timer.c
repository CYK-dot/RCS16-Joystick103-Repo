#include "timer.h"

u8 receive_flag = 0;
static u8 over_flag = 0;
static u8 connected_flag = 0;

static uint32_t Get_RCC_APBxPeriph_TIM(TIM_TypeDef* TIMx);

static uint8_t My_Get_TIM_IRQn(TIM_TypeDef* TIMx);
/*******************************************************************************
* 函 数 名         : TIM4_Init
* 函数功能		   : TIM4初始化函数
* 输    入         : 	count:计数次数   (0-65535)
											_div:分频系数    (0-65535)    输出定时时间为（count * _div / 72 000 000）
* 输    出         : 无
*******************************************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 count,u16 _div, u8 pri)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(Get_RCC_APBxPeriph_TIM(TIMx),ENABLE);//使能TIM4时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = count - 1;   //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = _div - 1; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE); //开启定时器中断
	TIM_ClearITPendingBit(TIMx,TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = My_Get_TIM_IRQn(TIMx);//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pri;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_Cmd(TIMx,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
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
* 函 数 名         : Connect_Indicate
* 函数功能		   : 连接指示灯工作
* 输    入         : 状态（ENABLE or DISABLE）
* 输    出         : 无
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
			GPIO_SetBits(GROUPA_gpio,Connect_LED_pin);     //连接上则连接指示灯常量
		else
		{
			if(count <= times)
				GPIO_SetBits(GROUPA_gpio,Connect_LED_pin);
			else if(count <= (2*times))
			{
				GPIO_ResetBits(GROUPA_gpio,Connect_LED_pin);
				if(count == 2*times)
					count = 0;																	 //未连接上则连接指示灯闪烁闪烁时间间隔为times*主函数delay
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

