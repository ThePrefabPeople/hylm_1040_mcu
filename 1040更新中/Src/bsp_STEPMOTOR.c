
#include "bsp_STEPMOTOR.h" 
#include "main.h"
#include "bsp_gpio.h"
//���1�Ƚ������ʱ��
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
{//�����ʼ��

  STEPMOTOR_TIMx_Init();
  HAL_TIM_Base_Start(&htimx_STEPMOTOR);
  TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
	HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);
	
}

static void STEPMOTOR_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* ���Ŷ˿�ʱ��ʹ�� */
  STEPMOTOR_TIM_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA_GPIO_CLK_ENABLE();
  
//		/* �������ԭ��������*/
//  GPIO_InitStruct.Pin = ORIGIN_PIN;
//	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
//	HAL_GPIO_Init(ORIGIN_PORT, &GPIO_InitStruct);
//	
//  HAL_NVIC_SetPriority(ORIGIN_EXTI_IRQn, 0, 1);
//	HAL_NVIC_EnableIRQ(ORIGIN_EXTI_IRQn);
  
  /* �����������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AFx_TIMx;        // GPIO��������TIM���ù���
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL_PORT, &GPIO_InitStruct);
  
  /* �����������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_DIR_PORT, &GPIO_InitStruct);
  
  /* �������ѻ�ʹ�ܿ�������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_ENA_PORT, &GPIO_InitStruct);
  
  //STEPMOTOR_DIR_FORWARD();
 // STEPMOTOR_OUTPUT_ENABLE();
}

/**
  * ��������: ��������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void STEPMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef 					sClockSourceConfig;             // ��ʱ��ʱ��
  TIM_MasterConfigTypeDef 				sMasterConfig;                 	// ��ʱ����ģʽ����
  TIM_BreakDeadTimeConfigTypeDef 	sBreakDeadTimeConfig;   				// ɲ��������ʱ������
  TIM_OC_InitTypeDef 							sConfigOC; 

  /* ��ʱ�������������� */
  //HAL_TIM_Base_DeInit(&htimx_STEPMOTOR);
  htimx_STEPMOTOR.Instance = 					STEPMOTOR_TIMx;                        // ��ʱ�����
  htimx_STEPMOTOR.Init.Prescaler = 		STEPMOTOR_TIM_PRESCALER;               // ��ʱ��Ԥ��Ƶ��
  htimx_STEPMOTOR.Init.CounterMode = 	TIM_COUNTERMODE_UP;                    // �����������ϼ���
  htimx_STEPMOTOR.Init.Period = 			STEPMOTOR_TIM_PERIOD;                  // ��ʱ������
  htimx_STEPMOTOR.Init.ClockDivision=	TIM_CLOCKDIVISION_DIV1;                // ʱ�ӷ�Ƶ
  htimx_STEPMOTOR.Init.RepetitionCounter = STEPMOTOR_TIM_REPETITIONCOUNTER;  // �ظ�������
  HAL_TIM_Base_Init(&htimx_STEPMOTOR);

  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;                 // ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&htimx_STEPMOTOR, &sClockSourceConfig);

  /* ��ʼ����ʱ���Ƚ�������� */
  HAL_TIM_OC_Init(&htimx_STEPMOTOR);
  
  /* ��ʱ�������ģʽ */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = 		TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx_STEPMOTOR, &sMasterConfig);
  
  /* ɲ��������ʱ������ */
  sBreakDeadTimeConfig.OffStateRunMode = 	TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = 				TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 				0;
  sBreakDeadTimeConfig.BreakState = 			TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = 		TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = 	TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_STEPMOTOR, &sBreakDeadTimeConfig);

  /* ��ʱ���Ƚ�������� */
  sConfigOC.OCMode = 			TIM_OCMODE_TOGGLE;           // �Ƚ����ģʽ����ת���
  sConfigOC.Pulse = 			Toggle_Pulse;                // ������
  sConfigOC.OCPolarity = 	TIM_OCPOLARITY_HIGH;         // �������
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // ����ͨ���������
  sConfigOC.OCFastMode = 	TIM_OCFAST_DISABLE;          // ����ģʽTIM_OCIDLESTATE_SET
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_TIM_CHANNEL_x);

  /* STEPMOTOR���GPIO��ʼ������ */
  STEPMOTOR_GPIO_Init();
  
  /* ���ö�ʱ���ж����ȼ���ʹ�� */
 HAL_NVIC_SetPriority(STEPMOTOR_TIMx_IRQn, 0, 0);//���ö�ʱ��8��ռ���ȼ�����Ӧ���ȼ�
 
 HAL_NVIC_EnableIRQ(STEPMOTOR_TIMx_IRQn);//�򿪶�ʱ��8�ж�
	
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    STEPMOTOR_TIM_RCC_CLK_ENABLE();
  }
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    STEPMOTOR_TIM_RCC_CLK_DISABLE();
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL_PORT,STEPMOTOR_TIM_PUL_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN);
    
    HAL_NVIC_DisableIRQ(STEPMOTOR_TIMx_IRQn);
  }
} 

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{//�Ƚ�ƥ���жϻص�����3
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
		{//һ��������������
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
				printf("����%d����\r\n",c);
				c=0;
				dpu =0;
				Var.Motor1.Pu =0;
				
				//ֹͣ�Ƚ�ƥ�����
				TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
				HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// ʹ���ж� �رձȽ����
			}
			
			if(Var.BSPwork.Stu != SYSINIT)
			{
				if(dpu >160)
				{//���ٽ׶�
					if(Var.Motor1.Pu <= 160)Toggle_Pulse =sp[160 - Var.Motor1.Pu];
					
					if(Toggle_Pulse < sp[0]) Toggle_Pulse =sp[0];
				}
				else
				{//���ٽ׶�
					if(dpu <= 160)Toggle_Pulse =sp[160 - dpu];
					
					if(Toggle_Pulse > sp[160]) Toggle_Pulse =sp[160];
				}
			}
		}
	}
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{//ԭ���жϼ��
	if(GPIO_Pin == ORIGIN_PIN)
	{
		HAL_Delay(10);/* ��ʱһС��ʱ�䣬�������� */
		if(HAL_GPIO_ReadPin(ORIGIN_PORT,ORIGIN_PIN) == 1 )//������Ϊԭ���ź�
		{															  
			Var.Motor1.ZeroPoint2 =1;
		}
		__HAL_GPIO_EXTI_CLEAR_IT(ORIGIN_PIN);	
	}
}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
