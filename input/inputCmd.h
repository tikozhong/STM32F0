/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.h
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _INPUT_CMD_H
#define _INPUT_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "input.h"

/* Exported types ------------------------------------------------------------*/
typedef enum{
	INPUT_EVENT_FALLING	=	0,
	INPUT_EVENT_RAISING	=	1,
}INPUT_EVENT_T;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern const char INPUT_HELP[];

/* Exported functions ------------------------------------------------------- */
u8 inputCmd(INPUT_DEV_T *dev, char* cmd, u16 len, const char* CMD);

#endif /* _INPUT_CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
