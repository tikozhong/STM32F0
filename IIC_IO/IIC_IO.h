/**********************************************************
filename: IIC_IO.h
**********************************************************/
#ifndef _IIC_IO_H_
#define _IIC_IO_H_

#include "misc.h"

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	/* config	*/
	PIN_T SCL;
	PIN_T SDA;
	u8 isBusy;
	void (*OsDelay)	(u8 ms);
}IIC_IO_Rsrc_T;

typedef struct{
	IIC_IO_Rsrc_T rsrc;
	//resource mutex
//	void	(*TakenThis)		(IIC_IO_Rsrc_T* pRsrc);
//	void	(*FreeThis)			(IIC_IO_Rsrc_T* pRsrc);
	//op
	void (*Start)		(IIC_IO_Rsrc_T* pRsrc);
	void (*Restart)	(IIC_IO_Rsrc_T* pRsrc);
	void (*Stop)		(IIC_IO_Rsrc_T* pRsrc);
	u8 (*ReadByte)	(IIC_IO_Rsrc_T* pRsrc);
	void (*WriteAck)	(IIC_IO_Rsrc_T* pRsrc);
	void (*WriteNoAck)(IIC_IO_Rsrc_T* pRsrc);
	void (*WriteByte)	(IIC_IO_Rsrc_T* pRsrc, u8 dat);
	u8 (*WaitAck)		(IIC_IO_Rsrc_T* pRsrc);
}IIC_IO_Dev_T;

void IIC_IO_Setup(IIC_IO_Dev_T *pDev, const PIN_T scl, const PIN_T sda);


#endif
