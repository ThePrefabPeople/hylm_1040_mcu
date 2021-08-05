#include "key.h"

unsigned char KeyValue;

static BUTTON_T s_tBtn[BUTTON_COUNT];
static KEY_FIFO_T s_Key;		/* 按键FIFO变量,结构体 */

void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(unsigned char _ucIndex);
//按键按下，持续，弹起，持续
//一连串的键值，50ms滤除抖动，发送键值到缓冲区，键值采集函数相对按键处理函数要慢一些
//这样能保证每次采集到按键都会在处理函数中响应，软件只需筛选所需的按键状态就可以了。
//
/*
*********************************************************************************************************
*	函 数 名: IsKeyDownX
*	功能说明: 判断按键是否按下
*	形    参: 无
*	返 回 值: 返回值1 表示按下，0表示未按下
*********************************************************************************************************
*/
static unsigned char IsKeyDown0(void) {if ((HAL_GPIO_ReadPin(GPIO_X00, PIN_X00)) == 0){return 1;}else return 0;}
static unsigned char IsKeyDown1(void) {if ((HAL_GPIO_ReadPin(GPIO_X01, PIN_X01)) == 0){return 1; }else return 0;}
static unsigned char IsKeyDown2(void) {if ((HAL_GPIO_ReadPin(GPIO_X02, PIN_X02)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown3(void) {if ((HAL_GPIO_ReadPin(GPIO_X03, PIN_X03)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown4(void) {if ((HAL_GPIO_ReadPin(GPIO_X04, PIN_X04)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown5(void) {if ((HAL_GPIO_ReadPin(GPIO_X05, PIN_X05)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown6(void) {if ((HAL_GPIO_ReadPin(GPIO_X06, PIN_X06)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown7(void) {if ((HAL_GPIO_ReadPin(GPIO_X07, PIN_X07)) == 0) return 1; else return 0;}

static unsigned char IsKeyDown8(void) {if ((HAL_GPIO_ReadPin(GPIO_X10, PIN_X10)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown9(void) {if ((HAL_GPIO_ReadPin(GPIO_X11, PIN_X11)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown10(void) {if ((HAL_GPIO_ReadPin(GPIO_X12, PIN_X12)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown11(void) {if ((HAL_GPIO_ReadPin(GPIO_X13, PIN_X13)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown12(void) {if ((HAL_GPIO_ReadPin(GPIO_X14, PIN_X14)) == 0) return 1; else return 0;}

static unsigned char IsKeyDown13(void) {if ((HAL_GPIO_ReadPin(GPIO_X15, PIN_X15)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown14(void) {if ((HAL_GPIO_ReadPin(GPIO_X16, PIN_X16)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown15(void) {if ((HAL_GPIO_ReadPin(GPIO_X17, PIN_X17)) == 0) return 1; else return 0;}
static unsigned char IsKeyDown16(void) {if ((HAL_GPIO_ReadPin(GPIO_X20, PIN_X20)) == 0) return 1; else return 0;}

//static unsigned char IsKeyDown0(void) {if((KeyValue&(1<<0))) {return 1;} else return 0;}
//static unsigned char IsKeyDown1(void) {if((KeyValue&(1<<1))) {return 1;} else return 0;}
//static unsigned char IsKeyDown2(void) {if((KeyValue&(1<<2))) {return 1;} else return 0;}
//static unsigned char IsKeyDown3(void) {if((KeyValue&(1<<3))) {return 1;} else return 0;}
//static unsigned char IsKeyDown4(void) {if((KeyValue&(1<<4))) {return 1;} else return 0;}
//static unsigned char IsKeyDown5(void) {if((KeyValue&(1<<5))) {return 1;} else return 0;}
//static unsigned char IsKeyDown6(void) {if((KeyValue&(1<<6))) {return 1;} else return 0;}
//static unsigned char IsKeyDown7(void) {if((KeyValue&(1<<7))) {return 1;} else return 0;}

//static unsigned char IsKeyDown(unsigned char count)
//{
//	if((KeyValue&(1<<count)))
//	{
//		return 1;
//	}
//	else
//		return 0;	
//}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButton
*	功能说明: 初始化按键
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* 初始化按键变量 */
	bsp_InitButtonHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonHard
*	功能说明: 初始化按键硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	/* 配置按键采样口为输入上拉，无中断 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	INPUT_RCC_CLK_ENABLE();
	
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Pin = PIN_X00|PIN_X01|PIN_X02|PIN_X03|PIN_X04|PIN_X05|PIN_X06|PIN_X07|\
												PIN_X14|PIN_X15|PIN_X16|PIN_X17|PIN_X20	;
	HAL_GPIO_Init(GPIO_X00, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = PIN_X10|PIN_X11|PIN_X12|PIN_X13|PIN_X14;
	HAL_GPIO_Init(GPIO_X10, &GPIO_InitStruct);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键变量
*	形    参：strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButtonVar(void)
{
	unsigned char i;

	/* 对按键FIFO读写指针清零 */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* 给每个按键结构体成员变量赋一组缺省值 */
	for (i = 0; i < BUTTON_COUNT; i++)
	{
		s_tBtn[i].LongTime = BUTTON_LONG_TIME;			/* 长按时间 0 表示不检测长按键事件 */
		s_tBtn[i].Count = BUTTON_FILTER_TIME / 2;		/* 计数器设置为滤波时间的一半 */
		s_tBtn[i].State = 0;							/* 按键缺省状态，0为未按下 */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* 按键按下的键值代码 */
		//s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* 按键弹起的键值代码 */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* 按键被持续按下的键值代码 */
		s_tBtn[i].RepeatSpeed = 0;						/* 按键连发的速度，0表示不支持连发 */
		s_tBtn[i].RepeatCount = 0;						/* 连发计数器 */
		
//		/* 判断按键按下的函数 */
//		s_tBtn[i].IsKeyDownFunc = IsKeyDown;
	}

	/* 如果需要单独更改某个按键的参数，可以在此单独重新赋值 */
	/* 比如，我们希望按键1按下超过1秒后，自动重发相同键值 */
//	s_tBtn[1].LongTime = 100;
//	s_tBtn[1].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */
//	s_tBtn[2].LongTime = 100;
//	s_tBtn[2].RepeatSpeed = 5;	/* 每隔50ms自动发送键值 */    


	/* 判断按键按下的函数 */
	s_tBtn[0].IsKeyDownFunc = IsKeyDown0;
	s_tBtn[1].IsKeyDownFunc = IsKeyDown1;
	s_tBtn[2].IsKeyDownFunc = IsKeyDown2;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown3;
	s_tBtn[4].IsKeyDownFunc = IsKeyDown4;
	s_tBtn[5].IsKeyDownFunc = IsKeyDown5;
	s_tBtn[6].IsKeyDownFunc = IsKeyDown6;
	s_tBtn[7].IsKeyDownFunc = IsKeyDown7;
	
	s_tBtn[8].IsKeyDownFunc = IsKeyDown8;
	s_tBtn[9].IsKeyDownFunc = IsKeyDown9;
	s_tBtn[10].IsKeyDownFunc = IsKeyDown10;
	s_tBtn[11].IsKeyDownFunc = IsKeyDown11;
	s_tBtn[12].IsKeyDownFunc = IsKeyDown12;
	
	s_tBtn[13].IsKeyDownFunc = IsKeyDown13;
	s_tBtn[14].IsKeyDownFunc = IsKeyDown14;
	s_tBtn[15].IsKeyDownFunc = IsKeyDown15;
	s_tBtn[16].IsKeyDownFunc = IsKeyDown16;
}



/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参：_KeyCode : 按键代码
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_PutKey(unsigned char _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
unsigned char bsp_GetKey(void)
{
	unsigned char ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyState
*	功能说明: 读取按键的状态
*	形    参：_ucKeyID : 按键ID，从0开始
*	返 回 值: 1 表示按下， 0 表示未按下
*********************************************************************************************************
*/
unsigned char bsp_KeyState(unsigned char _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

uint32_t GetPinSt(void)
{
	uint32_t input_pin=0;
	uint8_t i;
	for(i=0;i<BUTTON_COUNT;i++)
	{
		if(s_tBtn[i].State==1)
		{
			input_pin |=(1<<i);
		}
	}
	return input_pin;
}


/*
*********************************************************************************************************
*	函 数 名: bsp_DetectButton
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectButton(unsigned char i)
{
	BUTTON_T *pBtn;

	/* 如果没有初始化按键函数，则报错
	if (s_tBtn[i].IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
	}
	*/

	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc())
	{
		if (pBtn->Count < BUTTON_FILTER_TIME)
		{
			pBtn->Count = BUTTON_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * BUTTON_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* 发送按钮按下的消息 */
				bsp_PutKey((unsigned char)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* 键值放入按键FIFO */
						bsp_PutKey((unsigned char)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* 常按键后，每隔10ms发送1个按键 */
							bsp_PutKey((unsigned char)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else
	{
		if(pBtn->Count > BUTTON_FILTER_TIME)
		{
			pBtn->Count = BUTTON_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* 发送按钮弹起的消息 */
				bsp_PutKey((unsigned char)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_KeyPro
*	功能说明: 检测所有按键。非阻塞状态，必须被周期性的调用。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_KeyPro(void)
{
	unsigned char i;


	for (i = 0; i < BUTTON_COUNT; i++)
	{
		bsp_DetectButton(i);
	}
}

/*****************************  (END OF FILE) *********************************/
