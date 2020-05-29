/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : inputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "inputCmd.h"
#include "string.h"
#include "stdarg.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char INPUT_HELP[] = {
	"input commands:"
	"\n dev.help()"
	"\n dev.rename()"
	"\n dev.readPin()/(indx)/(indx0,indx1)"
	"\n dev.enableFalling(indx)/()"
	"\n dev.disableFalling(indx)/()"
	"\n dev.enableRaising(indx)/()"
	"\n dev.disableRaising(indx)/()"
	"\r\n"
};

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : inputCmd
* Description    : gpio function commands
* Input          : 
									: 
* Output         : None
* Return         : None 

output.writepin(%pin, %state)
output.toggle(%pin)
*******************************************************************************/
u8 inputCmd(INPUT_DEV_T *dev, char* cmd, u16 len, const char* CMD){
	u32 i,j,ii,jj;
	const char* line;
	char name[16] = {0};
	INPUT_RSRC_T *pRsrc = &dev->rsrc;
	
	if(strncmp(CMD, pRsrc->name, strlen(pRsrc->name)) != 0)	return 0;
	
	line = &CMD[strlen(pRsrc->name)];
	if(strncmp(line, ".help  ", strlen(".help  ")) == 0)	strFormat(cmd, len, INPUT_HELP);
	//.rename
	else if(sscanf(line, ".rename %s ", name)==1){
		strFormat(cmd, len, "+ok@%s.rename(\"%s\")\r\n", pRsrc->name, name);
		dev->Rename(&dev->rsrc, name);
	}
	//input.readPin()/(indx)/(indx0,indx1)
	else if(sscanf(line, ".readpin %d %d ", &i, &j)==2){
		if(i>=dev->rsrc.gpioLen || j>=dev->rsrc.gpioLen){
			strFormat(cmd, len, "+err@%s.readpin(\"overflow\")\r\n",pRsrc->name);	
			return 1;	
		}
		ii = 0;	jj = 0;
		if(dev->ReadPin(&dev->rsrc, i))	ii = 1;
		if(dev->ReadPin(&dev->rsrc, j))	jj = 1;
		strFormat(cmd, len, "+ok@%s.readpin(%d,%d,%d,%d)\r\n", pRsrc->name, i, ii, j, jj);
	}
	else if(sscanf(line, ".readpin %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(cmd, len, "+err@%s.readpin(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		ii = 0;	
		if(dev->ReadPin(&dev->rsrc, i))	ii = 1;
		strFormat(cmd, len, "+ok@%s.readpin(%d,%d)\r\n", pRsrc->name, i, ii);
	}
	else if(strncmp(line, ".readpin  ", strlen("readpin  ")) == 0){
		strFormat(cmd, len, "+ok@%s.readpin(0x%x)\r\n", pRsrc->name, dev->rsrc.status[0]);
	}
	//"\n input.enableFalling(indx)/()"
	else if(sscanf(line, ".enablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(cmd, len, "+err@%s.enablefalling(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pRsrc->enableFalling |= BIT(i);
		strFormat(cmd, len, "+ok@%s.enablefalling(0x%x)\r\n", pRsrc->name, pRsrc->enableFalling);
	}
	//"\n input.disableFalling(indx)/()"	
	else if(sscanf(line, ".disablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(cmd, len, "+err@%s.disablefalling(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pRsrc->enableFalling &= 0xffff ^ BIT(i);
		strFormat(cmd, len, "+ok@%s.disablefalling(0x%x)\r\n", pRsrc->name, pRsrc->enableFalling);
	}
	//"\n input.enableRaising(indx)/()"
	else if(sscanf(line, ".enableraising %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(cmd, len, "+err@%s.enableraising(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pRsrc->enableRaising |= BIT(i);
		strFormat(cmd, len, "+ok@%s.enableraising(0x%x)\r\n", pRsrc->name, pRsrc->enableRaising);
	}
	//"\n input.disableRaising(indx)/()"
	else if(sscanf(line, ".disablefalling %d ", &i)==1){
		if(i>=dev->rsrc.gpioLen){	
			strFormat(cmd, len, "+err@%s.disableraising(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pRsrc->enableRaising &= 0xffff ^ BIT(i);
		strFormat(cmd, len, "+ok@%s.disableraising(0x%x)\r\n", pRsrc->name, pRsrc->enableRaising);
	}
	
	else	strFormat(cmd, len, "+err@%s",CMD);
	
	return 1;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
