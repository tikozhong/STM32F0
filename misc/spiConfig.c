/**
  ******************************************************************************
  * File Name          : gpioConfigOp.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "spiConfig.h"

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/* firstBit	:     SPI_FIRSTBIT_LSB	SPI_FIRSTBIT_MSB	                        */
/* CLKPolarity	: SPI_POLARITY_LOW	SPI_POLARITY_HIGH                         */
/* CLKPhase  		: SPI_PHASE_1EDGE   SPI_PHASE_2EDGE                           */
/* BaudRatePrescaler	:  SPI_BAUDRATEPRESCALER_2..256                         */
/*----------------------------------------------------------------------------*/
s8 spiConfig(
	SPI_HandleTypeDef hspi, 
	u32 firstBit, 
	u32 CLKPolarity,
	u32 CLKPhase,
	u32 BaudRatePrescaler
	){
  hspi.Init.Mode = SPI_MODE_MASTER;
  hspi.Init.Direction = SPI_DIRECTION_2LINES;
  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
		
  hspi.Init.FirstBit = firstBit;		
  hspi.Init.BaudRatePrescaler = BaudRatePrescaler;		
  hspi.Init.CLKPolarity = CLKPolarity;
  hspi.Init.CLKPhase = CLKPhase;
		
  hspi.Init.NSS = SPI_NSS_SOFT;
  hspi.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi) != HAL_OK)	return -1;

//  hspi.Instance = SPI1;
//  hspi.Init.Mode = SPI_MODE_MASTER;
//  hspi.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi.Init.NSS = SPI_NSS_SOFT;
//  hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
//  hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi.Init.CRCPolynomial = 10;
//  if (HAL_SPI_Init(&hspi1) != HAL_OK)	return -1;
	return 0;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
