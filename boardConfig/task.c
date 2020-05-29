/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : task.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 11/18/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "task.h"

#include "stdarg.h"
#include "string.h"
#include <ctype.h>
#include "stdlib.h"
#include "stdio.h"
#include "board.h"

#include "inputcmd.h"
#include "outputcmd.h"

/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**********************************************
*  PINs Define
**********************************************/

/* Private function prototypes -----------------------------------------------*/
u8 tickRT4,tick4,tick8,tick16,tick32,tick64,tick128;
void taskRT4(void);
static void task0(void);
void task4(void);
void task8(void);
void task16(void);
void task32(void);
void task64(void);
void task128(void);

/**
  * @brief polling task
  * @param none
  * @retval None
  */
void taskPolling(void){
	task0();
	if(tick4>=4)		{task4();tick4 = 0;}
	if(tick8>=8)		{task8();tick8 = 0;}
	if(tick16>=16)	{task16();tick16 = 0;}
	if(tick32>=32)		{task32();tick32 = 0;}
	if(tick64>=64)		{task64();tick64 = 0;}
	if(tick128>=128)	{task128();tick128 = 0;}
}

/**
  * @brief quickly task
  * @param none
  * @retval None
  */
static void task0(void){
	UART_HandleTypeDef* huart;
	if(rs485.rsrc.squ == BIT(1)){
		huart = rs485.rsrc.uartdev.rsrc.huart;
		/* Enable the UART Parity Error interrupt and Data Register Not Empty interrupt */
		SET_BIT(huart->Instance->CR1, USART_CR1_PEIE | USART_CR1_RXNEIE);
		rs485.rsrc.squ = 0;
	}
}

/**
  * @brief real time task
  * @param none
  * @retval None
  */
void taskRT4(void){
}

/**
  * @brief tack per 4ms
  * @param none
  * @retval None
  */
void task4(void){
	input.Polling(&input.rsrc, 4);
}

/**
  * @brief tack per 8ms
  * @param none
  * @retval None
  */
void task8(void){
	console.RxMonitor(&console.rsrc);
	rs485.RxMonitor(&rs485.rsrc);
}

/**
  * @brief tack per 16ms
  * @param none
  * @retval None
  */
static u8 squ;
static u16 timeout;
static char lastsent[100];
void task16(void)
{
	u16 i,len;
	char lineOut[200],lineIn[100];
	u8 *p;

	switch(squ){
		case 0:	//wait a request
			p = rs485.RxFetchFrame(&rs485.rsrc, &len);
			if(console.RxFetchLine(&console.rsrc, lineIn, 100)){
				for(i=0;i<100;i++){
					if(lineIn[i] == 0)	break;
					if(lineIn[i] == '(' || lineIn[i] == ')' || lineIn[i] == ',')	lineIn[i] = ' ';
					if(lineIn[i] >= 'A' && lineIn[i] <= 'Z')	lineIn[i] += 32;
				}
				memset(lineOut,0,200);
				if(inputCmd(&input, lineOut, 200, lineIn)){	printS(lineOut);	}
				else if(outputCmd(&output, lineOut, 200, lineIn)){	printS(lineOut);	}
				else{	//485 sent
//					printS(lineIn);
					i = strlen(lineIn);
					rs485.TxSend(&rs485.rsrc, (u8*)lineIn, i);
					memset(lastsent,0,100);
					memcpy(lastsent, lineIn, i);
					squ = 1;
					timeout = 0;
				}
			}
			else if(p){
				memset(lineIn,0,100);
				memcpy(lineIn,p,len);
				free(p);
//				print("rcv:%s",lineIn);
				memset(lineOut,0,200);
				if(inputCmd(&input, lineOut, 200, lineIn)){	rs485.TxSend(&rs485.rsrc, (u8*)lineOut, strlen(lineOut));	}
				else if(outputCmd(&output, lineOut, 200, lineIn)){	rs485.TxSend(&rs485.rsrc, (u8*)lineOut, strlen(lineOut));	}
				else if(strncmp(lineIn,"0123456789\r\n", strlen("0123456789\r\n"))==0){
					strcpy((char*)lineOut, "+ok@0123456789\r\n");
					rs485.TxSend(&rs485.rsrc, (u8*)lineOut, strlen(lineOut));
				}
			}
			break;
		case 1:	//wait rs485 response
			timeout += 16;
			p = rs485.RxFetchFrame(&rs485.rsrc, &len);
			if(p){
				printS((char*)p);
				free(p);
				squ = 0;
			}
			else if(timeout > 200){
				print("+timeout@%s", lastsent);
				squ = 0;
			}
			break;
		
		default:
			squ = 0;
			break;
	}
}

/**
  * @brief tack per 32ms
  * @param none
  * @retval None
  */
void task32(void){
}

/**
  * @brief tack per 64ms
  * @param none
  * @retval None
  */
void task64(void)
{
}

/**
  * @brief tack per 128ms
  * @param none
  * @retval None
  */
void task128(void){
	HAL_GPIO_TogglePin(RUNNING.GPIOx, RUNNING.GPIO_Pin);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
