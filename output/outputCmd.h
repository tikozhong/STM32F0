/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : outputCmd.h
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:e
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OUTPUT_CMD_H
#define _OUTPUT_CMD_H

#include "misc.h"
#include "output.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern const char OUTPUT_HELP[];
//void outputCmdHelp(UART_DEV *huartDev);
//void outputReport(OUTPUT_DEV_T *pDev, const u8* RtpNo);
//u8 outputCmd(PIN_T *gpio, u8 gpioLen, const char* CMD, char* cmd, u8 cmdLen);
u8 outputCmd(OUTPUT_DEV_T* pDev, char* cmd, u16 len, const char* CMD);

#endif /* _OUTPUT_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
