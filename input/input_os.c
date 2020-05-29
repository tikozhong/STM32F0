/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : input_os.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 11/18/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "input_os.h"
#include "stdarg.h"
#include "inputCmd.h"

//#include "myString.h"
/* Private define ------------------------------------------------------------*/
//#define CMD_RTN_LEN	128
#define INPUT_POLLING_TIME 16

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	TASK_BONDING_T* p;	
	char devName[DEV_NAME_LEN];
	PCA9539_Dev_T* PCA9539[2];
}inputTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const char INPUT_T[] = {"input_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskInput(void * argument);
static s8 createTaskInput(osPriority priority, ...);
inputTaskArgument_t taskArgInput = {0};
TASK_BASE_T TaskInfoInput = {
	INPUT_T,					//	const u8* DEV_TYPE;
	INPUT_HELP,				//	const u8* HELP;
	createTaskInput,	//	s8 (*CreateTask)	(u8 argc, ...);
	NULL,
	0
};
INPUT_DEV_T INPUT;
/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Input         : None
* Return         : None
*******************************************************************************/
static void StartTaskInput(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	INPUT_DEV_T *pDev = NULL;
	INPUT_RSRC_T* pRsrc;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	TASK_BONDING_T* p = NULL;
	inputTaskArgument_t* arg = (inputTaskArgument_t*)argument;
	u8 i;
	
	InputDevSetup(&INPUT, arg->PCA9539[0], arg->PCA9539[1]);	
	p = arg->p;
	p->base->count++;	
	pDev = &INPUT;
	pRsrc = &pDev->rsrc;
	
	//new local mail component
	osMailQDef(outputTaskMail, 1, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(outputTaskMail), NULL);
	}
	devInfo_RegName(p, (char*)INPUT.rsrc.name, arg->devName);
	print("start %s...ok\n", p->devName);
	
	for(;;){
		evt = osMailGet(p->MailId, 0);    // wait for mail
		xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				inputCmd(&INPUT, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@INPUT osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
		}
		INPUT.Fetch(&INPUT.rsrc);
		//release falling/raising events
		if(INPUT.rsrc.enableFalling){
			for(i=0;i<16;i++){
				if(pRsrc->enableFalling & pRsrc->fallingEvt & (1U<<i)){
					pMailSnd = newMail(p->MailId, UartTaskMailId, 0);   // Allocate memory
					inputMakeEventMsg(&pMailSnd->Content, (char*)pRsrc->name, i, INPUT_EVENT_FALLING);//generate msg
					osMailPut(UartTaskMailId, pMailSnd);  // reply Mail
					osDelay(10);
				}
			}
		}
		if(INPUT.rsrc.enableRaising){
			for(i=0;i<16;i++){
				if(pRsrc->enableRaising & pRsrc->raisingEvt & (1U<<i)){
					pMailSnd = newMail(p->MailId, UartTaskMailId, 0);   // Allocate memory
					inputMakeEventMsg(&pMailSnd->Content, (char*)pRsrc->name, i, INPUT_EVENT_RAISING);//generate msg
					osMailPut(UartTaskMailId, pMailSnd);  // reply Mail
					osDelay(10);
				}
			}
		}
		xSemaphoreGive(xSemaphoreIIC);
		osDelay(INPUT_POLLING_TIME);
	}
}

//this function is run outside this task
static s8 createTaskInput(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgInput, 0, sizeof(inputTaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoInput;
			taskArgInput.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgInput.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgInput.devName, pStr);
	taskArgInput.PCA9539[0] = va_arg(ap, PCA9539_Dev_T*);
	taskArgInput.PCA9539[1] = va_arg(ap, PCA9539_Dev_T*);
	va_end(ap);	

	xTaskCreate( StartTaskInput, "input", 100, &taskArgInput, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgInput.p->TaskID);		//stack size in 4bytes	
	while(taskArgInput.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
