/**
总结一下，串口中断接收的流程：

USART2_IRQHandler(void)    ->    
HAL_UART_IRQHandler(UART_HandleTypeDef *huart)    ->    
UART_Receive_IT(UART_HandleTypeDef *huart)    ->    
HAL_UART_RxCpltCallback(huart);
Callback函数就是用户要重写在main.c里的回调函数。
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp_usartx.h"
#include "usart.h"

uint8_t aRxBuffer[6];

UART_HandleTypeDef husart1;
UART_HandleTypeDef husart2;
UART_HandleTypeDef husart5;
UART_HandleTypeDef husart6;

void RecCom5(u8 c);

/**
  * 函数功能: 串口硬件初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(huart->Instance==USART1x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART1x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART1x_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART1x_AFx;
    HAL_GPIO_Init(USART1x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART1x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART1x_Rx_GPIO, &GPIO_InitStruct);
  }
  else if(huart->Instance==USART2x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART2x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART2x_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART2x_AFx;
    HAL_GPIO_Init(USART2x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART2x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART2x_Rx_GPIO, &GPIO_InitStruct);    
  }
  else if(huart->Instance==USART5x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART5x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART5x_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART5x_AFx;
    HAL_GPIO_Init(USART5x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART5x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART5x_Rx_GPIO, &GPIO_InitStruct);    
  }
  else if(huart->Instance==USART6x)
  {
    /* 使能串口功能引脚GPIO时钟 */
    USART6x_GPIO_ClK_ENABLE();
  
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin = USART6x_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = USART6x_AFx;
    HAL_GPIO_Init(USART6x_Tx_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = USART6x_Rx_GPIO_PIN;
    HAL_GPIO_Init(USART6x_Rx_GPIO, &GPIO_InitStruct);    
  }  
}

/**
  * 函数功能: NVIC配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void MX_NVIC_USARTx_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART1x_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USART1x_IRQn);
  
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2x_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USART2x_IRQn);
	
  HAL_NVIC_SetPriority(USART5x_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(USART5x_IRQn);


  /* USART6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART6x_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(USART6x_IRQn);  
  
}


/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void UsartInit(void)
{
  /* 配置串口中断并使能，需要放在HAL_UART_Init函数后执行修改才有效 */
  MX_NVIC_USARTx_Init();
  
  /* 串口外设时钟使能 */
  USART1_RCC_CLK_ENABLE();
  
  husart1.Instance = USART1x;
  husart1.Init.BaudRate = USART1x_BAUDRATE;
  husart1.Init.WordLength = UART_WORDLENGTH_8B;
  husart1.Init.StopBits = UART_STOPBITS_1;
  husart1.Init.Parity = UART_PARITY_NONE;
  husart1.Init.Mode = UART_MODE_TX_RX;
  husart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart1);


  /* 串口外设时钟使能 */
  USART2_RCC_CLK_ENABLE();
  
  husart2.Instance = USART2x;
  husart2.Init.BaudRate = USART2x_BAUDRATE;
  husart2.Init.WordLength = UART_WORDLENGTH_8B;
  husart2.Init.StopBits = UART_STOPBITS_1;
  husart2.Init.Parity = UART_PARITY_NONE;
  husart2.Init.Mode = UART_MODE_TX_RX;
  husart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart2);
	
  USART5_RCC_CLK_ENABLE();
  
  husart5.Instance = USART5x;
  husart5.Init.BaudRate = USART5x_BAUDRATE;
  husart5.Init.WordLength = UART_WORDLENGTH_8B;
  husart5.Init.StopBits = UART_STOPBITS_1;
  husart5.Init.Parity = UART_PARITY_NONE;
  husart5.Init.Mode = UART_MODE_TX_RX;
  husart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husart5.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husart5);
  
  /* 串口外设时钟使能 */
//  USART6_RCC_CLK_ENABLE();
//  
//  husart6.Instance = USART6x;
//  husart6.Init.BaudRate = USART6x_BAUDRATE;
//  husart6.Init.WordLength = UART_WORDLENGTH_8B;
//  husart6.Init.StopBits = UART_STOPBITS_1;
//  husart6.Init.Parity = UART_PARITY_NONE;
//  husart6.Init.Mode = UART_MODE_TX_RX;
//  husart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  husart6.Init.OverSampling = UART_OVERSAMPLING_16;
//  HAL_UART_Init(&husart6);

	
  /* 使能中断接收，进入中断回调函数 */
	HAL_UART_Receive_IT(&husart1,&aRxBuffer[0],1);
	HAL_UART_Receive_IT(&husart5,&aRxBuffer[1],1);
  HAL_UART_Receive_IT(&husart2,&aRxBuffer[1],1);
	
//  HAL_UART_Receive_IT(&husart6,&aRxBuffer[2],1);
}

int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&husart1, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

