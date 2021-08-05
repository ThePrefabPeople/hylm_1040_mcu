#ifndef __KEY__H_
#define __KEY__H_
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc_ex.h"
#define BUTTON_COUNT	17		/* 2个普通按键 */

/* 定义按键IO */

#define INPUT_RCC_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define GPIO_X00	GPIOG
#define PIN_X00 	GPIO_PIN_0		/* 独立按键 key1 键  */
#define GPIO_X01	GPIOG
#define PIN_X01 	GPIO_PIN_1		/* 独立按键 key2 键 */
#define GPIO_X02	GPIOG
#define PIN_X02 	GPIO_PIN_2		/* 独立按键 key3 键  */
#define GPIO_X03	GPIOG
#define PIN_X03 	GPIO_PIN_3		/* 独立按键 key4 键 */
#define GPIO_X04	GPIOG
#define PIN_X04 	GPIO_PIN_4		/* 独立按键 key5 键  */
#define GPIO_X05	GPIOG
#define PIN_X05 	GPIO_PIN_5		/* 独立按键 key6 键 */
#define GPIO_X06	GPIOG
#define PIN_X06 	GPIO_PIN_6		/* 独立按键 key7 键  */
#define GPIO_X07	GPIOG
#define PIN_X07 	GPIO_PIN_7		/* 独立按键 key8 键 */


#define GPIO_X10	GPIOE
#define PIN_X10 	GPIO_PIN_0		/* 独立按键 key 键  */
#define GPIO_X11	GPIOE
#define PIN_X11 	GPIO_PIN_1		/* 独立按键 key 键  */
#define GPIO_X12	GPIOE
#define PIN_X12 	GPIO_PIN_2		/* 独立按键 key11 键  */
#define GPIO_X13	GPIOE
#define PIN_X13 	GPIO_PIN_3		/* 独立按键 key12 键  */
#define GPIO_X14	GPIOE
#define PIN_X14 	GPIO_PIN_4		/* 独立按键 key13 键  */

#define GPIO_X15	GPIOG
#define PIN_X15 	GPIO_PIN_8		/* 独立按键 key1 键  */
#define GPIO_X16	GPIOG
#define PIN_X16 	GPIO_PIN_9		/* 独立按键 key2 键 */
#define GPIO_X17	GPIOG
#define PIN_X17 	GPIO_PIN_10		/* 独立按键 key3 键  */

#define GPIO_X20	GPIOG
#define PIN_X20 	GPIO_PIN_15		/* 独立按键 key4 键 */


/* 按键滤波时间50ms, 单位10ms
 只有连续检测到50ms状态不变才认为有效，包括弹起和按下两种事件
*/
#define BUTTON_FILTER_TIME 	5
#define BUTTON_LONG_TIME 	300		/* 持续3秒，认为长按事件 */

/*
	每个按键对应1个全局的结构体变量。
	其成员变量是实现滤波和多种按键状态所必须的
*/


typedef struct
{
	/* 下面是一个函数指针，指向判断按键手否按下的函数 */
	unsigned char (*IsKeyDownFunc)(void); /* 按键按下的判断函数,1表示按下 */

	unsigned char Count;			/* 滤波器计数器 */
	unsigned int LongCount;		/* 长按计数器 */
	unsigned int LongTime;		/* 按键按下持续时间, 0表示不检测长按 */
	unsigned char  State;			/* 按键当前状态 */
	unsigned char RepeatSpeed;	/* 连续按键周期 */
	unsigned char RepeatCount;	/* 连续按键计数器 */
}BUTTON_T;

/* 按键FIFO用到变量 */
#define KEY_FIFO_SIZE	20
typedef struct
{
	unsigned char Buf[KEY_FIFO_SIZE];		/* 键值缓冲区 */
	unsigned char Read;					/* 缓冲区读指针 */
	unsigned char Write;					/* 缓冲区写指针 */
}KEY_FIFO_T;


