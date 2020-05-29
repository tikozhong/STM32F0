/**********************************************************
filename: TM1638.c
**********************************************************/

/*********************************************************/
//#include "board.h"
//#include "publicRsrc.h"
#include "ht2201.h"

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
#define HT2201_CAP	128
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/
/* Private function prototypes --------------------------*/
static void ht2201Write(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len);
static void ht2201Read(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len);
static void ht2201WritePage(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len);

/**********************************************************
 Public function
**********************************************************/
s8 ht2201DevSetup(
	HT2201Dev_t *pDev, 
	PIN_T SCL,
	PIN_T SDA	){
	//resource
	IIC_IO_Setup(&pDev->rsrc.iicdev, SCL, SDA);
	//ops
	pDev->Read = ht2201Read;
	pDev->Write = ht2201Write;
	return 0;
}
	
/**********************************************************
* Function Name  : ht2201Read
* Description    : 
* Input          : None
* Output         : None
* Return         : None
**********************************************************/
static void ht2201Read(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len){
	u8 i;
	IIC_IO_Dev_T* pI2cDev = &pRsrc->iicdev;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;

	//make start
	pI2cDev->Start(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc,0xa0);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return;
	}
	//word addr
	pI2cDev->WriteByte(pI2cRsrc, addr);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return;
	}
	//re-start
	pI2cDev->Restart(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, 0xa1);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return;
	}
	//read data
	for(i=0;i<len;i++){
		pBuf[i] = pI2cDev->ReadByte(pI2cRsrc);
		if(i <len-1)
			pI2cDev->WriteAck(pI2cRsrc);
		else
			pI2cDev->WriteNoAck(pI2cRsrc);
	}
	//stop
	pI2cDev->Stop(pI2cRsrc);
}

/**********************************************************
* Function Name  : ht2201Write
* Description    : 
* Input          : None
* Output         : None
* Return         : None
**********************************************************/
static void ht2201WritePage(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len){
	u8 i,tmp;
	IIC_IO_Dev_T* pI2cDev = &pRsrc->iicdev;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;
	
	if(addr+len > HT2201_CAP || len==0)	return;

	//make start
	pI2cDev->Start(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, 0xa0);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return;
	}
	//word addr
	pI2cDev->WriteByte(pI2cRsrc, addr);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return;
	}
	tmp = (len<8?len:8);
	for(i=0;i<tmp;i++){
		//write data
		pI2cDev->WriteByte(pI2cRsrc,pBuf[i]);
		if(pI2cDev->WaitAck(pI2cRsrc) == 1){
			pI2cDev->Stop(pI2cRsrc);
			return;
		}
	}		
	//stop
	pI2cDev->Stop(pI2cRsrc);
	//wait twr=5ms
	HAL_Delay(5);
}
static void ht2201Write(HT2201Rsrc_T *pRsrc, u8 addr, u8* pBuf, u8 len){
	u8 j,k,firstLen;
	if(addr+len > HT2201_CAP)	return;
	j = len;
	firstLen = addr%8;
	if(firstLen){
		k = (len<(8-firstLen)?len:(8-firstLen));
		ht2201WritePage(pRsrc, addr, &pBuf[0], k);
		j -= k;
	}
	for(;j;){
		if(j <= 8){
			ht2201WritePage(pRsrc, addr+(len-j), &pBuf[len-j], j);
			break;
		}
		else{
			ht2201WritePage(pRsrc, addr+(len-j), &pBuf[len-j], 8);
			j -= 8;
		}
	}
}

/**********************************************************
 == THE END ==
**********************************************************/
