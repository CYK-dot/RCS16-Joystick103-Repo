#ifndef __usart_H
#define __usart_H

#include "inc.h" 
#include "stdio.h" 

void USART1_Init(u32 bound);
void USART2_Init(u32 bound);
void USART3_Init(u32 bound);

//@name: USART_Send_Str
//@brief: ´®¿Ú·¢ËÍ×Ö·û´®
//@param:_USARTx   USARTºÅ
//@param:_TxBuffer ×Ö·û´®
__inline void USART_Send_Str(USART_TypeDef *_USARTx, uint8_t _TxBuffer[], u8 length)
{
	uint8_t i = 0;
	while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	for (;i< length; i++)
	{
		USART_SendData(_USARTx, (uint16_t)_TxBuffer[i]);
		while (USART_GetFlagStatus(_USARTx, USART_FLAG_TC) == RESET);
	}
}

#endif


