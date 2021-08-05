#include "timer.h"
#include "main.h"
#include "bsp_usartx.h"
#include "usart.h"


void TickMs1Pro(void)
{
}
void TickMs10Pro(void)
{
	
	if(Var.Sys.KeyCnt)Var.Sys.KeyCnt--;	
	
	
	if (Var.Com5Base.devRcvState != REC_INIT)
	{//串口5接收超时
		Var.Com5Base.devRcvTimer++;
		if (Var.Com5Base.devRcvTimer >= DEV_RCV_TIMEOUT)
		{
			Var.Com5Base.devRcvState = REC_INIT;
			Var.Com5Base.devRcvTimer = 0;
			if( Var.Com5Base.bDevRcvBuf != 1	)
			{
				Var.Com5Base.bDevRcvBuf=0;		
			}
		}
	}
}
void TickMs100Pro(void)
{
	
	Var.Sys.SecCnt++;
	if(Var.Sys.ResetCnt)Var.Sys.ResetCnt--;
	if(Var.BSPwork.SecCnt)Var.BSPwork.SecCnt--;
	if(Var.Motor1.StopDelay)Var.Motor1.StopDelay--;
	if(Var.AD7190.Rcnt)Var.AD7190.Rcnt--;
	if(Var.Monitor.UpCnt)Var.Monitor.UpCnt--;
}

void TickMs1000Pro(void)
{
	if(Var.Sys.Led1Cnt)Var.Sys.Led1Cnt--;
	if(Var.Sys.DebugCnt)Var.Sys.DebugCnt--;
	
	if(Var.Motor1.InitDelay)Var.Motor1.InitDelay--;
	
	if(Var.Com5.SecCnt)Var.Com5.SecCnt--;
	
	if(Var.BSPwork.Delay)Var.BSPwork.Delay--;
	
	if(Var.BSPwork.StopCnt)Var.BSPwork.StopCnt--;
	
	if(Var.Recorder.DeCnt )Var.Recorder.DeCnt--;
	
	
	if(Var.Monitor.UpCnt2)Var.Monitor.UpCnt2--;
}


void HAL_SYSTICK_Callback(void)
{//滴答定时器回调函数
	static u16 ct1=0;
	static u16 ct2=0;
	static u16 ct3=0;
	
	TickMs1Pro();
	if(++ct1 >= 10)
	{
		ct1=0;
		TickMs10Pro();
		if(++ct2 >= 10)
		{
			ct2=0;
			TickMs100Pro();	
			if(++ct3 >= 10)
			{
				ct3=0;
				TickMs1000Pro();
			}
		}
	}
}
