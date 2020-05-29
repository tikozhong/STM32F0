/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : board.c
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
#include "main.h"

#include "stdarg.h"
#include "string.h"
#include "stdio.h"

#include "task.h"
//#include "uartdev.h"
#include "ht2201.h"
#include "input.h"
#include "output.h"
//#include "dictionary.h"
#include "rs485dev.h"

char dBuf[200];

#define NOUSED_PIN_INDX 255

/* Public variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char ABOUT[] = {
	"MBGPIO1212-SRC 0.1 \r\n"
};

static const char HELP[] = {
	"\nCommands:"
     "\n help"
     "\n about"
	"\r\n"
};

/**********************************************
*  PINs Define
**********************************************/
const PIN_T NULL_PIN = {NULL, NULL};
const PIN_T RUNNING = {RUNNING_GPIO_Port, RUNNING_Pin};

const PIN_T OUT[12] = {
	{OUT00_GPIO_Port, OUT00_Pin},
	{OUT01_GPIO_Port, OUT01_Pin},
	{OUT02_GPIO_Port, OUT02_Pin},
	{SWDIO_OUT03_GPIO_Port, SWDIO_OUT03_Pin},
	{OUT04_GPIO_Port, OUT04_Pin},
	{OUT05_GPIO_Port, OUT05_Pin},
	{SWCLK_OUT06_GPIO_Port, SWCLK_OUT06_Pin},
	{OUT07_GPIO_Port, OUT07_Pin},
	{OUT08_GPIO_Port, OUT08_Pin},
	{OUT09_GPIO_Port, OUT09_Pin},
	{OUT10_GPIO_Port, OUT10_Pin},
	{OUT11_GPIO_Port, OUT11_Pin}
};

const PIN_T IN[12] = {
	{IN00_GPIO_Port, IN00_Pin},
	{IN01_GPIO_Port, IN01_Pin},
	{IN02_GPIO_Port, IN02_Pin},
	{IN03_GPIO_Port, IN03_Pin},
	{IN04_GPIO_Port, IN04_Pin},
	{IN05_GPIO_Port, IN05_Pin},
	{IN06_GPIO_Port, IN06_Pin},
	{IN07_GPIO_Port, IN07_Pin},
	{IN08_GPIO_Port, IN08_Pin},
	{IN09_GPIO_Port, IN09_Pin},
	{IN10_GPIO_Port, IN10_Pin},
	{IN11_GPIO_Port, IN11_Pin}
};

//EEPROM
const PIN_T SCL = {SCL_GPIO_Port, SCL_Pin};
const PIN_T SDA = {SDA_GPIO_Port, SDA_Pin};

const PIN_T DE = {DE_GPIO_Port, DE_Pin};
const PIN_T DET = {DET_GPIO_Port, DET_Pin};

u32 errorCode = 0;

UartDev_t console;
Rs485Dev_t rs485;
HT2201Dev_t erom;
//static AT24C256C_Dev_T erom;
//static DictDev_T dict;
INPUT_DEV_T input;
OUTPUT_DEV_T output;

#define RX_POOL_LEN	(1000)	
#define	RX_BUF_LEN	(100)
#define	TX_BUF_LEN	(1000)
static u8 uartMem[RX_POOL_LEN+2*RX_BUF_LEN+TX_BUF_LEN];
static u8 rs485Mem[RX_POOL_LEN+2*RX_BUF_LEN+TX_BUF_LEN];
UartDev_t console;

/* Private function prototypes -----------------------------------------------*/
void print(const char* FORMAT_ORG, ...);
//static void GetMatches ( const char * str, const char * format, ... );
void printS(const char* X);

s8 ioWrite(u16 addr, u8 *pDat, u16 nBytes);
s8 ioRead(u16 addr, u8 *pDat, u16 nBytes);

//TokenDev_t token;
void boardInit(void){
	HAL_Delay(1000);
	setupUartDev(&console, &huart1, uartMem, RX_POOL_LEN, RX_BUF_LEN, TX_BUF_LEN);
	printS(ABOUT);
	printS(HELP);

	printS("setup rs485...");
	setupRs485Dev(&rs485, &huart2, rs485Mem, RX_POOL_LEN, RX_BUF_LEN, TX_BUF_LEN, DE, DET);
	printS("ok\n");	
	
  printS("setup eeprom...");
	ht2201DevSetup(&erom,SCL,SDA);
//  AT24C256C_Setup1(
//      &erom,  //eeprom device
//      0x00,   //A0..A1 addr
//      CAPACITY_AT24C256C,   //容量
//      PAGESZ_AT24256C,      //写页面大小
//      SCL, SDA,             //管脚配置
//      NULL                  //外部延时, 写延时会用到
//  );
  printS("ok\n");

	printS("setup input..");
	InputDevSetup(&input, IN, 12, ioWrite, ioRead, 0);
	print("%s..ok\n", input.rsrc.name);

	printS("setup output...");
	outputDevSetup(&output, OUT, 12, 0x0000, ioWrite, ioRead, 20);
	print("%s..ok\n", output.rsrc.name);

//  printS("setup dictionary...");
//  DictSetup(
//      &dict,    //字典设备
//      128,      //在EEPROM中注册字典的初始地址
//      CAPACITY_AT24C256C,   //在EEPROM中注册字典的容量
//      0x00efffff, //写次数
//      ioWrite, 
//      ioRead   
//  );
//  printS("ok\n");

}

void printS(const char* STRING){
	console.TxSend(&console.rsrc, (const u8*)STRING, strlen(STRING));
}

void print(const char* FORMAT_ORG, ...){
	va_list ap;
	s16 bytes;
	char buf[200] = {0};
	//take string
	memset(buf, 0, 200);
	va_start(ap, FORMAT_ORG);
	bytes = vsnprintf(buf, 200, FORMAT_ORG, ap);
	va_end(ap);
	//send out
	if(bytes>0)	console.TxSend(&console.rsrc, (u8*)buf, bytes);
}

s8 ioWrite(u16 addr, u8 *pDat, u16 nBytes){
	erom.Write(&erom.rsrc, addr, pDat, nBytes);
	return 0;
}

s8 ioRead(u16 addr, u8 *pDat, u16 nBytes){
  erom.Read(&erom.rsrc, addr, pDat, nBytes);
	return 0;
}

//static s8 eepromIOWrite(u16 addr, const u8 *pDat, u16 nBytes){
//	EEPROM.Write(&EEPROM.rsrc, addr, pDat, nBytes);
//	return 0;
//}

//static s8 eepromIORead(u16 addr, u8 *pDat, u16 nBytes){
//	EEPROM.Read(&EEPROM.rsrc, addr, pDat, nBytes);
//	return 0;
//}

//void dictReadNum(const char* TAG, u32* val){
//	u32 tmp;
//	char str[20] = {0};
//	dict.ReadStr(&dict.rsrc, TAG, str, 20, &tmp);
//	CONVERT.StrToUint32(str, val);
//}

//void dictWriteNum(const char* TAG, u32 val){
//	char str[20] = {0};
//	CONVERT.Uint32ToDecStrN(str,val,0);
//	dict.WriteStr(&dict.rsrc, TAG, str, strlen(str));
//}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle){}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == huart2.Instance){
		HAL_GPIO_WritePin(DE.GPIOx, DE.GPIO_Pin, GPIO_PIN_RESET);
		rs485.rsrc.squ <<= 1;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
