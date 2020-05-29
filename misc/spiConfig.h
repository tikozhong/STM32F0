/**
  ******************************************************************************
  * File Name          : gpioConfigOp.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_CONFIG_H
#define _SPI_CONFIG_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
	 
typedef enum {
	MSB = 0,
	LSB = 1
} SPI_FirstBit_t;	
	 
s8 spiConfig(
	SPI_HandleTypeDef hspi, 
	u32 firstBit, 
	uint32_t CLKPolarity,
	uint32_t CLKPhase,
	uint32_t BaudRatePrescaler
	);

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
