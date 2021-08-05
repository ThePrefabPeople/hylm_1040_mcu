
#include "bsp_STEPMOTOR.h" 
#include "main.h"
#include "bsp_gpio.h"
//电机1比较输出定时器
TIM_HandleTypeDef htimx_STEPMOTOR;

__IO uint32_t Toggle_Pulse = 50288; 
const u16 sp[161]=
{ 8504, 8512, 8521, 8530, 8540, 8551, 8563, 8575, 8589, 8603, 8619, 8636, 8655, 8674, 8695, 8718, 8743, 8769, 8798, 8828,
 8861, 8897, 8935, 8976, 9021, 9068, 9119, 9174, 9233, 9297, 9365, 9438, 9517, 9602, 9692, 9790, 9894,10006,10126,10254,
10392,10539,10697,10865,11045,11237,11442,11660,11893,12141,12405,12685,12982,13297,13631,13984,14358,14752,15167,15603,
16062,16543,17047,17573,18122,18694,19287,19903,20540,21198,21874,22570,23282,24011,24753,25508,26273,27047,27828,28613,
29400,30187,30972,31753,32527,33292,34047,34789,35518,36230,36926,37602,38260,38897,39513,40106,40678,41227,41753,42257,
42738,43197,43633,44048,44442,44816,45169,45503,45818,46115,46395,46659,46907,47140,47358,47563,47755,47935,48103,48261,
48408,48546,48674,48794,48906,49010,49108,49198,49283,49362,49435,49503,49567,49626,49681,49732,49779,49824,49865,49903,
49939,49972,50002,50031,50057,50082,50105,50126,50145,50164,50181,50197,50211,50225,50237,50249,50260,50270,50279,50288,50288};
	
void MotorInit(void)
{//电机初始化

  STEPMOTOR_TIMx_Init();
  HAL_TIM_Base_Start(&htimx_STEPMOTOR);
  TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
	HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);
	
}

static void STEPMOTOR_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* 引脚端口时钟使能 */
  STEPMOTOR_TIM_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA_GPIO_CLK_ENABLE();
  
//		/* 步进电机原点检测输入*/
//  GPIO_InitStruct.Pin = ORIGIN_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
//	HAL_GPIO_Init(ORIGIN_PORT, &GPIO_InitStruct);
//	
//  HAL_NVIC_SetPriority(ORIGIN_EXTI_IRQn, 0, 1);
//	HAL_NVIC_EnableIRQ(ORIGIN_EXTI_IRQn);
  
  /* 驱动器脉冲控制引脚IO初始化 */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AFx_TIMx;        // GPIO引脚用做TIM复用功能
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL_PORT, &GPIO_InitStruct);
  
  /* 驱动器方向控制引脚IO初始化 */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(STEPMOTOR_DIR_PORT, &GPIO_InitStruct);
  
  /* 驱动器脱机使能控制引脚IO初始化 */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(STEPMOTOR_ENA_PORT, &GPIO_InitStruct);
  
  //STEPMOTOR_DIR_FORWARD();
 // STEPMOTOR_OUTPUT_ENABLE();
}

