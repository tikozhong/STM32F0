/**********************************************************
filename: misc.h
**********************************************************/
#ifndef _MISC_H_
#define _MISC_H_

#define USING_RTOS	0
#define USING_F0	1

#if USING_RTOS
#include "cmsis_os.h"
#endif

#if USING_F0
#include "stm32f0xx_hal.h"
#endif

#if USING_F1
#include "stm32f1xx_hal.h"
#endif

#define NOP __NOP
#define CALLBACK_MSG_LEN	48
#define DEV_NAME_LEN		16

#define	BIT(n)	((u32)1<<n)
#define	BIT_LEN(n)	(0XFFFFFFFF>>(32-n))

#define MAX_TASK			64
#define MAX_INSTANCE	16
/****************************************************************************
 @ enums
****************************************************************************/
typedef enum{DEV_ERROR = 0, DEV_SUCCESS = !DEV_ERROR}	DEV_STATUS;
typedef enum{FALSE = 0, TRUE = !FALSE}	Bool;
typedef enum{NONE_BLOCKING = 0, BLOCKING,} TRANSFER_BLOCK_T;
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef uint8_t	 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;
typedef int8_t 		s8;
typedef int16_t 	s16;
typedef int32_t 	s32;

typedef struct {
	GPIO_TypeDef* GPIOx;	// Port Register Base addr.
	uint16_t GPIO_Pin;		// Pin
} PIN_T;

typedef struct {
	s32 mul;
	u16 div;
} FLOAT_T;

typedef int32_t Float16B;

typedef struct {
	u32 integer;
	u16 decimal;
} DOUBLE;

typedef struct {
	u8 offset;
	u16 gainMul;
	u16 gainDiv;
} CAL_T;

typedef struct {
	u16 offset;
	u32 gainMul;
	u32 gainDiv;
}CAL32_T;

typedef struct {
	const char *KeyStr;
	const char *ReplaceBy;
}CMD_DEF_T;

extern char endStr[4];

/* delay about 1us*/
void miscDelay(u8 us);
s16 strFormat(char *buf, u16 len, const char* FORMAT_ORG, ...);
void devRename(char* oldName, const char* NEW_NAME);
#endif
