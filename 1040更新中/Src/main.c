#include "main.h"
#include "stm32f4xx_hal.h"
#include "string.h"
#include "key.h"
#include "function.h"
#include "key_pro.h"
#include "motor.h"
#include "usart.h"
#include "sys.h"
#include "bsp_gpio.h"
#include "bsp_usartx.h"
#include "bsp_beep.h"
#include "bsp_led.h"
#include "bsp_STEPMOTOR.h"
#include "bsp_spiflash.h"
#include "bsp_AD5689.h"
#include "bsp_AD7190.h"
#include "monitor.h"
#include "cmd_queue.h"

ts_VAR Var={0};
//1
int main(void)
{
  HAL_Init(); 					//复位所有外设，初始化Flash接口和系统滴答定时器
  SystemClock_Config();	// 配置系统时钟
	UsartInit();					//初始化串口
  LedInit(); 
  BeepInit();
  OutputInit();
	bsp_InitButton();	//输入端口初始化
	MotorInit();			//电机控制初始化
  MX_SPIFlash_Init();
	FlashInfo();			//输出FLASH器件信息
  AD5689_Init();
	ParmInit();			//读取运行参数
	queue_reset();	//串口屏接收环形缓冲区
	Ad7190Init();
	
//中断检测原点	
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	GPIO_InitStruct.Pin = ORIGIN_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIO_X00, &GPIO_InitStruct);  
//	
//  /* 配置中断优先级 */
//  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	
	Var.Motor1.CalibrationPulse	= Var.EParam.Pulse;
	Var.Motor1.InitDelay = POWERON_DELAY;//开机延时
	Var.BSPwork.Stu = SYSINIT;
	
	BEEP_ON;HAL_Delay(200);BEEP_OFF;
	printf("<main>版本号:%s\r\n",(char *)VERSION);
	printf("<main>K1:开始;K2:停止;K3:复位;K4:模拟原点信号;K5:模拟气缸位置信号\r\n");   
	
  while (1)
  {
		Uart1Pro();			//串口1调试参数
		Uart5Pro();			//接收参数，上位机调试
		RunLedPro();		//指示灯运行
		MapIoPro();			//io输出
		GetInputPro();	//输入采集
		FlashWritePro();//保存参数 
		AD7190Pro();		//模拟量采集
		KeyPro();				//按键处理
		WorkPro();			//程序流程控制
		MotorRun();			//电机 
		MonitorPro();		//串口屏
		PowerPro();			//高压电源输出		
		if(0 == Var.Sys.KeyCnt)
		{//按键采集固定10毫秒
			bsp_KeyPro();
			Var.Sys.KeyCnt=1;
		}
		if(0 == Var.Sys.DebugCnt)
		{//1秒定时调试
			//printf("系统状态：%d\r\n",Var.BSPwork.Stu);
			Var.Sys.DebugCnt=10;
		}
  }
}


