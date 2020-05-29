/**********************************************************
filename: HT2201.h
**********************************************************/
#ifndef _HT2201_H_
#define _HT2201_H_
#include "IIC_IO.h"

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
//	char name[DEV_NAME_LEN];
	IIC_IO_Dev_T iicdev;
}HT2201Rsrc_T;

typedef struct{
	HT2201Rsrc_T rsrc;
	void (*Write)	(HT2201Rsrc_T* pRsrc, u8 addr, u8* pDat, u8 datLen);
	void (*Read)	(HT2201Rsrc_T* pRsrc, u8 addr, u8* pDat, u8 datLen);
}HT2201Dev_t;

s8 ht2201DevSetup(
	HT2201Dev_t *pDev, 
	PIN_T SCL,
	PIN_T SDA
);

#endif
