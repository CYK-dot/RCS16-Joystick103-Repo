#include "inc.h"

char str[16];
float Batt_Volt;
u16 DATA = 0x0000;
u16 RX,RY,LX,LY;
Remote_Struct remote_data;					//ң������
u8 Remote_Send_Data[8];            //���͵�����
u8 Remote_Receive_Data[10];					//���յ�����
u8	BEGIN_FLAG = 0xCE;							//��ʼ��־
u8	END_FLAG = 0xEC;								//������־
u16 ch0,ch1,ch2,ch3;
extern int16_t receive_data[10];

/*******************************************************************************
* �� �� ��         : init_allY
* ��������		   : ��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void init_all(void)
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  //�ж����ȼ����� ��2��
	
	USART1_Init(9600);			//������Ϊ9600
	ADC1_Init();							//ҡ��adc
	ADC2_Init();							//�����ɼ�adc
	TIMx_Init(TIM4, 300, 36000, 5);   //��ʱ3�룬����ʱʱ��Ϊ3s
	//TIMx_Init(TIM3, 1000, 7200, 6);   //��ʱ100ms
	KEY_Init();
	LED_Init();
	OLED_Init();
	Lora_Init();

	delay_ms(20);
	//printf("initialized successfully !");
}
/*******************************************************************************
* �� �� ��         : OLED_Show
* ��������		   : OLED��ʾ
* ��    ��         : NewState
* ��    ��         : ��
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
* �� �� ��         : Send_Data
* ��������		   : �����ֱ�����
* ��    ��         : NewState
* ��    ��         : ��
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
* �� �� ��         : main
* ��������		   : ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
int main()
{
	init_all();
	
	while(1)
	{
		stick_scan(ENABLE);						//��ȡҡ��ģ����ʹ��
		key_scan(ENABLE);							//��ȡ����ֵʹ��
//		Power_Indicate(ENABLE);				//��Դָʾ����ʾʹ��
		Connect_Indicate(ENABLE);			//����ָʾ����ʾʹ��
		SEND(ENABLE);									//�򴮿ڷ�������ʹ��
		OLED_Show(ENABLE);						//OLED��ʾʹ��
		
		delay_ms(5);	
	}
}

