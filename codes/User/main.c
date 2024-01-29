#include "inc.h"

char str[16];
float Batt_Volt;
u16 DATA = 0x0000;
u16 RX,RY,LX,LY;
Remote_Struct remote_data;					//遥控数据
u8 Remote_Send_Data[8];            //发送的数据
u8 Remote_Receive_Data[10];					//接收的数据
u8	BEGIN_FLAG = 0xCE;							//开始标志
u8	END_FLAG = 0xEC;								//结束标志
u16 ch0,ch1,ch2,ch3;
extern int16_t receive_data[10];

/*******************************************************************************
* 函 数 名         : init_allY
* 函数功能		   : 初始化函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void init_all(void)
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //中断优先级分组 分2组
	
	USART1_Init(9600);			//波特率为9600
	ADC1_Init();							//摇杆adc
	ADC2_Init();							//电量采集adc
	TIMx_Init(TIM4, 300, 36000, 5);   //定时3秒，即超时时间为3s
	//TIMx_Init(TIM3, 1000, 7200, 6);   //定时100ms
	KEY_Init();
	LED_Init();
	OLED_Init();
	Lora_Init();

	delay_ms(20);
	//printf("initialized successfully !");
}
/*******************************************************************************
* 函 数 名         : OLED_Show
* 函数功能		   : OLED显示
* 输    入         : NewState
* 输    出         : 无
*******************************************************************************/
void OLED_Show(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{

		sprintf(str,"KEY:%4x",remote_data.KEY);
		OLED_ShowString(0,0,(u8*)str);
		sprintf(str,"LX:%3d",remote_data.LX);
		OLED_ShowString(0,10,(u8*)str);
		sprintf(str,"LY:%3d",remote_data.LY);
		OLED_ShowString(0,20,(u8*)str);
		sprintf(str,"RX:%3d",remote_data.RX);
		OLED_ShowString(0,30,(u8*)str);
		sprintf(str,"RY:%3d",remote_data.RY);
		OLED_ShowString(0,40,(u8*)str);
//		sprintf(str,"speedx:%6d",rec.eive_data[0]);
//		OLED_ShowString(0,10,(u8*)str);
//		sprintf(str,"speedy:%6d",receive_data[1]);
//		OLED_ShowString(0,20,(u8*)str);
//		sprintf(str,"speedz:%6d",receive_data[2]);
//		OLED_ShowString(0,30,(u8*)str);
		OLED_Refresh_Gram();
	}
	else
		return;
}
/*******************************************************************************
* 函 数 名         : Send_Data
* 函数功能		   : 发送手柄数据
* 输    入         : NewState
* 输    出         : 无
*******************************************************************************/
static void SEND(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		Remote_Send_Data[0] = BEGIN_FLAG;  //0xCE
		Remote_Send_Data[1] = (u8)((remote_data.KEY & 0xff00) >> 8);
		Remote_Send_Data[2] = (u8)(remote_data.KEY & 0x00ff);
		Remote_Send_Data[3] = remote_data.LX;
		Remote_Send_Data[4] = remote_data.LY;
		Remote_Send_Data[5] = remote_data.RX;
		Remote_Send_Data[6] = remote_data.RY;
		Remote_Send_Data[7] = END_FLAG;   //0xEC
		
		USART_Send_Str(USART1,Remote_Send_Data, 8);
	}
	else
		return;
}
	
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	init_all();
	
	while(1)
	{
		stick_scan(ENABLE);						//获取摇杆模拟量使能
		key_scan(ENABLE);							//获取按键值使能
//		Power_Indicate(ENABLE);				//电源指示灯显示使能
		Connect_Indicate(ENABLE);			//连接指示灯显示使能
		SEND(ENABLE);									//向串口发送数据使能
		OLED_Show(ENABLE);						//OLED显示使能
		
		delay_ms(5);	
	}
}

