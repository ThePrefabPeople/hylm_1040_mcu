#include "motor.h"
#include "main.h"
#include "bsp_gpio.h"
void StartMotor(void); 
void StopMotor(void); 


u8 MotorSerchZero(void)
{//电机找原点函数
	static u8 st=0;
	
	if(Var.Motor1.SerchZero)
	{
		Var.Motor1.SerchZero=0;
		st =0;
		printf("<MotorSerchZero>开始找零\r\n");
	}
	
	switch(st)
	{
		case 0:
		{//设置运动脉冲数量
			Var.Motor1.SetPulse = SUBVION*2;
			Var.Motor1.Pulse =0;
			OUTPUT15_ON;
			StartMotor();
			Toggle_Pulse = 16800;
			st++;
			printf("电机开始寻找原点\r\n");
			break;
		}
		case 1:
		{
			if(0 == Var.Motor1.ZeroPoint)
			{//接近原点1
				st =2;
				printf("接近原点1。\r\n");
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//运行一圈仍然没找到原点
				st =0;
				printf("电机开始寻找原点失败。\r\n");
				return  2;
			}
			break;
		}
		
		case 2:
		{
			if(1 == Var.Motor1.ZeroPoint)
			{//接近原点2,开始爬行
				Toggle_Pulse = 42000;//
				Var.Motor1.ZeroPoint2 =0;
				st=3;
				printf("接近原点2。\r\n");
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//运行一圈仍然没找到原点
				st =0;
				printf("电机开始寻找原点失败。\r\n");
				return  2;
			}
			break;
		}
		case 3:
		{
			if(0 == Var.Motor1.ZeroPoint || Var.Motor1.ZeroPoint2 ==1)
			{//接近原点3,开始爬行
				
				Var.Motor1.SetPulse = Var.Motor1.CalibrationPulse;
				Var.Motor1.Pulse =0;
				st=4;
				printf("接近原点3 %d,%d,%d。\r\n",Var.Motor1.CalibrationPulse,Var.Motor1.ZeroPoint,Var.Motor1.ZeroPoint2);
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//运行一圈仍然没找到原点
				st =0;
				printf("电机开始寻找原点失败。\r\n");
				return  2;
			}
			break;
		}
		case 4:
		{
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//找到原点
				st =0;
				printf("电机找原点成功。\r\n");
				return  1;
			}
			break;
		}
		default:st =0;break;
	}
	return 0;
}


void MotorMoveTo(u8 d)
{//电机移动到指定位置
	static u8 st=0;
	switch(st)
	{
		case 0:
		{//设置移动脉冲参数
			Var.Motor1.SetPulse = d*(SUBVION/20);
			if(Var.Motor1.SetPulse != Var.Motor1.Pulse )
			{
				if(Var.Motor1.SetPulse > Var.Motor1.Pulse)OUTPUT15_ON; 
				else if(Var.Motor1.SetPulse < Var.Motor1.Pulse) OUTPUT15_OFF;
				if(Var.Motor1.Limit == 1 )
				{
					StartMotor();
					st++;
				
					printf("开始移动:于%d秒从位置%d(%d)向位置%d(%d)移动!\r\n",\
																										Var.Sys.SecCnt,\
																										Var.Motor1.CurPoint,\
																										Var.Motor1.Pulse,\
																										d,\
																										Var.Motor1.SetPulse);
				}
			}
			else
			{
				if(0 == Var.Motor1.StopDelay)
				{
					Var.BSPwork.NextStep = 1;
				}
			}
			break;
		}
		case 1:
		{//等待运行结束
			if(Var.Motor1.SetPulse == Var.Motor1.Pulse) 
			{
				
				printf("移动结束:于%d秒从位置%d移动到位置%d\r\n",\
																					Var.Sys.SecCnt,\
																					Var.Motor1.CurPoint,\
																					Var.Motor1.SetPoint);
				Var.Motor1.CurPoint =Var.Motor1.SetPoint;
				
				Var.Motor1.StopDelay =20;
				st=0;
			}
			break;
		}
		default :st =0;break;
	}
}

void MotorRun(void)
{//电机运行控制
	u8 tmp;
	switch(Var.BSPwork.Stu)
	{//运行状态
		case SYSINIT:
		{//初始化开机找原点
			if(Var.Motor1.InitDelay){return;}//开机延时
			
			tmp = MotorSerchZero();//找零点
			if(1 == tmp )
			{//找到原点
				Var.Motor1.SetPulse =0;
				Var.Motor1.Pulse =0;
				Var.Motor1.SetPoint =0;
				Var.Motor1.CurPoint =0;
				Var.BSPwork.Stu = SYSIDLE;
				printf("<MotorRun>电机找到零点,进入到IDLE模式！\r\n");
			}
			else if(2 == tmp)
			{//找原点失败
				StopMotor();
				Var.BSPwork.Stu = SYSEROR;
				printf("<MotorRun>电机找原点失败，运行脉冲走完了进入到SYSEROR模式！\r\n");
			}
			
			if(Var.Motor1.Limit == 0 )
			{//气缸阻止信号
				StopMotor();
				Var.BSPwork.Stu = SYSEROR;
				printf("<MotorRun>检查电机限位信号！,按复位键重新找零点。\r\n");
			}
			break;
		}
		case SYSIDLE:
		case KEYSTOP:
		case KEYRUN:
		case KEYSET:
		{//等待运行指令,运行到指定位置
			//if(1 == Var.Motor1.SerchZero) break;

			MotorMoveTo(Var.Motor1.SetPoint);
			break;
		}
		default :break;
	}
}

void StartMotor(void) 
{
	TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_ENABLE);
  HAL_TIM_OC_Start_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// 使能中断 关闭比较输出
}


void StopMotor(void) 
{
	Var.Motor1.Pu =0;
	TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
  HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// 使能中断 关闭比较输出
	
	//HAL_TIM_Base_Stop(&htimx_STEPMOTOR);
}

