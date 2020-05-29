/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : uartdev.c
* Author             : Tiko Zhong
* Date First Issued  : 04/20/2020
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 04/20/2020: V0.1
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "publicRsrc.h"
#include "uartDev.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "crc16.h"
#include "stdlib.h"

/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_FRAM_HEAD	(0xed98ba)
#define UART_FRAM_END	(0x89abcd)
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static u8 uartRxMonitor(UartRsrc_t *pRsrc);
static u8 uartRxFetchLine(UartRsrc_t *pRsrc, char* line, u16 len);
static u8* uartRxFetchFrame(UartRsrc_t *pRsrc, u16* len);
static u16 uartTxSend(UartRsrc_t *pRsrc, const u8* BUF, u16 len);
static u16 uartTxSendFrame(UartRsrc_t *pRsrc, const u8* BUF, u16 len);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : uartSrscSetup
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void setupUartDev(
	UartDev_t *pDev, 
	UART_HandleTypeDef* huart,
	u8* p,	/*	all memmory	*/
	u16	rxPoolLen,
	u16 rxBufLen,
	u16 txBufLen
){
	UartRsrc_t *pRsrc = &pDev->rsrc;
	pRsrc->huart = huart;
	
	memset(p,0,(rxPoolLen+2*rxBufLen+txBufLen));
	pRsrc->rxPool = p;
	pRsrc->rxBuf0 = p+rxPoolLen;
	pRsrc->rxBuf1 = p+rxPoolLen+rxBufLen;
	pRsrc->txBuf = p+rxPoolLen+2*rxBufLen;

	pRsrc->rxCurBuf = pRsrc->rxBuf0;
	pRsrc->rxNxtBuf = pRsrc->rxBuf1;
	pRsrc->rxPoolIndx = 0;
	pRsrc->rxBufLen = rxBufLen;
	pRsrc->rxPoolLen = rxPoolLen-1;
	pRsrc->txBufLen = txBufLen;
	pRsrc->txIndx = 0;
	
	//register op
	pDev->RxMonitor = uartRxMonitor;
	pDev->RxFetchLine = uartRxFetchLine;
	pDev->RxFetchFrame = uartRxFetchFrame;
	pDev->TxSend = uartTxSend;
	pDev->TxSendFrame = uartTxSendFrame;
	
	//start to receive
	while(HAL_UART_Receive_IT(huart, pRsrc->rxCurBuf, pRsrc->rxBufLen) == HAL_OK){}
}

void uartTxMakeFrame(u8* p, const u8* BUF, u16 len, u16 crc){
	*p = UART_FRAM_HEAD&0xff;	p++;
	*p = (UART_FRAM_HEAD>>8)&0xff;	p++;
	*p = (UART_FRAM_HEAD>>16)&0xff;	p++;
	memcpy(p, BUF, len);	p+=len;
	*p = crc&0xff;	p++;
	*p = (crc>>8)&0xff;	p++;
	*p = UART_FRAM_END&0xff;	p++;
	*p = (UART_FRAM_END>>8)&0xff;	p++;
	*p = (UART_FRAM_END>>16)&0xff;	
}

static u16 uartTxSend(UartRsrc_t *pRsrc, const u8* BUF, u16 len){
	u16 ret = len;
	if(BUF == NULL || len==0)	return 0;
	if(pRsrc->huart->gState == HAL_UART_STATE_READY){
		memcpy(pRsrc->txBuf, BUF, len);
		if(HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, len) != HAL_OK)	return 0;
		pRsrc->txIndx = len;
	}
	else{
		/* Disable interrupts */
		CLEAR_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
		memcpy(&pRsrc->txBuf[pRsrc->txIndx], BUF, len);
    pRsrc->huart->TxXferSize  += ret;
    pRsrc->huart->TxXferCount += ret;
		SET_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
		pRsrc->txIndx += len;
	}
	return ret;
}

static u16 uartTxSendFrame(UartRsrc_t *pRsrc, const u8* BUF, u16 len){
	u16 ret;
	u16 crc;
	if(BUF == NULL || len==0)	return 0;
	crc = CRC16(BUF, len, 0xacca);
	ret = (len+8);
	if(pRsrc->huart->gState == HAL_UART_STATE_READY){
		uartTxMakeFrame(pRsrc->txBuf, BUF, len, crc);
		if(HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, ret) != HAL_OK)	return 0;
		pRsrc->txIndx = ret;
	}
	else{
		/* Disable interrupts */
		CLEAR_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
		uartTxMakeFrame(&pRsrc->txBuf[pRsrc->txIndx], BUF, len, crc);
    pRsrc->huart->TxXferSize  += ret;
    pRsrc->huart->TxXferCount += ret;
		SET_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
		pRsrc->txIndx += ret;
	}
	return ret;
}

//static u16 uartTxSend(UartRsrc_t *pRsrc, const u8* BUF, u16 len){
//	u16 ret = 0;
//	if(BUF == NULL || len==0)	return 0;
//	if(pRsrc->huart->gState == HAL_UART_STATE_READY){
//		memcpy(pRsrc->txBuf, BUF, len);
//		if(HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, len) == HAL_OK)	ret = len;
//		pRsrc->txIndx = len;
//	}
//	else{
//		/* Disable interrupts */
//		CLEAR_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
//		memcpy(&pRsrc->txBuf[pRsrc->txIndx], BUF, len);
//    pRsrc->huart->TxXferSize  += len;
//    pRsrc->huart->TxXferCount += len;
//		SET_BIT(pRsrc->huart->Instance->CR1, (USART_CR1_TXEIE | USART_CR1_TCIE));
//		pRsrc->txIndx += len;
//		ret = len;
//	}
//	return ret;
//}


