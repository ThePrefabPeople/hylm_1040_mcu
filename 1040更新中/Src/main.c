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
  HAL_Init(); 					//��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ��
  SystemClock_Config();	// ����ϵͳʱ��
	UsartInit();					//��ʼ������
  LedInit(); 
  BeepInit();
  OutputInit();
	bsp_InitButton();	//����˿ڳ�ʼ��
	MotorInit();			//������Ƴ�ʼ��
  MX_SPIFlash_Init();
	FlashInfo();			//���FLASH������Ϣ
  AD5689_Init();
	ParmInit();			//��ȡ���в���
	queue_reset();	//���������ջ��λ�����
	Ad7190Init();
	
//�жϼ��ԭ��	
//	GPIO_InitTypeDef GPIO_InitStruct;
//	
//	GPIO_InitStruct.Pin = ORIGIN_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  HAL_GPIO_Init(GPIO_X00, &GPIO_InitStruct);  
//	
//  /* �����ж����ȼ� */
//  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
//  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	
	Var.Motor1.CalibrationPulse	= Var.EParam.Pulse;
	Var.Motor1.InitDelay = POWERON_DELAY;//������ʱ
	Var.BSPwork.Stu = SYSINIT;
	
	BEEP_ON;HAL_Delay(200);BEEP_OFF;
	printf("<main>�汾��:%s\r\n",(char *)VERSION);
	printf("<main>K1:��ʼ;K2:ֹͣ;K3:��λ;K4:ģ��ԭ���ź�;K5:ģ������λ���ź�\r\n");   
	
  while (1)
  {
		Uart1Pro();			//����1���Բ���
		Uart5Pro();			//���ղ�������λ������
		RunLedPro();		//ָʾ������
		MapIoPro();			//io���
		GetInputPro();	//����ɼ�
		FlashWritePro();//������� 
		AD7190Pro();		//ģ�����ɼ�
		KeyPro();				//��������
		WorkPro();			//�������̿���
		MotorRun();			//��� 
		MonitorPro();		//������
		PowerPro();			//��ѹ��Դ���		
		if(0 == Var.Sys.KeyCnt)
		{//�����ɼ��̶�10����
			bsp_KeyPro();
			Var.Sys.KeyCnt=1;
		}
		if(0 == Var.Sys.DebugCnt)
		{//1�붨ʱ����
			//printf("ϵͳ״̬��%d\r\n",Var.BSPwork.Stu);
			Var.Sys.DebugCnt=10;
		}
  }
}


