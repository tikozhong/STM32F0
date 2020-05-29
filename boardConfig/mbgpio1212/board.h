/**********************************************************
filename: board.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BOARD_H
#define _BOARD_H
#include "misc.h"

#include "uartdev.h"
//#include "ht2201.h"
#include "input.h"
#include "output.h"
#include "rs485dev.h"

//#include "at24c256.h"
//#include "dictionary.h"
//#include "PCA9539.h"
//#include "MCP3421.h"

#define DEV_PORT_SUM 6
#define INPUT_LOW		GPIO_PIN_SET
#define INPUT_HIGH	GPIO_PIN_RESET

//board error code
#define BSD_W25Q32_ERR	0
#define BSD_FS_ERR			1

//port config
#define USING_UART 	((u8)1<<0)
#define USING_IIC 	((u8)1<<1)
#define USING_SPI 	((u8)1<<2)

//OS COMPONENTS
//extern SemaphoreHandle_t xSemaphoreIIC;
//extern SemaphoreHandle_t xSemaphoreUART;

//define at main.c
//extern SPI_HandleTypeDef hspi1;
//extern SPI_HandleTypeDef hspi2;
//extern SPI_HandleTypeDef hspi3;

//extern UART_HandleTypeDef huart4;
//extern UART_HandleTypeDef huart5;
//extern UART_HandleTypeDef huart1;
//extern UART_HandleTypeDef huart2;
//extern UART_HandleTypeDef huart3;

//extern osThreadId defaultTaskHandle;

/* output variables for extern function --------------------------------------*/

//extern char endStr[4];
//extern u32 errorCode;

extern const PIN_T RUNNING;
extern const PIN_T OUT[12];
extern const PIN_T IN[12];

extern char dBuf[200];

extern UartDev_t console;
extern INPUT_DEV_T input;
extern OUTPUT_DEV_T output;
extern Rs485Dev_t rs485;

void boardInit(void);
void boardPolling(void);

void print(const char* FORMAT_ORG, ...);
void printS(const char* MSG);

s8 ioWrite(u16 addr, u8 *pDat, u16 nBytes);
s8 ioRead(u16 addr, u8 *pDat, u16 nBytes);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
