/**********************************************************
filename: task.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TASK_H
#define _TASK_H
#include "misc.h"

/* output variables for extern function --------------------------------------*/
extern u8 tickRT4,tick4,tick8,tick16,tick32,tick64,tick128;
extern void taskPolling(void);
extern void taskRT4(void);
extern void task4(void);
extern void task8(void);
extern void task16(void);
extern void task32(void);
extern void task64(void);
extern void task128(void);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