static u8 uartRxMonitor(UartRsrc_t *pRsrc){
	u16 count,tmp;
	u8* pTmp;
	UART_HandleTypeDef *huart = pRsrc->huart;
	
//  __HAL_LOCK(huart);
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));		
	count = huart->RxXferCount;
	//restart uart
	huart->pRxBuffPtr  = pRsrc->rxNxtBuf;
	huart->RxXferCount = pRsrc->rxBufLen;
	SET_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
//	__HAL_UNLOCK(huart);
	
	count = huart->RxXferSize - count;
	if(count == 0){	/* nothing copy buffer to pool */}
	else{
		//copy pCurRcvUi to buffer
		if(pRsrc->rxPoolLen < pRsrc->rxPoolIndx)		pRsrc->rxPoolIndx = pRsrc->rxPoolLen;
		tmp = pRsrc->rxPoolLen - pRsrc->rxPoolIndx;	//empty memory to copy to
		if(count > tmp){
			tmp = count - tmp;	//need to loop count
			memcpy(pRsrc->rxPool, &pRsrc->rxPool[tmp], pRsrc->rxPoolLen - tmp);
			pRsrc->rxPoolIndx -= tmp;
			memset(&pRsrc->rxPool[pRsrc->rxPoolIndx],0,pRsrc->rxPoolLen - pRsrc->rxPoolIndx);
		}
		memcpy(&pRsrc->rxPool[pRsrc->rxPoolIndx], pRsrc->rxCurBuf, count);
		pRsrc->rxPoolIndx += count;
	}

	//swap pool, get ready for next buffer
	pTmp = pRsrc->rxCurBuf;
	pRsrc->rxCurBuf = pRsrc->rxNxtBuf;
	pRsrc->rxNxtBuf = pTmp;
	memset(pRsrc->rxNxtBuf,0,pRsrc->rxBufLen);
	return (count);
}

static u8 uartRxFetchLine(UartRsrc_t *pRsrc, char* line, u16 len){
	u8 ret = 0;
	char *p;
	u16 i,j,tmp;
	
	if(pRsrc->rxPoolIndx){
		for(i=0;i<pRsrc->rxPoolIndx;i++){
			p = strstr((char*)&pRsrc->rxPool[i], "\r\n");
			if(p){
				j = p-(char*)&pRsrc->rxPool[i]+2;
				tmp = (j>len?len:j);
				memset(line,0,len);
				memcpy(line,&pRsrc->rxPool[i],tmp);
				//loop
				tmp = p+2-(char*)pRsrc->rxPool;	//need to loop count
				memcpy(pRsrc->rxPool, &pRsrc->rxPool[tmp], pRsrc->rxPoolLen - tmp);
				pRsrc->rxPoolIndx -= tmp;
				memset(&pRsrc->rxPool[pRsrc->rxPoolIndx],0,pRsrc->rxPoolLen - pRsrc->rxPoolIndx);
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

static u8* uartRxFetchFrame(UartRsrc_t *pRsrc, u16* len){
	u16 i,j,crc0,crc1;
	u8 *head, *end, *pCrc, *frame;
	
	*len = 0;
	if(pRsrc->rxPoolIndx > 8){
		head = NULL;
		for(i=0;i<pRsrc->rxPoolIndx-3;i++){
			if( (pRsrc->rxPool[i]==(UART_FRAM_HEAD&0xff))	&&
					(pRsrc->rxPool[i+1]==((UART_FRAM_HEAD>>8)&0xff))	&&
					(pRsrc->rxPool[i+2]==((UART_FRAM_HEAD>>16)&0xff))	)
			{
				head = &pRsrc->rxPool[i];
				j = i+3+2;
				break;
			}
		}
		if(head==NULL)	return NULL;
	
		end = NULL;
		for(i=j;i<=pRsrc->rxPoolIndx-3;i++){
			if( (pRsrc->rxPool[i]==(UART_FRAM_END&0xff))	&&
					(pRsrc->rxPool[i+1]==((UART_FRAM_END>>8)&0xff))	&&
					(pRsrc->rxPool[i+2]==((UART_FRAM_END>>16)&0xff))	)
			{
				end = &pRsrc->rxPool[i];
				break;
			}
		}

		if(end==NULL)	return NULL;

		pCrc = end -1;
		crc0 = 0;
		crc0 = *pCrc;	crc0<<=8;	pCrc--;
		crc0 |= *pCrc;

		i = pCrc-head-3;	//payload len
		frame = malloc(i);
				
		memcpy(frame,head+3,i);
		*len = i;
		
		//loop
		head = end+3;
		end = &pRsrc->rxPool[pRsrc->rxPoolIndx];
		i = end-head;	//need to loop count
		memcpy(pRsrc->rxPool, head, i);
		pRsrc->rxPoolIndx = i;
		return frame;
	}
	return NULL;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
