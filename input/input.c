#include "input.h"
#include "string.h"

/**********************************************************
 private function
**********************************************************/
static void inputRename(INPUT_RSRC_T* pRsrc, const char* NAME);
static void inputPolling(INPUT_RSRC_T* pRsrc, u8 tick);
static s8 inputReadPin(INPUT_RSRC_T* pRsrc, u8 pin);
/**********************************************************
 public function
**********************************************************/
void InputDevSetup(
	INPUT_DEV_T *pDev, 
	const PIN_T *gpio, u8 gpioLen,
	s8 (*ioWrite)(u16 addr, u8 *pDat, u16 nBytes),
	s8 (*ioRead)(u16 addr, u8 *pDat, u16 nBytes),
	u16 eepromBase
){
	u8 name[DEV_NAME_LEN+1] = {0};
	u8 i,checkcode;	
	INPUT_RSRC_T *pRsrc = &pDev->rsrc;
	
	memset(pRsrc, 0, sizeof(INPUT_RSRC_T));
	pRsrc->PIN = gpio;
	pRsrc->gpioLen = gpioLen;
	pRsrc->status[0] = 0xffffffff;
	pRsrc->status[1] = 0xffffffff;
	pRsrc->ioRead = ioRead;
	pRsrc->ioWrite = ioWrite;
	pRsrc->eepromBase = eepromBase;

	//read devname from eeprom
	pRsrc->ioRead(pRsrc->eepromBase, name, DEV_NAME_LEN+1);
	checkcode = 0xca;
	for(i=0;i<DEV_NAME_LEN;i++)	checkcode ^= name[i];
	if(checkcode == name[DEV_NAME_LEN]){	memcpy(pRsrc->name, name, DEV_NAME_LEN);	}
	else{		inputRename(pRsrc, "input");	}

	pDev->Rename = inputRename;
	pDev->Polling = inputPolling;
	pDev->ReadPin = inputReadPin;
}

/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void inputPolling(INPUT_RSRC_T* pRsrc, u8 tick){
	u8 i;
	pRsrc->tick += tick;
	if(pRsrc->tick < 20)	return;
	pRsrc->tick = 0;
	pRsrc->status[1] = pRsrc->status[0];
	pRsrc->status[0] = 0xffffffff;
	for(i=0;i<pRsrc->gpioLen;i++){
		if(HAL_GPIO_ReadPin(pRsrc->PIN[i].GPIOx, pRsrc->PIN[i].GPIO_Pin) == GPIO_PIN_RESET)
			pRsrc->status[0] &= (0xffffffff^BIT(i));
		if((pRsrc->status[1]&BIT(i))>0 && (pRsrc->status[0]&BIT(i))==0){
			if((pRsrc->enableFalling & BIT(i)) && pRsrc->fallingCallback!=NULL)	pRsrc->fallingCallback(i);	}
		else if((pRsrc->status[1]&BIT(i))==0 && (pRsrc->status[0]&BIT(i))>0){
			if((pRsrc->enableRaising & BIT(i)) && pRsrc->raisingCallback!=NULL)	pRsrc->raisingCallback(i);	}
	}
}

/*******************************************************************************
* Function Name  : InputReadPin
* Description    : all sample 
* Input          : None
* Output         : 0: low(turn on), 1:high	2:shaking
* Return         : None
*******************************************************************************/
static s8 inputReadPin(INPUT_RSRC_T* pRsrc, u8 pin){
	if(pin>=pRsrc->gpioLen)	return 1;
	if(pRsrc->status[0] & BIT(pin))	return 1;
	else return 0;
}

static void inputRename(INPUT_RSRC_T* pRsrc, const char* NAME){
	u8 name[DEV_NAME_LEN+1],i;
	devRename(pRsrc->name, NAME);
	memcpy(name, pRsrc->name, DEV_NAME_LEN);
	name[DEV_NAME_LEN] = 0xca;
	for(i=0;i<DEV_NAME_LEN;i++)	name[DEV_NAME_LEN] ^= name[i];
	pRsrc->ioWrite(pRsrc->eepromBase, name, DEV_NAME_LEN+1);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

