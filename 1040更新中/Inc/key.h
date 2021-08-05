#ifndef __KEY__H_
#define __KEY__H_
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc_ex.h"
#define BUTTON_COUNT	17		/* 2����ͨ���� */

/* ���尴��IO */

#define INPUT_RCC_CLK_ENABLE()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define GPIO_X00	GPIOG
#define PIN_X00 	GPIO_PIN_0		/* �������� key1 ��  */
#define GPIO_X01	GPIOG
#define PIN_X01 	GPIO_PIN_1		/* �������� key2 �� */
#define GPIO_X02	GPIOG
#define PIN_X02 	GPIO_PIN_2		/* �������� key3 ��  */
#define GPIO_X03	GPIOG
#define PIN_X03 	GPIO_PIN_3		/* �������� key4 �� */
#define GPIO_X04	GPIOG
#define PIN_X04 	GPIO_PIN_4		/* �������� key5 ��  */
#define GPIO_X05	GPIOG
#define PIN_X05 	GPIO_PIN_5		/* �������� key6 �� */
#define GPIO_X06	GPIOG
#define PIN_X06 	GPIO_PIN_6		/* �������� key7 ��  */
#define GPIO_X07	GPIOG
#define PIN_X07 	GPIO_PIN_7		/* �������� key8 �� */


#define GPIO_X10	GPIOE
#define PIN_X10 	GPIO_PIN_0		/* �������� key ��  */
#define GPIO_X11	GPIOE
#define PIN_X11 	GPIO_PIN_1		/* �������� key ��  */
#define GPIO_X12	GPIOE
#define PIN_X12 	GPIO_PIN_2		/* �������� key11 ��  */
#define GPIO_X13	GPIOE
#define PIN_X13 	GPIO_PIN_3		/* �������� key12 ��  */
#define GPIO_X14	GPIOE
#define PIN_X14 	GPIO_PIN_4		/* �������� key13 ��  */

#define GPIO_X15	GPIOG
#define PIN_X15 	GPIO_PIN_8		/* �������� key1 ��  */
#define GPIO_X16	GPIOG
#define PIN_X16 	GPIO_PIN_9		/* �������� key2 �� */
#define GPIO_X17	GPIOG
#define PIN_X17 	GPIO_PIN_10		/* �������� key3 ��  */

#define GPIO_X20	GPIOG
#define PIN_X20 	GPIO_PIN_15		/* �������� key4 �� */


/* �����˲�ʱ��50ms, ��λ10ms
 ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
*/
#define BUTTON_FILTER_TIME 	5
#define BUTTON_LONG_TIME 	300		/* ����3�룬��Ϊ�����¼� */

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/


typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	unsigned char (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	unsigned char Count;			/* �˲��������� */
	unsigned int LongCount;		/* ���������� */
	unsigned int LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	unsigned char  State;			/* ������ǰ״̬ */
	unsigned char RepeatSpeed;	/* ������������ */
	unsigned char RepeatCount;	/* �������������� */
}BUTTON_T;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	20
typedef struct
{
	unsigned char Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	unsigned char Read;					/* ��������ָ�� */
	unsigned char Write;					/* ������дָ�� */
}KEY_FIFO_T;


/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,				/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,				/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,				/* 3������ */
	KEY_3_LONG,				/* 3������ */   

	KEY_4_DOWN,				/* 3������ */
	KEY_4_UP,				/* 3������ */
	KEY_4_LONG,				/* 3������ */   

	KEY_5_DOWN,				/* 3������ */
	KEY_5_UP,				/* 3������ */
	KEY_5_LONG,				/* 3������ */   

	KEY_6_DOWN,				/* 3������ */
	KEY_6_UP,				/* 3������ */
	KEY_6_LONG,				/* 3������ */   

	KEY_7_DOWN,				/* 3������ */
	KEY_7_UP,				/* 3������ */
	KEY_7_LONG,				/* 3������ */   
	
	KEY_8_DOWN,				/* 3������ */
	KEY_8_UP,				/* 3������ */
	KEY_8_LONG,				/* 3������ */
	
	KEY_9_DOWN,				/* 3������ *///ʵ�ʰ�����ʼ
	KEY_9_UP,				/* 3������ */
	KEY_9_LONG,				/* 3������ */   
	
	KEY_10_DOWN,			/* 3������ */
	KEY_10_UP,				/* 3������ */
	KEY_10_LONG,			/* 3������ */   
	
	KEY_11_DOWN,			/* 3������ */
	KEY_11_UP,				/* 3������ */
	KEY_11_LONG,			/* 3������ */   
	
	KEY_12_DOWN,			/* 3������ */
	KEY_12_UP,				/* 3������ */
	KEY_12_LONG,			/* 3������ */   
	
	KEY_13_DOWN,			/* 3������ */
	KEY_13_UP,				/* 3������ */
	KEY_13_LONG,			/* 3������ */   
	
	KEY_14_DOWN,			/* 3������ */
	KEY_14_UP,				/* 3������ */
	KEY_14_LONG,			/* 3������ */   
	
	KEY_15_DOWN,			/* 3������ */
	KEY_15_UP,				/* 3������ */
	KEY_15_LONG,			/* 3������ */   
	
	KEY_16_DOWN,			/* 3������ */
	KEY_16_UP,				/* 3������ */
	KEY_16_LONG,			/* 3������ */   
	
	KEY_17_DOWN,			/* 3������ */
	KEY_17_UP,				/* 3������ */
	KEY_17_LONG,			/* 3������ */   

}KEY_ENUM;
/* ����ID */


enum
{
	KID_1 = 0,
	KID_2,
};

///* ����Ӧ�ó����ܶ��尴���� */
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

/* ���ⲿ���õĺ������� */
extern void bsp_InitButton(void);
extern unsigned char bsp_GetKey(void);
extern void bsp_KeyPro(void);
extern unsigned char bsp_KeyState(unsigned char _ucKeyID);
extern uint32_t GetPinSt(void);

#endif
