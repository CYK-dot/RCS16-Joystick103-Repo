#include "adc.h"

extern Remote_Struct remote_data;

u8 NUM_CH = 4;
u8 NUM_AVER = 20;
u16 AD_Value[20][4];//用来存放ADC转换结果，也是DMA的目标地址,3通道，每通道采集10次后面取平均数 


void DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能时钟

    DMA_DeInit(DMA1_Channel1);    //将通道一寄存器设为默认值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);//该参数用以定义DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_Value;//该参数用以定义DMA内存基地址(转换结果保存的地址)
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//该参数规定了外设是作为数据传输的目的地还是来源，此处是作为来源
    DMA_InitStructure.DMA_BufferSize = NUM_CH*NUM_AVER;//定义指定DMA通道的DMA缓存的大小,单位为数据单位。这里也就是ADCConvertedValue的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//设定外设地址寄存器递增与否,此处设为不变 Disable
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//用来设定内存地址寄存器递增与否,此处设为递增，Enable
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMA通道拥有高优先级 分别4个等级 低、中、高、非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//使能DMA通道的内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA的通道

    DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道一
}

/*******************************************************************************
* 函 数 名         : ADCx_Init
* 函数功能		   : ADC初始化	
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void ADCx_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_pinLX|ADC_pinLY|ADC_pinRX|ADC_pinRY;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = 4;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	ADC_RegularChannelConfig(ADC1, LX_CH, 1, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, LY_CH, 2, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, RX_CH, 3, ADC_SampleTime_239Cycles5 ); 
  ADC_RegularChannelConfig(ADC1, RY_CH, 4, ADC_SampleTime_239Cycles5 ); 
   
  // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数） 
  ADC_DMACmd(ADC1, ENABLE); 
  
	ADC_Cmd(ADC1, ENABLE);  //使能指定的ADC1 
       
  ADC_ResetCalibration(ADC1); //使能复位校准   
        
  while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束 
       
  ADC_StartCalibration(ADC1);  //开启AD校准 
    
  while(ADC_GetCalibrationStatus(ADC1));   //等待校准结束 
  DMA_Config();  		
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能指定的ADC1的软件转换启动功能	

}

/*******************************************************************************
* 函 数 名         : Get_ADC_Value
* 函数功能		   : 获取通道ch的转换值，取times次,然后平均 	
* 输    入         : ch:通道编号
					 times:获取次数
* 输    出         : 通道ch的times次转换结果平均值
*******************************************************************************/
void Get_DMA_ADC_Value(void)
{
	u8 i=0;
	u8 j=0;
	int sum;
	u8 data[4] = {0};
	float temp = 0;
	//设置指定ADC的规则组通道，一个序列，采样时间
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
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_pinLX|ADC_pinLY|ADC_pinRX|ADC_pinRY;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
  
	ADC_Cmd(ADC1, ENABLE);  //使能指定的ADC1 
       
  ADC_ResetCalibration(ADC1); //使能复位校准   
        
  while(ADC_GetResetCalibrationStatus(ADC1)); //等待复位校准结束 
       
  ADC_StartCalibration(ADC1);  //开启AD校准 
    
  while(ADC_GetCalibrationStatus(ADC1));   //等待校准结束 

}


/*******************************************************************************
* 函 数 名         : ADC2_Init
* 函数功能		   : ADC2初始化	
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void ADC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC2,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	GPIO_InitStructure.GPIO_Pin = ADC_Volt_pin;//ADC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfChannel = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
	
	ADC_RegularChannelConfig(ADC2, ADC_Volt_CH, 1, ADC_SampleTime_239Cycles5 ); 
  
  // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数） 
 // ADC_DMACmd(ADC1, ENABLE); 
  
	ADC_Cmd(ADC2, ENABLE);  //使能指定的ADC1  
  ADC_ResetCalibration(ADC2); //使能复位校准       
  while(ADC_GetResetCalibrationStatus(ADC2)); //等待复位校准结束    
  ADC_StartCalibration(ADC2);  //开启AD校准 
  while(ADC_GetCalibrationStatus(ADC2));   //等待校准结束 

}


/*******************************************************************************
* 函 数 名         : Get_ADC1_Value
* 函数功能		   : 获取通道ch的转换值，取times次,然后平均 	
* 输    入         : ch:通道编号
					 times:获取次数
* 输    出         : 通道ch的times次转换结果平均值
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
* 函 数 名         : Get_ADC2_Value
* 函数功能		   : 获取通道ch的转换值，取times次,然后平均 	
* 输    入         : ch:通道编号
					 times:获取次数
* 输    出         : 通道ch的times次转换结果平均值
*******************************************************************************/
u16 Get_ADC_Value(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
	ADC_RegularChannelConfig(ADCx, ADC_Channel, 1, ADC_SampleTime_239Cycles5 ); 
	ADC_SoftwareStartConvCmd(ADCx, ENABLE);		//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADCx);	//返回最近一次ADC2规则组的转换结果
} 

/**************************************************************************
函数功能：读取电池电压 
入口参数：无
返回  值：电池电压 单位MV
**************************************************************************/
float Get_battery_volt(void)   
{  
	float Voltage;//电池电压
	
			Voltage = (float)(Get_ADC_Value(ADC1, ADC_Channel_0)/4095*MAX_VOLT); 		//此处添加电压转换公式 Volt=Get_Adc(Battery_Ch)*3.3*11.5*100/1.5/4096;	//电阻分压，具体根据原理图简单分析可以得到	
	return Voltage;
}
