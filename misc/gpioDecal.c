/**
  ******************************************************************************
  * File Name          : gpioConfigOp.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpioDecal.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void SetupGpioConfigOp(GpioConfigOp_t* p){
	//cfg
	p->As_OUTPUT_PP_NOPULL_LOW		=as_OUTPUT_PP_NOPULL_LOW;		
	p->As_OUTPUT_PP_NOPULL_HIGH		=as_OUTPUT_PP_NOPULL_HIGH;	
	p->As_OUTPUT_PP_PULLUP_LOW		=as_OUTPUT_PP_PULLUP_LOW;		
	p->As_OUTPUT_PP_PULLUP_HIGH		=as_OUTPUT_PP_PULLUP_HIGH;	
	p->As_OUTPUT_PP_PULLDWN_LOW		=as_OUTPUT_PP_PULLDWN_LOW;	
	p->As_OUTPUT_PP_PULLDWN_HIGH	=as_OUTPUT_PP_PULLDWN_HIGH;	
	p->As_OUTPUT_OD_NOPULL_LOW		=as_OUTPUT_OD_NOPULL_LOW;		
	p->As_OUTPUT_OD_NOPULL_HIGH		=as_OUTPUT_OD_NOPULL_HIGH;	
	p->As_OUTPUT_OD_PULLUP_LOW		=as_OUTPUT_OD_PULLUP_LOW;		
	p->As_OUTPUT_OD_PULLUP_HIGH		=as_OUTPUT_OD_PULLUP_HIGH;	
	p->As_INPUT_NOPULL						=as_INPUT_NOPULL;						
	p->As_INPUT_PULLUP						=as_INPUT_PULLUP;						
	p->As_INPUT_PULLDWN						=as_INPUT_PULLDWN;		
	//op
	p->Write = write;
	p->Toggle = toggle;
	p->Read = readPin;
	p->WriteX = writeX;
	p->ToggleX = toggleX;
	p->ReadX = readPinX;
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_NOPULL_LOW	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_NOPULL_HIGH	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_PULLUP_LOW	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_PULLUP_HIGH	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_PULLDWN_LOW	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_PP_PULLDWN_HIGH(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_OD_NOPULL_LOW	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_OD_NOPULL_HIGH	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_OD_PULLUP_LOW	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_OUTPUT_OD_PULLUP_HIGH	(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_INPUT_NOPULL		(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_INPUT_PULLUP		(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void as_INPUT_PULLDWN		(PIN_T PIN){
	GPIO_TypeDef* GPIOx = PIN.GPIOx;
	uint16_t GPIO_Pin = PIN.GPIO_Pin;
  GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void write(PIN_T x, GPIO_PinState status){
	HAL_GPIO_WritePin(x.GPIOx, x.GPIO_Pin, status);
}
void toggle(PIN_T x){
	HAL_GPIO_TogglePin(x.GPIOx, x.GPIO_Pin);
}
GPIO_PinState readPin(PIN_T x){
	return (HAL_GPIO_ReadPin(x.GPIOx, x.GPIO_Pin));
}
void writeX(PIN_T x0, GPIO_PinState status0, ...){
}
void toggleX(PIN_T x0, ...){
}
u32 readPinX(PIN_T x0, ...){
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
