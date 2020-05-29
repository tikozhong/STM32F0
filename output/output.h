/**********************************************************
filename: output.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OUTPUT_H
#define _OUTPUT_H

#include "misc.h"

#define OUTPUT_POLLING_TIME 16

typedef enum{
	PIN_RESET = 0,
	PIN_SET = 1
}OUTPUT_STATUS;

typedef struct {
	char name[DEV_NAME_LEN];
	const PIN_T* gpio;
	u8 gpioLen;
	u16 status;
	s8 (*ioWrite)(u16 addr, u8 *pDat, u16 nBytes);
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes);
	u16 eepromBase;
} OUTPUT_RSRC_T;

typedef struct {	
	OUTPUT_RSRC_T rsrc;
	void (*Rename)(OUTPUT_RSRC_T* pRsrc, const char* NAME);
	void (*WritePinHEX)(OUTPUT_RSRC_T* pRsrc, u16 pinStatus);
	void (*WritePin)(OUTPUT_RSRC_T* pRsrc, u8 pinIndx, OUTPUT_STATUS level);
	void (*TogglePin)(OUTPUT_RSRC_T* pRsrc, u8 pinIndx);
//	s8 (*AutoTogglePin) (OUTPUT_RSRC_T* pRsrc, u8 pinIndx, u16 mSec);
//	s8 (*StopTogglePin) (OUTPUT_RSRC_T* pRsrc, u8 pinIndx, u16 status);
//	void (*Polling)(OUTPUT_RSRC_T *pRsrc);
}OUTPUT_DEV_T;

/* output variables for extern function --------------------------------------*/
DEV_STATUS outputDevSetup(
	OUTPUT_DEV_T *pDev, 
	const PIN_T* gpio,
	u8 gpioLen,
	u16 initStatus,
	s8 (*ioWrite)(u16 addr, u8 *pDat, u16 nBytes),
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes),
	u16 eepromBase
);
#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