void RecCom1(u8 c);
void RecCom2(u8 c);

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{//中断回调函数
  if(UartHandle->Instance==USART1)
  {  
    //HAL_UART_Transmit(&husart1,&aRxBuffer[0],1,0);
		RecCom1(aRxBuffer[0]);
    HAL_UART_Receive_IT(&husart1,&aRxBuffer[0],1);
  }
  else if(UartHandle->Instance==USART2)
  {
    //HAL_UART_Transmit(&husart2,&aRxBuffer[1],1,0);
		
		RecCom1(aRxBuffer[1]);
    HAL_UART_Receive_IT(&husart2,&aRxBuffer[1],1);  
	
  }
  else if(UartHandle->Instance==UART5)
  {
		RecCom5(aRxBuffer[1]);
    HAL_UART_Receive_IT(&husart5,&aRxBuffer[1],1);    
  }
  else if(UartHandle->Instance==USART6)
  {
    HAL_UART_Transmit(&husart6,&aRxBuffer[2],1,0);
    HAL_UART_Receive_IT(&husart6,&aRxBuffer[2],1);    
  }
    
}

void RecCom1(u8 c)
{//协议接收
	static u8 ptr=0;
	Var.Com5.Buf[ptr%100]=c;
	ptr++;
	
	if(ptr > 2 && Var.Com5.Buf[ptr-2] == '\r' && Var.Com5.Buf[ptr-1] == '\n' )
	{
		Var.Com5.Busy =1;
		Var.Com5.Num = ptr;
		ptr =0;
	}
}
void RecCom2(u8 c)
{//协议接收
	c=c;
}

void send0_buf(u8 *tmpbuf,u8 buflen)
{//缓冲区发送
	
	HAL_UART_Transmit(&husart5,tmpbuf,buflen,0XFFFF);
}

//20 00 00 00 00 0D 0A
void RecCom5(u8 c)
{
	if(Var.Com5Base.bDevRcvBuf!=1)
	{
		switch (Var.Com5Base.devRcvState)
		{
			case REC_INIT:
			if(DEV_RCV_SYNC==c)
			{
				Var.Com5Base.devRcvState=REC_ADDR;	
			}
			break;
		case REC_ADDR:        
			if(c==Var.g_iddevice)
			{
				Var.Com5Base.devRcvCtr = 0;
				Var.Com5Base.RcvBuf[Var.Com5Base.devRcvCtr] = c;
				Var.Com5Base.devRcvCtr++; 
				Var.Com5Base.devRcvState = REC_CMND;	    	   		
			}
			else
			{
				Var.Com5Base.devRcvState=REC_INIT;	
			}
			break;
		case REC_CMND:        
			Var.Com5Base.devRcvChs=c; 
			Var.Com5Base.RcvBuf[Var.Com5Base.devRcvCtr] = c;          
			Var.Com5Base.devRcvCtr++; 
			Var.Com5Base.devRcvState = REC1_LEN; 
			Var.Com5Base.bDevRcvBuf=2;
			break;
		case  REC_LEN: 
			Var.Com5Base.devRcvChs^=c;	      	  
			Var.Com5Base.devRcvLength=c;       	  
			Var.Com5Base.RcvBuf[Var.Com5Base.devRcvCtr] = c;
			Var.Com5Base.devRcvCtr++;      	
			if(Var.Com5Base.devRcvLength)
			{
				if(Var.Com5Base.devRcvLength<REC_BUFLEN)
				{
					Var.Com5Base.devRcvState=REC_DATA;
				}
				else
				{
					Var.Com5Base.devRcvState=REC_INIT;	
					Var.Com5Base.bDevRcvBuf=0;
				}
			}
			else
			{
				Var.Com5Base.devRcvState=REC_CHS;	
			}     
			break;
		case REC_DATA:  
			Var.Com5Base.devRcvChs^=c;  
			
			Var.Com5Base.RcvBuf[Var.Com5Base.devRcvCtr] = c;
			Var.Com5Base.devRcvCtr++;
			Var.Com5Base.devRcvLength--;
			if(Var.Com5Base.devRcvLength==0)
				Var.Com5Base.devRcvState = REC_CHS;  
			break;
			
		case REC_CHS:       // 校验字节接收
			Var.Com5Base.RcvBuf[Var.Com5Base.devRcvCtr] = c;
			Var.Com5Base.devRcvCtr++;
			if(c==Var.Com5Base.devRcvChs)
			{    	  		
				Var.Com5Base.devRcvState = REC_END0;
			}
			else
			{
				Var.Com5Base.devRcvState = REC_INIT;
			}
			
			break;
		case REC_END0:       // 校验字节接收
			if(c==DEV_RCV_END0)
			{    	  		
				Var.Com5Base.devRcvState = REC_END1;
			}
			else
			{
				Var.Com5Base.devRcvState = REC_INIT;	
			}
			break;
		case REC_END1:       // 校验字节接收
			if(c == DEV_RCV_END1)
			{
				Var.Com5Base.bDevRcvBuf = 1;
			}
			else
			{
				Var.Com5Base.bDevRcvBuf=0;	
			}
			Var.Com5Base.devRcvState = REC_INIT;
			
			break;
		default: 
			Var.Com5Base.devRcvState = REC_INIT;
			break;
      }	
  
	}
	Var.Com5Base.devRcvTimer = 0;  // 接收字节时间清零
}

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