/* 定义键值代码
	推荐使用enum, 不用#define，原因：
	(1) 便于新增键值,方便调整顺序，使代码看起来舒服点
	(2) 编译器可帮我们避免键值重复。
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 表示按键事件 */

	KEY_1_DOWN,				/* 1键按下 */
	KEY_1_UP,				/* 1键弹起 */
	KEY_1_LONG,				/* 1键长按 */

	KEY_2_DOWN,				/* 2键按下 */
	KEY_2_UP,				/* 2键弹起 */
	KEY_2_LONG,				/* 2键长按 */

	KEY_3_DOWN,				/* 3键按下 */
	KEY_3_UP,				/* 3键弹起 */
	KEY_3_LONG,				/* 3键长按 */   

	KEY_4_DOWN,				/* 3键按下 */
	KEY_4_UP,				/* 3键弹起 */
	KEY_4_LONG,				/* 3键长按 */   

	KEY_5_DOWN,				/* 3键按下 */
	KEY_5_UP,				/* 3键弹起 */
	KEY_5_LONG,				/* 3键长按 */   

	KEY_6_DOWN,				/* 3键按下 */
	KEY_6_UP,				/* 3键弹起 */
	KEY_6_LONG,				/* 3键长按 */   

	KEY_7_DOWN,				/* 3键按下 */
	KEY_7_UP,				/* 3键弹起 */
	KEY_7_LONG,				/* 3键长按 */   
	
	KEY_8_DOWN,				/* 3键按下 */
	KEY_8_UP,				/* 3键弹起 */
	KEY_8_LONG,				/* 3键长按 */
	
	KEY_9_DOWN,				/* 3键按下 *///实际按键开始
	KEY_9_UP,				/* 3键弹起 */
	KEY_9_LONG,				/* 3键长按 */   
	
	KEY_10_DOWN,			/* 3键按下 */
	KEY_10_UP,				/* 3键弹起 */
	KEY_10_LONG,			/* 3键长按 */   
	
	KEY_11_DOWN,			/* 3键按下 */
	KEY_11_UP,				/* 3键弹起 */
	KEY_11_LONG,			/* 3键长按 */   
	
	KEY_12_DOWN,			/* 3键按下 */
	KEY_12_UP,				/* 3键弹起 */
	KEY_12_LONG,			/* 3键长按 */   
	
	KEY_13_DOWN,			/* 3键按下 */
	KEY_13_UP,				/* 3键弹起 */
	KEY_13_LONG,			/* 3键长按 */   
	
	KEY_14_DOWN,			/* 3键按下 */
	KEY_14_UP,				/* 3键弹起 */
	KEY_14_LONG,			/* 3键长按 */   
	
	KEY_15_DOWN,			/* 3键按下 */
	KEY_15_UP,				/* 3键弹起 */
	KEY_15_LONG,			/* 3键长按 */   
	
	KEY_16_DOWN,			/* 3键按下 */
	KEY_16_UP,				/* 3键弹起 */
	KEY_16_LONG,			/* 3键长按 */   
	
	KEY_17_DOWN,			/* 3键按下 */
	KEY_17_UP,				/* 3键弹起 */
	KEY_17_LONG,			/* 3键长按 */   

}KEY_ENUM;
/* 按键ID */


enum
{
	KID_1 = 0,
	KID_2,
};

///* 根据应用程序功能定义按键宏 */
#define KEY_DOWN_START	    KEY_5_DOWN
#define KEY_UP_START		KEY_5_UP
#define KEY_LONG_START     KEY_5_LONG

#define KEY_DOWN_STOP	    KEY_6_DOWN
#define KEY_UP_STOP		KEY_6_UP
#define KEY_LONG_STOP     KEY_6_LONG

#define KEY_DOWN_RESET		KEY_7_DOWN
#define KEY_UP_RESET		KEY_7_UP
#define KEY_LONG_RESET		KEY_7_LONG


#define KEY_DOWN_K1	    KEY_9_DOWN
#define KEY_UP_K1			KEY_9_UP
#define KEY_LONG_K1	     KEY_9_LONG

#define KEY_DOWN_K2	    KEY_10_DOWN
#define KEY_UP_K2		KEY_10_UP
#define KEY_LONG_K2     KEY_10_LONG

#define KEY_DOWN_K3	    KEY_11_DOWN
#define KEY_UP_K3		KEY_11_UP
#define KEY_LONG_K3     KEY_11_LONG

#define KEY_DOWN_K4		KEY_12_DOWN
#define KEY_UP_K4		KEY_12_UP
#define KEY_LONG_K5		KEY_12_LONG


//#define KEY_DOWN_CYCLE		KEY_4_DOWN
//#define KEY_UP_CYCLE		KEY_4_UP
//#define KEY_LONG_CYCLE		KEY_4_LONG

//#define KEY_DOWN_MODE		KEY_5_DOWN
//#define KEY_UP_MODE		KEY_5_UP
//#define KEY_LONG_MODE		KEY_5_LONG

//#define KEY_DOWN_DCAC		KEY_6_DOWN
//#define KEY_UP_DCAC		KEY_6_UP
//#define KEY_LONG_DCAC		KEY_6_LONG


//#define KEY_DOWN_FADD		KEY_7_DOWN
//#define KEY_UP_FADD		KEY_7_UP
//#define KEY_LONG_FADD		KEY_7_LONG


//#define KEY_DOWN_FSUB		KEY_8_DOWN
//#define KEY_UP_FSUB		KEY_8_UP
//#define KEY_LONG_FSUB		KEY_8_LONG

/* 供外部调用的函数声明 */
extern void bsp_InitButton(void);
extern unsigned char bsp_GetKey(void);
extern void bsp_KeyPro(void);
extern unsigned char bsp_KeyState(unsigned char _ucKeyID);
extern uint32_t GetPinSt(void);

#endif
