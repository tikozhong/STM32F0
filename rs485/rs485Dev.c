/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : rs485Dev.c
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
#include "rs485Dev.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//#include "board.h"

/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static u8 rs485RxMonitor(Rs485Rsrc_t *pRsrc);
static u8* rs485RxFetchFrame(Rs485Rsrc_t *pRsrc, u16* len);
//static u16 rs485TxMonitor(Rs485Rsrc_t *pRsrc);
static u16 rs485TxSend(Rs485Rsrc_t *pRsrc, const u8* BUF, u16 len);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : uartSrscSetup
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void setupRs485Dev(
	Rs485Dev_t *pDev,
	UART_HandleTypeDef* huart,
	u8* p,	/*	all memmory	*/
	u16	rxPoolLen,
	u16 rxBufLen,
	u16 txBufLen,
	const PIN_T DE,
	const PIN_T DET
){
	Rs485Rsrc_t *pRsrc = &pDev->rsrc;
	
	setupUartDev(&pRsrc->uartdev, huart,
		p,	/*	all memmory	*/
		rxPoolLen,
		rxBufLen,
		txBufLen
	);
	
	pRsrc->DE = DE;
	pRsrc->DET = DET;
	pRsrc->squ = 0;
	
	pDev->RxMonitor = rs485RxMonitor;
	pDev->RxFetchFrame = rs485RxFetchFrame;
	pDev->TxSend = rs485TxSend;

	HAL_GPIO_WritePin(pRsrc->DE.GPIOx, pRsrc->DE.GPIO_Pin, GPIO_PIN_RESET);
}

static u16 rs485TxSend(Rs485Rsrc_t *pRsrc, const u8* BUF, u16 len){
	u16 ret = 0;
	UART_HandleTypeDef* huart = pRsrc->uartdev.rsrc.huart;
	UartDev_t* pUartDev = &pRsrc->uartdev;
	
	if(BUF == NULL || len == 0 )	return 0;
	
	if(HAL_GPIO_ReadPin(pRsrc->DET.GPIOx, pRsrc->DET.GPIO_Pin) == GPIO_PIN_SET)	return 0;	
	HAL_GPIO_WritePin(pRsrc->DE.GPIOx, pRsrc->DE.GPIO_Pin, GPIO_PIN_SET);	
  /* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));	

	pUartDev->TxSendFrame(&pUartDev->rsrc, BUF, len);
	pRsrc->squ = 1;
	
	return ret;
}

static u8 rs485RxMonitor(Rs485Rsrc_t *pRsrc){
	return (pRsrc->uartdev.RxMonitor(&pRsrc->uartdev.rsrc));
}

static u8* rs485RxFetchFrame(Rs485Rsrc_t *pRsrc, u16* len){
	return (pRsrc->uartdev.RxFetchFrame(&pRsrc->uartdev.rsrc, len));
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
