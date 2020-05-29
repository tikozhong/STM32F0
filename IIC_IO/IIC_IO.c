/**********************************************************
filename: IIC_IO.c
**********************************************************/

/************************包含头文件***************************************************/
#include "IIC_IO.h"
#include "gpioDecal.h"

#define TICK_IIC 1
/**********************************************************
 Private function
**********************************************************/
static void	IIC_takenThis		(IIC_IO_Rsrc_T* pRsrc);
static void	IIC_freeThis		(IIC_IO_Rsrc_T* pRsrc);

static void IIC_START(IIC_IO_Rsrc_T* pRsrc);
static void IIC_RESTART(IIC_IO_Rsrc_T* pRsrc);
static void IIC_STOP(IIC_IO_Rsrc_T* pRsrc);
static void IIC_WriteAck(IIC_IO_Rsrc_T* pRsrc);
static void IIC_WriteNoAck(IIC_IO_Rsrc_T* pRsrc);
static u8 IIC_WaitAck(IIC_IO_Rsrc_T* pRsrc);
static void IIC_WriteByte(IIC_IO_Rsrc_T* pRsrc, u8 dat);
static u8 IIC_ReadByte(IIC_IO_Rsrc_T* pRsrc);

static void IIC_delayUs(u8);

/**********************************************************
 Public function
**********************************************************/
void IIC_IO_Setup(IIC_IO_Dev_T *pDev, const PIN_T scl, const PIN_T sda){
	IIC_IO_Rsrc_T *pRsrc = &pDev->rsrc;
	//config1
	pRsrc->SCL = scl;
	pRsrc->SDA = sda;
	pRsrc->isBusy = 1;
	//config2
	as_OUTPUT_OD_NOPULL_HIGH(scl);
	as_OUTPUT_OD_NOPULL_HIGH(sda);
	//registe op
	pDev->Start = IIC_START;
	pDev->Restart = IIC_RESTART;
	pDev->Stop = IIC_STOP;
	pDev->WaitAck = IIC_WaitAck;
	pDev->WriteAck = IIC_WriteAck;
	pDev->WriteNoAck = IIC_WriteNoAck;
	pDev->ReadByte = IIC_ReadByte;
	pDev->WriteByte = IIC_WriteByte;	
	//release resource
	pRsrc->isBusy = 0;
}

static void	IIC_takenThis		(IIC_IO_Rsrc_T* pRsrc){
	while(pRsrc->isBusy == 1){
		if(pRsrc->OsDelay)	pRsrc->OsDelay(1);
	}
	pRsrc->isBusy = 1;
}
static void	IIC_freeThis		(IIC_IO_Rsrc_T* pRsrc){
	pRsrc->isBusy = 0;
}

/**********************************************************
 change io direction
**********************************************************/
static void IIC_START(IIC_IO_Rsrc_T* pRsrc){
	IIC_takenThis(pRsrc);
	IIC_RESTART(pRsrc);
}

static void IIC_RESTART(IIC_IO_Rsrc_T* pRsrc){
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);		
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_RESET);		IIC_delayUs(2);
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);
}

static void IIC_STOP(IIC_IO_Rsrc_T* pRsrc){
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_RESET);	IIC_delayUs(2);
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);		
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);	
	IIC_freeThis(pRsrc);
}

/*
	Before enter, SCL, SDA are both low
*/
static u8 IIC_ReadByte(IIC_IO_Rsrc_T* pRsrc){
	u8 i,rtn=0;

	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);	
	for(i=0;i<8;i++){
		HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);	
		IIC_delayUs(3);
		
		rtn <<= 1;
		if(HAL_GPIO_ReadPin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin) == GPIO_PIN_SET)	rtn |= 0x01;
		
		HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);
		IIC_delayUs(5);
	}
	return rtn;
}

static void IIC_WriteAck(IIC_IO_Rsrc_T* pRsrc){
	//HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);		IIC_delayUs(6);
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);	
}
static void IIC_WriteNoAck(IIC_IO_Rsrc_T* pRsrc){
	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);		
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);		IIC_delayUs(5);
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);	
}

static void IIC_WriteByte(IIC_IO_Rsrc_T* pRsrc, u8 dat){
	u8 i,tmp = dat;
	for(i=0;i<8;i++){
		if(tmp & 0x80)	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);
		else	HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_RESET);	
		
		tmp <<= 1;
		HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);	
		IIC_delayUs(4);
		
		HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);			
		HAL_GPIO_WritePin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin, GPIO_PIN_SET);
		IIC_delayUs(2);
	}
}

//wait for 5ms
static u8 IIC_WaitAck(IIC_IO_Rsrc_T* pRsrc){
	u8 rtn,i;
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_SET);		IIC_delayUs(5);	
	for(i=0;i<=5;i++){
		if(HAL_GPIO_ReadPin(pRsrc->SDA.GPIOx, pRsrc->SDA.GPIO_Pin) == GPIO_PIN_RESET){
			rtn = 0;	break;
		}
		else	rtn = 1;
	}	
	HAL_GPIO_WritePin(pRsrc->SCL.GPIOx, pRsrc->SCL.GPIO_Pin, GPIO_PIN_RESET);		

	return rtn;
}

static void IIC_delayUs(u8 us){
	while(us){
		us--;
	}
}

/**********************************************************
 == THE END ==
**********************************************************/
