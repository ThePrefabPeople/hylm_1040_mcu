
#include "bsp_gpio.h"
#include "key.h"
#include "bsp_led.h"
void OutputInit(void)
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能(开启)光耦引脚对应IO端口时钟 */  
  OUTPUT1_RCC_CLK_ENABLE();  
	OUTPUT7_RCC_CLK_ENABLE();
	OUTPUT8_RCC_CLK_ENABLE();
	OUTPUT9_RCC_CLK_ENABLE();
	OUTPUT12_RCC_CLK_ENABLE();
	OUTPUT15_RCC_CLK_ENABLE();

  /* 配置OUTPUT GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = OUTPUT1_GPIO_PIN|OUTPUT2_GPIO_PIN|OUTPUT3_GPIO_PIN|OUTPUT4_GPIO_PIN|\
												OUTPUT5_GPIO_PIN|OUTPUT6_GPIO_PIN|OUTPUT18_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT1_GPIO, &GPIO_InitStruct);   
	
  GPIO_InitStruct.Pin = OUTPUT7_GPIO_PIN|OUTPUT10_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT7_GPIO, &GPIO_InitStruct);   
  
  GPIO_InitStruct.Pin = OUTPUT8_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT8_GPIO, &GPIO_InitStruct);   
	
  GPIO_InitStruct.Pin = OUTPUT9_GPIO_PIN|OUTPUT13_GPIO_PIN|OUTPUT14_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT9_GPIO, &GPIO_InitStruct); 


  GPIO_InitStruct.Pin = OUTPUT11_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT11_GPIO, &GPIO_InitStruct);  
	
  GPIO_InitStruct.Pin = OUTPUT12_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT12_GPIO, &GPIO_InitStruct);  
	
  GPIO_InitStruct.Pin = OUTPUT15_GPIO_PIN|OUTPUT16_GPIO_PIN|OUTPUT17_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT15_GPIO, &GPIO_InitStruct);  
	
	
}

void INPUT_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能(开启)INPUT引脚对应IO端口时钟 */  
  INPUT1_RCC_CLK_ENABLE();

  /* 配置INPUT GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = INPUT1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT1_GPIO, &GPIO_InitStruct);   
	
  GPIO_InitStruct.Pin = INPUT2_GPIO_PIN;
  HAL_GPIO_Init(INPUT2_GPIO, &GPIO_InitStruct); 
	
  GPIO_InitStruct.Pin = INPUT3_GPIO_PIN;
  HAL_GPIO_Init(INPUT3_GPIO, &GPIO_InitStruct);   
	
  GPIO_InitStruct.Pin = INPUT4_GPIO_PIN;
  HAL_GPIO_Init(INPUT4_GPIO, &GPIO_InitStruct); 
	
  GPIO_InitStruct.Pin = INPUT5_GPIO_PIN;
  HAL_GPIO_Init(INPUT5_GPIO, &GPIO_InitStruct); 
	
  GPIO_InitStruct.Pin = INPUT6_GPIO_PIN;
  HAL_GPIO_Init(INPUT6_GPIO, &GPIO_InitStruct); 
	
  GPIO_InitStruct.Pin = INPUT7_GPIO_PIN;
  HAL_GPIO_Init(INPUT7_GPIO, &GPIO_InitStruct); 
	
  GPIO_InitStruct.Pin = INPUT8_GPIO_PIN;
  HAL_GPIO_Init(INPUT8_GPIO, &GPIO_InitStruct); 
}

void MapIoPro(void)
{//开关量输出
	
	if(Var.Out.Y00) { LED2_ON; } else { LED2_OFF; }
	if(Var.Out.Y01) { LED3_ON; } else { LED3_OFF; }
	
	if(Var.Out.Y00) { OUTPUT1_ON; } else { OUTPUT1_OFF; }
	if(Var.Out.Y01) { OUTPUT2_ON; } else { OUTPUT2_OFF; }
	if(Var.Out.Y02) { OUTPUT3_ON; } else { OUTPUT3_OFF; }
	if(Var.Out.Y03) { OUTPUT4_ON; } else { OUTPUT4_OFF; }
	
	//MOTOR3 端口
	if(Var.Out.Y04) { OUTPUT5_ON; } else { OUTPUT5_OFF; }
	if(Var.Out.Y05) { OUTPUT6_ON; } else { OUTPUT6_OFF; }
	
	//MOTOR4 端口
	if(Var.Out.Y07) { OUTPUT8_ON; } else { OUTPUT8_OFF; }
	if(Var.Out.Y10) { OUTPUT9_ON; } else { OUTPUT9_OFF; }
	if(Var.Out.Y11) { OUTPUT10_ON; } else { OUTPUT10_OFF; }
	
	//编码器端口
	if(Var.Out.Y12) { OUTPUT11_ON; } else { OUTPUT11_OFF; }
	if(Var.Out.Y13) { OUTPUT12_ON; } else { OUTPUT12_OFF; }
	if(Var.Out.Y14) { OUTPUT13_ON; } else { OUTPUT13_OFF; }
	if(Var.Out.Y15) { OUTPUT14_ON; } else { OUTPUT14_OFF; }

	//MOTOR1 端口
	//if(Var.Out.Y16) { OUTPUT15_ON; } else { OUTPUT15_OFF; }//方向
	if(Var.Out.Y17) { OUTPUT16_ON; } else { OUTPUT16_OFF; }//使能
	
	//MOTOR2 端口
	//if(Var.Out.Y20) { OUTPUT17_ON; } else { OUTPUT17_OFF; }//方向
	if(Var.Out.Y21) { OUTPUT18_ON; } else { OUTPUT18_OFF; }//使能
	
}


void GetInputPro(void)
{//获取外部输入状态,默认状态0，当有输入时为1
	u32 temp;
	temp = GetPinSt();
	
	Var.In.X00 = (temp>>0) & 0x01;
	Var.In.X01 = (temp>>1) & 0x01;
	Var.In.X02 = (temp>>2) & 0x01;
	Var.In.X03 = (temp>>3) & 0x01;
	Var.In.X04 = (temp>>4) & 0x01;
	Var.In.X05 = (temp>>5) & 0x01;
	Var.In.X06 = (temp>>6) & 0x01;
	Var.In.X07 = (temp>>7) & 0x01;
	
	
	Var.In.X10 = (temp>>8) & 0x01;
	Var.In.X11 = (temp>>9) & 0x01;
	Var.In.X12 = (temp>>10) & 0x01;
	Var.In.X13 = (temp>>11) & 0x01;
	Var.In.X14 = (temp>>12) & 0x01;
	Var.In.X15 = (temp>>13) & 0x01;
	Var.In.X16 = (temp>>14) & 0x01;
	Var.In.X17 = (temp>>15) & 0x01;
	
	Var.In.X20 = (temp>>16) & 0x01;
	
#if TEST_ENV
	Var.Motor1.ZeroPoint 	= Var.In.X13;	//电机原点
	Var.Motor1.Limit 			= ~Var.In.X14;	//气缸检测信号
	Var.Power.DoorFlag		= Var.In.X14;	//高压门控
#else
	Var.Motor1.ZeroPoint 	= Var.In.X00;	//电机原点1
	Var.Motor1.Limit 			= Var.In.X02;	//气缸检测信号,安全时时0，危险时是1
	
	Var.Power.DoorFlag		= Var.In.X07;	//高压门控     安全时时1，危险时是0
#endif
	
}


