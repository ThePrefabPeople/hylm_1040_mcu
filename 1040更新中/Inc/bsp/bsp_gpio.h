#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 --------------------------------------------------------------*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;

/* 以下宏定义是YS-F4Pro开发板专门预留4个光耦输出端口 */
#define OUTPUT1_RCC_CLK_ENABLE           __HAL_RCC_GPIOF_CLK_ENABLE
#define OUTPUT1_GPIO_PIN                 GPIO_PIN_12
#define OUTPUT2_GPIO_PIN                 GPIO_PIN_13
#define OUTPUT3_GPIO_PIN                 GPIO_PIN_14
#define OUTPUT4_GPIO_PIN                 GPIO_PIN_15
#define OUTPUT5_GPIO_PIN                 GPIO_PIN_2
#define OUTPUT6_GPIO_PIN                 GPIO_PIN_1
#define OUTPUT18_GPIO_PIN                GPIO_PIN_11
#define OUTPUT1_GPIO                     GPIOF

#define OUTPUT7_RCC_CLK_ENABLE           __HAL_RCC_GPIOI_CLK_ENABLE
#define OUTPUT10_GPIO_PIN                GPIO_PIN_2
#define OUTPUT7_GPIO_PIN                 GPIO_PIN_7
#define OUTPUT7_GPIO                     GPIOI

#define OUTPUT8_RCC_CLK_ENABLE           __HAL_RCC_GPIOH_CLK_ENABLE
#define OUTPUT8_GPIO_PIN                 GPIO_PIN_7
#define OUTPUT8_GPIO                     GPIOH

#define OUTPUT9_RCC_CLK_ENABLE           __HAL_RCC_GPIOC_CLK_ENABLE
#define OUTPUT9_GPIO_PIN                 GPIO_PIN_8
#define OUTPUT13_GPIO_PIN                GPIO_PIN_7
#define OUTPUT14_GPIO_PIN                GPIO_PIN_6
#define OUTPUT9_GPIO                     GPIOC

#define OUTPUT11_RCC_CLK_ENABLE           __HAL_RCC_GPIOB_CLK_ENABLE
#define OUTPUT11_GPIO_PIN                 GPIO_PIN_3
#define OUTPUT11_GPIO                     GPIOB

#define OUTPUT12_RCC_CLK_ENABLE           __HAL_RCC_GPIOA_CLK_ENABLE
#define OUTPUT12_GPIO_PIN                 GPIO_PIN_15
#define OUTPUT12_GPIO                     GPIOA


#define OUTPUT15_RCC_CLK_ENABLE           __HAL_RCC_GPIOD_CLK_ENABLE
#define OUTPUT15_GPIO_PIN                 GPIO_PIN_3
#define OUTPUT16_GPIO_PIN                 GPIO_PIN_7
#define OUTPUT17_GPIO_PIN                 GPIO_PIN_11
#define OUTPUT15_GPIO                     GPIOD

#define OUTPUT1_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT1_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT1_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT1_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT1_TOGGLE                   HAL_GPIO_TogglePin(OUTPUT1_GPIO,OUTPUT1_GPIO_PIN)                // 输出反转

#define OUTPUT2_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT2_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT2_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT2_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT2_TOGGLE                   HAL_GPIO_TogglePin(OUTPUT1_GPIO,OUTPUT2_GPIO_PIN)                // 输出反转

#define OUTPUT3_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT3_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT3_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT3_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT3_TOGGLE                   HAL_GPIO_TogglePin(OUTPUT1_GPIO,OUTPUT3_GPIO_PIN)                // 输出反转

#define OUTPUT4_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT4_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT4_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT4_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT4_TOGGLE                   HAL_GPIO_TogglePin(OUTPUT1_GPIO,OUTPUT4_GPIO_PIN)                // 输出反转

#define OUTPUT5_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT5_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT5_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT5_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT6_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT6_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT6_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT6_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT7_ON                       HAL_GPIO_WritePin(OUTPUT7_GPIO,OUTPUT7_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT7_OFF                      HAL_GPIO_WritePin(OUTPUT7_GPIO,OUTPUT7_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT8_ON                       HAL_GPIO_WritePin(OUTPUT8_GPIO,OUTPUT8_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT8_OFF                      HAL_GPIO_WritePin(OUTPUT8_GPIO,OUTPUT8_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT9_ON                       HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT9_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT9_OFF                      HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT9_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT10_ON                       HAL_GPIO_WritePin(OUTPUT7_GPIO,OUTPUT10_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT10_OFF                      HAL_GPIO_WritePin(OUTPUT7_GPIO,OUTPUT10_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平

//#define OUTPUT11_OFF                      HAL_GPIO_WritePin(OUTPUT11_GPIO,OUTPUT11_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
//#define OUTPUT11_ON                      HAL_GPIO_WritePin(OUTPUT11_GPIO,OUTPUT11_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
//#define OUTPUT12_OFF                       HAL_GPIO_WritePin(OUTPUT12_GPIO,OUTPUT12_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
//#define OUTPUT12_ON                     HAL_GPIO_WritePin(OUTPUT12_GPIO,OUTPUT12_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
//#define OUTPUT13_OFF                       HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT13_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
//#define OUTPUT13_ON                      HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT13_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
//#define OUTPUT14_OFF                       HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT14_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
//#define OUTPUT14_ON                      HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT14_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平

