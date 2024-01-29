#include "adc.h"

extern Remote_Struct remote_data;

u8 NUM_CH = 4;
u8 NUM_AVER = 20;
u16 AD_Value[20][4];//�������ADCת�������Ҳ��DMA��Ŀ���ַ,3ͨ����ÿͨ���ɼ�10�κ���ȡƽ���� 


void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��ʱ��

    DMA_DeInit(DMA1_Channel1);    //��ͨ��һ�Ĵ�����ΪĬ��ֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//�ò������Զ���DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;//�ò������Զ���DMA�ڴ����ַ(ת���������ĵ�ַ)
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//�ò����涨����������Ϊ���ݴ����Ŀ�ĵػ�����Դ���˴�����Ϊ��Դ
    DMA_InitStructure.DMA_BufferSize = NUM_CH*NUM_AVER;//����ָ��DMAͨ����DMA����Ĵ�С,��λΪ���ݵ�λ������Ҳ����ADCConvertedValue�Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�趨�����ַ�Ĵ����������,�˴���Ϊ���� Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����趨�ڴ��ַ�Ĵ����������,�˴���Ϊ������Enable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ��ӵ�и����ȼ� �ֱ�4���ȼ� �͡��С��ߡ��ǳ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//ʹ��DMAͨ�����ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

    DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ��һ
}

/*******************************************************************************
* �� �� ��         : ADCx_Init
* ��������		   : ADC��ʼ��	
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_pinLX|ADC_pinLY|ADC_pinRX|ADC_pinRY;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 4;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_RegularChannelConfig(ADC1, LX_CH, 1, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, LY_CH, 2, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, RX_CH, 3, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, RY_CH, 4, ADC_SampleTime_239Cycles5 ); 
   
  // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ����� 
  ADC_DMACmd(ADC1, ENABLE); 
  
	ADC_Cmd(ADC1, ENABLE);  //ʹ��ָ����ADC1 
       
  ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼   
        
  while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼���� 
       
  ADC_StartCalibration(ADC1);  //����ADУ׼ 
    
  while(ADC_GetCalibrationStatus(ADC1));   //�ȴ�У׼���� 
  DMA_Config();  		
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ����ADC1�����ת����������	

}

/*******************************************************************************
* �� �� ��         : Get_ADC_Value
* ��������		   : ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 	
* ��    ��         : ch:ͨ�����
					 times:��ȡ����
* ��    ��         : ͨ��ch��times��ת�����ƽ��ֵ
*******************************************************************************/
void Get_DMA_ADC_Value(void)
{
	u8 i=0;
	u8 j=0;
	int sum;
	u8 data[4] = {0};
	float temp = 0;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	for(i=0;i<NUM_CH;i++)
	{
		sum=0;
		for(j=0;j<NUM_AVER;j++)
		{
			 sum += AD_Value[j][i];
		}
		temp = sum / NUM_AVER /16;
		if(temp >= 255)
			temp = 255;
		data[i] = (u8)temp;
	}
	remote_data.LX = data[0];
	remote_data.LY = data[1];
	remote_data.RX = data[2];
	remote_data.RY = data[3];
} 

void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_pinLX|ADC_pinLY|ADC_pinRX|ADC_pinRY;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
  
	ADC_Cmd(ADC1, ENABLE);  //ʹ��ָ����ADC1 
       
  ADC_ResetCalibration(ADC1); //ʹ�ܸ�λУ׼   
        
  while(ADC_GetResetCalibrationStatus(ADC1)); //�ȴ���λУ׼���� 
       
  ADC_StartCalibration(ADC1);  //����ADУ׼ 
    
  while(ADC_GetCalibrationStatus(ADC1));   //�ȴ�У׼���� 

}


/*******************************************************************************
* �� �� ��         : ADC2_Init
* ��������		   : ADC2��ʼ��	
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void ADC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC2,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_Volt_pin;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_RegularChannelConfig(ADC2, ADC_Volt_CH, 1, ADC_SampleTime_239Cycles5 ); 
  
  // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ����� 
 // ADC_DMACmd(ADC1, ENABLE); 
  
	ADC_Cmd(ADC2, ENABLE);  //ʹ��ָ����ADC1  
  ADC_ResetCalibration(ADC2); //ʹ�ܸ�λУ׼       
  while(ADC_GetResetCalibrationStatus(ADC2)); //�ȴ���λУ׼����    
  ADC_StartCalibration(ADC2);  //����ADУ׼ 
  while(ADC_GetCalibrationStatus(ADC2));   //�ȴ�У׼���� 

}


/*******************************************************************************
* �� �� ��         : Get_ADC1_Value
* ��������		   : ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 	
* ��    ��         : ch:ͨ�����
					 times:��ȡ����
* ��    ��         : ͨ��ch��times��ת�����ƽ��ֵ
*******************************************************************************/
void stick_scan(FunctionalState NewState)
{
	u8 CHx[4] = {LX_CH, LY_CH, RX_CH, RY_CH};

		remote_data.LX = Get_ADC_Value(ADC1 , CHx[0])/16;
		remote_data.LY = Get_ADC_Value(ADC1 , CHx[1])/16;
		remote_data.RX = Get_ADC_Value(ADC1 , CHx[2])/16;
		remote_data.RY = Get_ADC_Value(ADC1 , CHx[3])/16;
	
	
} 

/*******************************************************************************
* �� �� ��         : Get_ADC2_Value
* ��������		   : ��ȡͨ��ch��ת��ֵ��ȡtimes��,Ȼ��ƽ�� 	
* ��    ��         : ch:ͨ�����
					 times:��ȡ����
* ��    ��         : ͨ��ch��times��ת�����ƽ��ֵ
*******************************************************************************/
u16 Get_ADC_Value(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_239Cycles5 ); 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//ʹ��ָ����ADC1�����ת����������		 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADCx);	//�������һ��ADC2�������ת�����
} 

/**************************************************************************
�������ܣ���ȡ��ص�ѹ 
��ڲ�������
����  ֵ����ص�ѹ ��λMV
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Voltage;//��ص�ѹ
	
			Voltage = (float)(Get_ADC_Value(ADC1, ADC_Channel_0)/4095*MAX_VOLT); 		//�˴���ӵ�ѹת����ʽ Volt=Get_Adc(Battery_Ch)*3.3*11.5*100/1.5/4096;	//�����ѹ���������ԭ��ͼ�򵥷������Եõ�	
	return Voltage;
}
