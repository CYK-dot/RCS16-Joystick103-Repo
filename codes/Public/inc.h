#ifndef _INC_H
#define _INC_H


#include "stm32f10x.h"
#include "systick.h"
#include "usart.h"
#include "adc.h"
#include "key.h"
#include "oled.h"
#include "led.h"
#include "bitband.h"
#include "systick.h"
#include "lora.h"
#include "timer.h"

typedef struct
 {
	u8 LX;			//right Joystick left-right
	u8 LY;			//right Joystick ahead-back
	u8 RX;			//left Joystick left-right
	u8 RY;			//left Joystick ahead-back
	u16 KEY;				//left switch
 }Remote_Struct;					//遥控数据结构体

 
#define MAX_VOLT 7.4			//电池电压最大值
#define WARN_VOLT 6.8  		//报警电压
 
//-------------A组GPIO--------------
#define 	GROUPA_gpio 	GPIOA

#define 	ADC_pinLX 	GPIO_Pin_3
#define 	ADC_pinLY 	GPIO_Pin_2
#define 	ADC_pinRX 	GPIO_Pin_1
#define 	ADC_pinRY 	GPIO_Pin_0

#define 	LX_CH				ADC_Channel_3
#define 	LY_CH				ADC_Channel_2
#define 	RX_CH				ADC_Channel_1
#define 	RY_CH				ADC_Channel_0

#define		UP_pin		GPIO_Pin_7
#define		DOWN_pin	GPIO_Pin_4
#define		LEFT_pin	GPIO_Pin_6
#define		RIGHT_pin	GPIO_Pin_5

#define		Power_LED_pin	GPIO_Pin_11
#define		Connect_LED_pin	GPIO_Pin_12

#define		Usart1_TX_pin	GPIO_Pin_9
#define		Usart1_RX_pin	GPIO_Pin_10

#define		Switch2_pin	GPIO_Pin_8


//-------------B组GPIO--------------
#define 	GROUPB_gpio 	GPIOB

#define Undef_LED1			GPIO_Pin_3

#define		ADC_Volt_pin	GPIO_Pin_0
#define 	ADC_Volt_CH				ADC_Channel_0

#define		KEY1_pin	GPIO_Pin_9
#define		KEY2_pin	GPIO_Pin_8
#define		KEY3_pin	GPIO_Pin_7
#define		KEY4_pin	GPIO_Pin_6
#define		KEY5_pin	GPIO_Pin_4
#define		KEY6_pin	GPIO_Pin_5

#define		Switch1_pin	GPIO_Pin_15

#define		Usart3_TX_pin	GPIO_Pin_10
#define		Usart3_RX_pin	GPIO_Pin_11

#define		Lora_AUX	GPIO_Pin_14

#define		OLED_SCL_pin	GPIO_Pin_10
#define		OLED_SDA_pin	GPIO_Pin_11
#define		OLED_RST_pin	GPIO_Pin_1
#define		OLED_DC_pin	GPIO_Pin_2
/******定义OLED位操作******/
#define 	OLED_SCLK_Clr()  PBout(10)=0  //SCL
#define 	OLED_SCLK_Set()  PBout(10)=1   //SCL
#define 	OLED_SDIN_Clr()  PBout(11)=0   //SDA
#define 	OLED_SDIN_Set()  PBout(11)=1   //SDA
#define 	OLED_RST_Clr() PBout(1)=0   //RST
#define 	OLED_RST_Set() PBout(1)=1   //RST
#define 	OLED_RS_Clr() PBout(2)=0    //DC
#define 	OLED_RS_Set() PBout(2)=1    //DC

//-------------C组GPIO--------------
#define 	GROUPC_gpio 	GPIOC

#define Stick1_KEY GPIO_Pin_15
#define Stick2_KEY GPIO_Pin_14

#define Undef_LED2 				GPIO_Pin_13

#endif

