/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : output_os.c
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : 
*                      
********************************************************************************
* History:
* 09/04/2017: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "output_os.h"
#include "stdarg.h"
#include "outputCmd.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	TASK_BONDING_T* p;	
	char devName[DEV_NAME_LEN];
	PCA9539_Dev_T* PCA9539[2];
	u32 intiStatus;
}outputTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const char OUTPUT_T[] = {"output_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskOutput(void * argument);
static s8 createTaskOutput(osPriority priority, ...);
outputTaskArgument_t taskArgOutput = {0};
TASK_BASE_T TaskInfoOutput = {
	OUTPUT_T,						//	const u8* DEV_TYPE;
	OUTPUT_HELP,				//	const u8* HELP;
	createTaskOutput,		//	s8 (*CreateTask)	(u8 argc, ...);
	NULL,
	0
};
OUTPUT_DEV_T OUTPUT = {0};
/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTaskOutput(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
//	OUTPUT_DEV_T OUTPUT = {0};
//	char str[32]={0};
	TASK_BONDING_T* p = NULL;
	outputTaskArgument_t* arg = (outputTaskArgument_t*)argument;
	
	outputDevSetup(&OUTPUT, arg->PCA9539[0], arg->PCA9539[1], arg->intiStatus);	
	p = arg->p;
	p->base->count++;	
	//new local mail component
	osMailQDef(outputTaskMail, 1, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(outputTaskMail), NULL);
	}
	devInfo_RegName(p, (char*)OUTPUT.rsrc.name, arg->devName);
	print("start %s...ok\n", arg->devName);

	for(;;){
		evt = osMailGet(p->MailId, 0);    // wait for mail
		xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				outputCmd(&OUTPUT, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@OUTPUT osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
		}
		OUTPUT.Polling(&OUTPUT.rsrc);
		xSemaphoreGive(xSemaphoreIIC);
		osDelay(OUTPUT_POLLING_TIME);
	}
}

//this function is run outside this task
static s8 createTaskOutput(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgOutput, 0, sizeof(outputTaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoOutput;
			taskArgOutput.p = &devInfo[i];
			break;
		}
	}
	if(taskArgOutput.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgOutput.devName, pStr);
	taskArgOutput.PCA9539[0] = va_arg(ap, PCA9539_Dev_T*);
	taskArgOutput.PCA9539[1] = va_arg(ap, PCA9539_Dev_T*);
	taskArgOutput.intiStatus = va_arg(ap, u32);
	va_end(ap);	

	xTaskCreate( StartTaskOutput, "output", 100, &taskArgOutput, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgOutput.p->TaskID);		//stack size in 4bytes	
	while(taskArgOutput.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
