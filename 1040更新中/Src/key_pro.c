#include "key_pro.h"
#include "key.h"
#include "function.h"

void KeyPro(void)
{//按键处理
	u8 tmp;
	te_PRON_ID tp;
	tmp = bsp_GetKey();

	switch(Var.BSPwork.Stu)
	{//运行状态
		case SYSINIT:
		{//FF,初始化按键不起作用
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//测试原点初始化
				
				printf("按下复位键！\r\n");
			}
			break;
		}
		case SYSIDLE:
		{//2,初始化完毕自动进入IDLE
			if((KEY_DOWN_START == tmp)|| (KEY_DOWN_K1 == tmp) ||(PC_START == Var.Pc.Key))
			{//开始按键
				Var.BSPwork.Delay =3;
				Var.Pc.Key=0;
				Var.BSPwork.RunPro = Var.Pc.WaitPro;
				tp = GetProOrder(&Var.BSPwork.RunPro);
				if(tp)
				{
					RunProId(tp);
					Var.BSPwork.Stu = KEYSET;		//进入设置运行参数	
					printf("开始键按下，3秒后执行程序\r\n");
				}
			}
			
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//复位
				Var.Motor1.SerchZero =1;
				
				Var.BSPwork.Stu = SYSINIT;
				printf("按下复位键，重新找寻零点\r\n");
			}
			break;
		}
		case KEYRUN:
		{//5，运行状态
			if(KEY_DOWN_START == tmp || KEY_DOWN_K1 == tmp)
			{//暂停运行
				Var.Pc.Key = 0;
				Var.BSPwork.Stu = KSYSHOLD;
				printf("程序暂停运行\r\n再次按下恢复程序继续运行\r\n");
			}
			
			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || PC_STOP == Var.Pc.Key)
			{//停止按键
				
				Var.BSPwork.Stu = KEYSTOP;		//进入设置运行参数

				printf("程序停止运行\r\n");				
			}
			break;
		}
		case  KSYSHOLD:
		{//暂停保持
			if(KEY_DOWN_START == tmp || KEY_DOWN_K1 == tmp)
			{//继续运行
				Var.BSPwork.SecCnt=10;
				Var.BSPwork.Stu = KEYRUN;
				printf("开始继续运行\r\n");
			}

			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || (PC_STOP == Var.Pc.Key))
			{//停止按键
				
				Var.Pc.Key =0;
				
				Var.BSPwork.Stu = KEYSTOP;		//进入设置运行参数
			
				printf("\r\n运行停止程序\r\n");
			}
			
			break;
		}		
		case DOORHOLD:
		{
			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || (PC_STOP == Var.Pc.Key))
			{//停止按键
				
				Var.Pc.Key =0;
				
				Var.BSPwork.Stu = KEYSTOP;		//进入设置运行参数
			
				printf("\r\n运行停止程序\r\n");
			}
			break;
		}
		case SYSEROR:
		{//运行中出现问题
			
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//重新找零
				printf("按下复位键\r\n");
				Var.Motor1.SerchZero =1;
				Var.BSPwork.Stu = SYSINIT;
			}
			break;
		}
		default :break;
	}
}
