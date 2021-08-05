#include "motor.h"
#include "main.h"
#include "bsp_gpio.h"
void StartMotor(void); 
void StopMotor(void); 


u8 MotorSerchZero(void)
{//�����ԭ�㺯��
	static u8 st=0;
	
	if(Var.Motor1.SerchZero)
	{
		Var.Motor1.SerchZero=0;
		st =0;
		printf("<MotorSerchZero>��ʼ����\r\n");
	}
	
	switch(st)
	{
		case 0:
		{//�����˶���������
			Var.Motor1.SetPulse = SUBVION*2;
			Var.Motor1.Pulse =0;
			OUTPUT15_ON;
			StartMotor();
			Toggle_Pulse = 16800;
			st++;
			printf("�����ʼѰ��ԭ��\r\n");
			break;
		}
		case 1:
		{
			if(0 == Var.Motor1.ZeroPoint)
			{//�ӽ�ԭ��1
				st =2;
				printf("�ӽ�ԭ��1��\r\n");
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//����һȦ��Ȼû�ҵ�ԭ��
				st =0;
				printf("�����ʼѰ��ԭ��ʧ�ܡ�\r\n");
				return  2;
			}
			break;
		}
		
		case 2:
		{
			if(1 == Var.Motor1.ZeroPoint)
			{//�ӽ�ԭ��2,��ʼ����
				Toggle_Pulse = 42000;//
				Var.Motor1.ZeroPoint2 =0;
				st=3;
				printf("�ӽ�ԭ��2��\r\n");
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//����һȦ��Ȼû�ҵ�ԭ��
				st =0;
				printf("�����ʼѰ��ԭ��ʧ�ܡ�\r\n");
				return  2;
			}
			break;
		}
		case 3:
		{
			if(0 == Var.Motor1.ZeroPoint || Var.Motor1.ZeroPoint2 ==1)
			{//�ӽ�ԭ��3,��ʼ����
				
				Var.Motor1.SetPulse = Var.Motor1.CalibrationPulse;
				Var.Motor1.Pulse =0;
				st=4;
				printf("�ӽ�ԭ��3 %d,%d,%d��\r\n",Var.Motor1.CalibrationPulse,Var.Motor1.ZeroPoint,Var.Motor1.ZeroPoint2);
			}
			
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//����һȦ��Ȼû�ҵ�ԭ��
				st =0;
				printf("�����ʼѰ��ԭ��ʧ�ܡ�\r\n");
				return  2;
			}
			break;
		}
		case 4:
		{
			if(Var.Motor1.Pulse >= Var.Motor1.SetPulse)
			{//�ҵ�ԭ��
				st =0;
				printf("�����ԭ��ɹ���\r\n");
				return  1;
			}
			break;
		}
		default:st =0;break;
	}
	return 0;
}


void MotorMoveTo(u8 d)
{//����ƶ���ָ��λ��
	static u8 st=0;
	switch(st)
	{
		case 0:
		{//�����ƶ��������
			Var.Motor1.SetPulse = d*(SUBVION/20);
			if(Var.Motor1.SetPulse != Var.Motor1.Pulse )
			{
				if(Var.Motor1.SetPulse > Var.Motor1.Pulse)OUTPUT15_ON; 
				else if(Var.Motor1.SetPulse < Var.Motor1.Pulse) OUTPUT15_OFF;
				if(Var.Motor1.Limit == 1 )
				{
					StartMotor();
					st++;
				
					printf("��ʼ�ƶ�:��%d���λ��%d(%d)��λ��%d(%d)�ƶ�!\r\n",\
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
		{//�ȴ����н���
			if(Var.Motor1.SetPulse == Var.Motor1.Pulse) 
			{
				
				printf("�ƶ�����:��%d���λ��%d�ƶ���λ��%d\r\n",\
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
{//������п���
	u8 tmp;
	switch(Var.BSPwork.Stu)
	{//����״̬
		case SYSINIT:
		{//��ʼ��������ԭ��
			if(Var.Motor1.InitDelay){return;}//������ʱ
			
			tmp = MotorSerchZero();//�����
			if(1 == tmp )
			{//�ҵ�ԭ��
				Var.Motor1.SetPulse =0;
				Var.Motor1.Pulse =0;
				Var.Motor1.SetPoint =0;
				Var.Motor1.CurPoint =0;
				Var.BSPwork.Stu = SYSIDLE;
				printf("<MotorRun>����ҵ����,���뵽IDLEģʽ��\r\n");
			}
			else if(2 == tmp)
			{//��ԭ��ʧ��
				StopMotor();
				Var.BSPwork.Stu = SYSEROR;
				printf("<MotorRun>�����ԭ��ʧ�ܣ��������������˽��뵽SYSERORģʽ��\r\n");
			}
			
			if(Var.Motor1.Limit == 0 )
			{//������ֹ�ź�
				StopMotor();
				Var.BSPwork.Stu = SYSEROR;
				printf("<MotorRun>�������λ�źţ�,����λ����������㡣\r\n");
			}
			break;
		}
		case SYSIDLE:
		case KEYSTOP:
		case KEYRUN:
		case KEYSET:
		{//�ȴ�����ָ��,���е�ָ��λ��
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
  HAL_TIM_OC_Start_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// ʹ���ж� �رձȽ����
}


void StopMotor(void) 
{
	Var.Motor1.Pu =0;
	TIM_CCxChannelCmd(STEPMOTOR_TIMx,STEPMOTOR_TIM_CHANNEL_x,TIM_CCx_DISABLE);
  HAL_TIM_OC_Stop_IT(&htimx_STEPMOTOR,STEPMOTOR_TIM_CHANNEL_x);// ʹ���ж� �رձȽ����
	
	//HAL_TIM_Base_Stop(&htimx_STEPMOTOR);
}

