/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : outputCmd.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "outputCmd.h"
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
const char OUTPUT_HELP[] = {
	"output commands:"
	"\n dev.help()"
	"\n dev.rename()"
	"\n dev.readPin()/(indx)/(indx0,indx1)"
	"\n dev.writePin(status)/(indx, b)/(indx0,bit0,indx1,bit1)"
	"\n dev.togglePin()/(indx)/(indx0,indx1)"
};

/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : outputCmd
* Description    : gpio function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 

output.writepin(%pin, %state)
output.toggle(%pin)
*******************************************************************************/
u8 outputCmd(OUTPUT_DEV_T* pDev, char* cmd, u16 len, const char* CMD){
	u32 i,j,ii,jj;
	const char* line;
	char name[16] = {0};
	OUTPUT_RSRC_T *pRsrc = &pDev->rsrc;
		
	if(strncmp(CMD, pRsrc->name, strlen(pRsrc->name)) != 0)	return 0;
	
	line = &CMD[strlen(pRsrc->name)];
	
	if(strncmp(line, ".help  ", strlen(".help  ")) == 0)	strFormat(cmd, len, OUTPUT_HELP);

	//.rename
	else if(sscanf(line, ".rename %s ", name)==1){
		strFormat(cmd, len, "+ok@%s.rename(\"%s\")\r\n", pRsrc->name, name);
		pDev->Rename(&pDev->rsrc, name);
	}
	//output.readPin()/(indx)/(indx0,indx1)
	else if(sscanf(line, ".readpin %d %d ", &i, &j)==2){
		if(i>=pRsrc->gpioLen || j>=pRsrc->gpioLen){	
			strFormat(cmd, len, "+err@%s.readpin(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		ii = 0;	jj = 0;
		if(pRsrc->status & BIT(i))	ii = 1;
		if(pRsrc->status & BIT(j))	jj = 1;
		strFormat(cmd, len, "+ok@%s.readpin(%d,%d,%d,%d)\r\n", pRsrc->name, i, ii, j, jj);
	}
	else if(sscanf(line, ".readpin %d ", &i)==1){
		if(i>=pRsrc->gpioLen){	strFormat(cmd, len, "+err@%s.readpin(\"overflow\")\r\n", pRsrc->name);	return 1;	}
		ii = 0;	
		if(pRsrc->status & BIT(i))	ii = 1;
		strFormat(cmd, len, "+ok@%s.readpin(%d,%d)\r\n", pRsrc->name, i, ii);
	}
	else if(strncmp(line, ".readpin  ", strlen(".readpin  ")) == 0){
		strFormat(cmd, len, "+ok@%s.readpin(0x%x)\r\n", pRsrc->name, pRsrc->status);
	}
	
	//output.writePin(status)/(indx, b)/(indx0,bit0,indx1,bit1)
	else if(sscanf(line, ".writepin %d %d %d %d ", &i, &j, &ii, &jj)==4){
		if(i>=pRsrc->gpioLen || ii>=pRsrc->gpioLen){	
			strFormat(cmd, len, "+err@%s.writepin(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		if(j)		pDev->WritePin(pRsrc, i, PIN_SET);
		else pDev->WritePin(pRsrc, i, PIN_RESET);
		if(jj)	pDev->WritePin(pRsrc, ii, PIN_SET);
		else pDev->WritePin(pRsrc, ii, PIN_RESET);
		strFormat(cmd, len, "+ok@%s.writepin(%d,%d,%d,%d)\r\n", pRsrc->name, i, j, ii, jj);
	}
	else if(sscanf(line, ".writepin %d %d ", &i, &j)==2){
		if(i>=pRsrc->gpioLen){	
			strFormat(cmd, len, "+err@%s.writepin(\"overflow\")\r\n", pRsrc->name);
			return 1;	
		}
		if(j)		pDev->WritePin(pRsrc, i, PIN_SET);
		else pDev->WritePin(pRsrc, i, PIN_RESET);
		strFormat(cmd, len, "+ok@%s.writepin(%d,%d)\r\n", pRsrc->name, i, j);
	}
	else if(sscanf(line, ".writepin %d ", &i)==1){
		pDev->WritePinHEX(pRsrc, i);
		strFormat(cmd, len, "+ok@%s.writepin(%d)\r\n", pRsrc->name, i);
	}

	//output.togglePin()/(indx)/(indx0,indx1)
	else if(sscanf(line, ".togglepin %d %d ", &i, &j)==2){
		if(i>=pRsrc->gpioLen || j>=pRsrc->gpioLen){	
			strFormat(cmd, len, "+err@%s.readpin(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pDev->TogglePin(pRsrc, i);
		pDev->TogglePin(pRsrc, j);
		strFormat(cmd, len, "+ok@%s.togglepin(%d,%d)\r\n", pRsrc->name, i,j);
	}
	else if(sscanf(line, ".togglepin %d ", &i)==1){
		if(i>=pRsrc->gpioLen){	
			strFormat(cmd, len, "+err@%s.togglepin(\"overflow\")\r\n", pRsrc->name);	
			return 1;	
		}
		pDev->TogglePin(pRsrc, i);
		strFormat(cmd, len, "+ok@%s.togglepin(%d)\r\n", pRsrc->name, i);
	}
	else if(strncmp(line, ".togglepin  ", strlen(".togglepin  ")) == 0){
		for(i=0;i<pRsrc->gpioLen;i++)	pDev->TogglePin(pRsrc, i);
		strFormat(cmd, len, "+ok@%s.togglepin()\r\n", pRsrc->name);
	}
	else	strFormat(cmd, len, "+err@%s",CMD);
	
	return 1;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
