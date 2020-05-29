/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : uartdev.h
* Author             : Tiko Zhong
* Date First Issued  : 04/20/2020
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 04/20/2020: V0.1
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_DEV_H
#define _UART_DEV_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"

/* Exported types ------------------------------------------------------------*/
typedef struct{
	UART_HandleTypeDef* huart;
	//rx parameter
	u8 *rxPool, *rxBuf0, *rxBuf1, *rxCurBuf, *rxNxtBuf;
	u16 rxPoolLen, rxBufLen;
	u16 rxPoolIndx;
	//tx parameter
	u8 *txBuf;
	u16 txIndx;
	u16 txBufLen;
	//callback
	void (*beforeSend)(void);
	void (*afterSend)(void);
}UartRsrc_t;

typedef struct{
	UartRsrc_t rsrc;
	u8 (*RxMonitor)		(UartRsrc_t *pRsrc);
	u8 (*RxFetchLine)	(UartRsrc_t *pRsrc, char* line, u16 len);
	u8* (*RxFetchFrame)	(UartRsrc_t *pRsrc, u16* len);
	u16 (*TxSend)		(UartRsrc_t *pRsrc, const u8* BUF, u16 len);
	u16 (*TxSendFrame)	(UartRsrc_t *pRsrc, const u8* BUF, u16 len);
}UartDev_t;

/* Exported variables --------------------------------------------------------*/
//extern u8 rxBufIndx;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void setupUartDev(
	UartDev_t *pDev, 
	UART_HandleTypeDef* huart,
	u8* p,	/*	all memmory	*/
	u16	rxPoolLen,
	u16 rxBufLen,
	u16 txBufLen
);

#endif /* _MY_UART_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