#define OUTPUT11_ON                      HAL_GPIO_WritePin(OUTPUT11_GPIO,OUTPUT11_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT11_OFF                      HAL_GPIO_WritePin(OUTPUT11_GPIO,OUTPUT11_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT12_ON                       HAL_GPIO_WritePin(OUTPUT12_GPIO,OUTPUT12_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT12_OFF                      HAL_GPIO_WritePin(OUTPUT12_GPIO,OUTPUT12_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT13_ON                       HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT13_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT13_OFF                      HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT13_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
#define OUTPUT14_ON                       HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT14_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT14_OFF                      HAL_GPIO_WritePin(OUTPUT9_GPIO,OUTPUT14_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平

//MOTOR1 DIR
#define OUTPUT15_ON                       HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT15_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT15_OFF                      HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT15_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
//MOTOR1 EN
#define OUTPUT16_ON                       HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT16_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT16_OFF                      HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT16_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平

//MOTOR2 DIR
#define OUTPUT17_ON                       HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT17_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT17_OFF                      HAL_GPIO_WritePin(OUTPUT15_GPIO,OUTPUT17_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平
//MOTOR1 EN
#define OUTPUT18_ON                       HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT18_GPIO_PIN,GPIO_PIN_SET)    // 输出高电平
#define OUTPUT18_OFF                      HAL_GPIO_WritePin(OUTPUT1_GPIO,OUTPUT18_GPIO_PIN,GPIO_PIN_RESET)  // 输出低电平



/* 以下宏定义是YS-F4Pro开发板专门拉出来的12个引脚加上光耦作为输入端引脚 */
#define INPUT1_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT1_GPIO_PIN                 GPIO_PIN_0
#define INPUT1_GPIO                     GPIOG
#define INPUT1_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT2_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT2_GPIO_PIN                 GPIO_PIN_1
#define INPUT2_GPIO                     GPIOG
#define INPUT2_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT3_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT3_GPIO_PIN                 GPIO_PIN_2
#define INPUT3_GPIO                     GPIOG
#define INPUT3_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT4_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT4_GPIO_PIN                 GPIO_PIN_3
#define INPUT4_GPIO                     GPIOG
#define INPUT4_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT5_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT5_GPIO_PIN                 GPIO_PIN_4
#define INPUT5_GPIO                     GPIOG
#define INPUT5_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT6_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT6_GPIO_PIN                 GPIO_PIN_5
#define INPUT6_GPIO                     GPIOG
#define INPUT6_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT7_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT7_GPIO_PIN                 GPIO_PIN_6
#define INPUT7_GPIO                     GPIOG
#define INPUT7_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT8_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
#define INPUT8_GPIO_PIN                 GPIO_PIN_7
#define INPUT8_GPIO                     GPIOG
#define INPUT8_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

//#define INPUT_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
//#define INPUT_GPIO_PIN                 GPIO_PIN_8
//#define INPUT_GPIO                     GPIOG
//#define INPUT_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */
//
//#define INPUT_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
//#define INPUT_GPIO_PIN                 GPIO_PIN_9
//#define INPUT_GPIO                     GPIOG
//#define INPUT_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

//#define INPUT_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
//#define INPUT_GPIO_PIN                 GPIO_PIN_10
//#define INPUT_GPIO                     GPIOG
//#define INPUT_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

//#define INPUT_RCC_CLK_ENABLE           __HAL_RCC_GPIOG_CLK_ENABLE
//#define INPUT_GPIO_PIN                 GPIO_PIN_15
//#define INPUT_GPIO                     GPIOG
//#define INPUT_DOWN_LEVEL               1  /* 根据原理图设计，INPUT引脚为高电平时表示检测到输入，所以这里设置为1 */

#define INPUT1_STATE	(HAL_GPIO_ReadPin(INPUT1_GPIO,INPUT1_GPIO_PIN))
#define INPUT2_STATE	(HAL_GPIO_ReadPin(INPUT2_GPIO,INPUT2_GPIO_PIN))
#define INPUT3_STATE	(HAL_GPIO_ReadPin(INPUT3_GPIO,INPUT3_GPIO_PIN))
#define INPUT4_STATE	(HAL_GPIO_ReadPin(INPUT4_GPIO,INPUT4_GPIO_PIN))
#define INPUT5_STATE	(HAL_GPIO_ReadPin(INPUT5_GPIO,INPUT5_GPIO_PIN))
#define INPUT6_STATE	(HAL_GPIO_ReadPin(INPUT6_GPIO,INPUT6_GPIO_PIN))
#define INPUT7_STATE	(HAL_GPIO_ReadPin(INPUT7_GPIO,INPUT7_GPIO_PIN))
#define INPUT8_STATE	(HAL_GPIO_ReadPin(INPUT8_GPIO,INPUT8_GPIO_PIN))

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void OutputInit(void);
void INPUT_GPIO_Init(void);
void MapIoPro(void);
void GetInputPro(void);

#endif  // __BSP_GPIO_H__

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
