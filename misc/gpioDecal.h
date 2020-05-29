/**
  ******************************************************************************
  * File Name          : gpioConfigOp.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _gpio_decal_H
#define _gpio_decal_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
	 
void as_OUTPUT_PP_NOPULL_LOW		(PIN_T PIN);
void as_OUTPUT_PP_NOPULL_HIGH	(PIN_T PIN);
void as_OUTPUT_PP_PULLUP_LOW		(PIN_T PIN);
void as_OUTPUT_PP_PULLUP_HIGH	(PIN_T PIN);
void as_OUTPUT_PP_PULLDWN_LOW	(PIN_T PIN);
void as_OUTPUT_PP_PULLDWN_HIGH	(PIN_T PIN);
void as_OUTPUT_OD_NOPULL_LOW		(PIN_T PIN);
void as_OUTPUT_OD_NOPULL_HIGH	(PIN_T PIN);
void as_OUTPUT_OD_PULLUP_LOW		(PIN_T PIN);
void as_OUTPUT_OD_PULLUP_HIGH	(PIN_T PIN);
void as_INPUT_NOPULL						(PIN_T PIN);
void as_INPUT_PULLUP						(PIN_T PIN);
void as_INPUT_PULLDWN					(PIN_T PIN);

void write(PIN_T, GPIO_PinState);
void toggle(PIN_T);
GPIO_PinState readPin(PIN_T);
void writeX(PIN_T, GPIO_PinState, ...);
void toggleX(PIN_T, ...);
u32 readPinX(PIN_T, ...);	

typedef struct{
	//CONFIG
	void (*As_OUTPUT_PP_NOPULL_LOW)		(PIN_T);
	void (*As_OUTPUT_PP_NOPULL_HIGH)	(PIN_T);
	void (*As_OUTPUT_PP_PULLUP_LOW)		(PIN_T);
	void (*As_OUTPUT_PP_PULLUP_HIGH)	(PIN_T);
	void (*As_OUTPUT_PP_PULLDWN_LOW)	(PIN_T);
	void (*As_OUTPUT_PP_PULLDWN_HIGH)	(PIN_T);
	void (*As_OUTPUT_OD_NOPULL_LOW)		(PIN_T);
	void (*As_OUTPUT_OD_NOPULL_HIGH)	(PIN_T);
	void (*As_OUTPUT_OD_PULLUP_LOW)		(PIN_T);
	void (*As_OUTPUT_OD_PULLUP_HIGH)	(PIN_T);
	void (*As_INPUT_NOPULL)						(PIN_T);
	void (*As_INPUT_PULLUP)						(PIN_T);
	void (*As_INPUT_PULLDWN)					(PIN_T);
	//OP
	void (*Write)(PIN_T, GPIO_PinState);
	void (*Toggle)(PIN_T);
	GPIO_PinState (*Read)(PIN_T);
	void (*WriteX)(PIN_T, GPIO_PinState, ...);
	void (*ToggleX)(PIN_T, ...);
	u32 (*ReadX)(PIN_T, ...);
} GpioConfigOp_t; 

void SetupGpioConfigOp(GpioConfigOp_t* pDev);

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