/**
  * 函数功能: 驱动器定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void STEPMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef 					sClockSourceConfig;             // 定时器时钟
  TIM_MasterConfigTypeDef 				sMasterConfig;                 	// 定时器主模式配置
  TIM_BreakDeadTimeConfigTypeDef 	sBreakDeadTimeConfig;   				// 刹车和死区时间配置
  TIM_OC_InitTypeDef 							sConfigOC; 

  /* 定时器基本环境配置 */
  //HAL_TIM_Base_DeInit(&htimx_STEPMOTOR);
  htimx_STEPMOTOR.Instance = 					STEPMOTOR_TIMx;                        // 定时器编号
  htimx_STEPMOTOR.Init.Prescaler = 		STEPMOTOR_TIM_PRESCALER;               // 定时器预分频器
  htimx_STEPMOTOR.Init.CounterMode = 	TIM_COUNTERMODE_UP;                    // 计数方向：向上计数
  htimx_STEPMOTOR.Init.Period = 			STEPMOTOR_TIM_PERIOD;                  // 定时器周期
  htimx_STEPMOTOR.Init.ClockDivision=	TIM_CLOCKDIVISION_DIV1;                // 时钟分频
  htimx_STEPMOTOR.Init.RepetitionCounter = STEPMOTOR_TIM_REPETITIONCOUNTER;  // 重复计数器
  HAL_TIM_Base_Init(&htimx_STEPMOTOR);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;                 // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_STEPMOTOR, &sClockSourceConfig);

  /* 初始化定时器比较输出环境 */
  HAL_TIM_OC_Init(&htimx_STEPMOTOR);
  
  /* 定时器主输出模式 */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = 		TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx_STEPMOTOR, &sMasterConfig);
  
  /* 刹车和死区时间配置 */
  sBreakDeadTimeConfig.OffStateRunMode = 	TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = 				TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 				0;
  sBreakDeadTimeConfig.BreakState = 			TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = 		TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = 	TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_STEPMOTOR, &sBreakDeadTimeConfig);

  /* 定时器比较输出配置 */
  sConfigOC.OCMode = 			TIM_OCMODE_TOGGLE;           // 比较输出模式：反转输出
  sConfigOC.Pulse = 			Toggle_Pulse;                // 脉冲数
  sConfigOC.OCPolarity = 	TIM_OCPOLARITY_HIGH;         // 输出极性
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // 互补通道输出极性
  sConfigOC.OCFastMode = 	TIM_OCFAST_DISABLE;          // 快速模式TIM_OCIDLESTATE_SET
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_TIM_CHANNEL_x);

  /* STEPMOTOR相关GPIO初始化配置 */
  STEPMOTOR_GPIO_Init();
  
  /* 配置定时器中断优先级并使能 */
 HAL_NVIC_SetPriority(STEPMOTOR_TIMx_IRQn, 0, 0);//配置定时器8抢占优先级和响应优先级
 
 HAL_NVIC_EnableIRQ(STEPMOTOR_TIMx_IRQn);//打开定时器8中断
	
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    STEPMOTOR_TIM_RCC_CLK_ENABLE();
  }
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    STEPMOTOR_TIM_RCC_CLK_DISABLE();
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL_PORT,STEPMOTOR_TIM_PUL_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN);
    
    HAL_NVIC_DisableIRQ(STEPMOTOR_TIMx_IRQn);
  }
} 

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{//比较匹配中断回调函数3
  __IO uint32_t count;
  __IO uint32_t tmp;
	__IO static u8 ct=0;
	__IO u32	dpu;
	__IO static u32	c=0;
	
	
  if(__HAL_TIM_GET_IT_SOURCE(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx) !=RESET)
  {
    __HAL_TIM_CLEAR_IT(&htimx_STEPMOTOR, STEPMOTOR_TIM_IT_CCx);
		
		count =__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
		tmp = STEPMOTOR_TIM_PERIOD & (count+Toggle_Pulse);
		__HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x,tmp);
		
		ct++;
		if(ct >=2)
		{//一个完整脉冲走完
			ct=0;
			c++;
			if(Var.Motor1.Pulse > Var.Motor1.SetPulse)
			{
				dpu = Var.Motor1.Pulse - Var.Motor1.SetPulse;
				Var.Motor1.Pulse--;
				
				if(Var.Motor1.Pu < 160)Var.Motor1.Pu++;
			}
			else if(Var.Motor1.Pulse < Var.Motor1.SetPulse)
			{
				dpu = Var.Motor1.SetPulse - Var.Motor1.Pulse;
				Var.Motor1.Pulse++;
				
				if(Var.Motor1.Pu < 160)Var.Motor1.Pu++;
			}		
			
			if(Var.Motor1.Pulse == Var.Motor1.SetPulse)
			{
				printf("走了%d脉冲\r\n",c);
				c=0;
				dpu =0;
				Var.Motor1.Pu =0;
				
				//停止比较匹配输出
				TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
				HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// 使能中断 关闭比较输出
			}
			
			if(Var.BSPwork.Stu != SYSINIT)
			{
				if(dpu >160)
				{//加速阶段
					if(Var.Motor1.Pu <= 160)Toggle_Pulse =sp[160 - Var.Motor1.Pu];
					
					if(Toggle_Pulse < sp[0]) Toggle_Pulse =sp[0];
				}
				else
				{//减速阶段
					if(dpu <= 160)Toggle_Pulse =sp[160 - dpu];
					
					if(Toggle_Pulse > sp[160]) Toggle_Pulse =sp[160];
				}
			}
		}
	}
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{//原点中断检测
	if(GPIO_Pin == ORIGIN_PIN)
	{
		HAL_Delay(10);/* 延时一小段时间，消除抖动 */
		if(HAL_GPIO_ReadPin(ORIGIN_PORT,ORIGIN_PIN) == 1 )//上升沿为原点信号
		{															  
			Var.Motor1.ZeroPoint2 =1;
		}
		__HAL_GPIO_EXTI_CLEAR_IT(ORIGIN_PIN);	
	}
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
