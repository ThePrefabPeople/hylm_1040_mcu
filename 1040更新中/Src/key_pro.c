#include "key_pro.h"
#include "key.h"
#include "function.h"

void KeyPro(void)
{//��������
	u8 tmp;
	te_PRON_ID tp;
	tmp = bsp_GetKey();

	switch(Var.BSPwork.Stu)
	{//����״̬
		case SYSINIT:
		{//FF,��ʼ��������������
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//����ԭ���ʼ��
				
				printf("���¸�λ����\r\n");
			}
			break;
		}
		case SYSIDLE:
		{//2,��ʼ������Զ�����IDLE
			if((KEY_DOWN_START == tmp)|| (KEY_DOWN_K1 == tmp) ||(PC_START == Var.Pc.Key))
			{//��ʼ����
				Var.BSPwork.Delay =3;
				Var.Pc.Key=0;
				Var.BSPwork.RunPro = Var.Pc.WaitPro;
				tp = GetProOrder(&Var.BSPwork.RunPro);
				if(tp)
				{
					RunProId(tp);
					Var.BSPwork.Stu = KEYSET;		//�����������в���	
					printf("��ʼ�����£�3���ִ�г���\r\n");
				}
			}
			
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//��λ
				Var.Motor1.SerchZero =1;
				
				Var.BSPwork.Stu = SYSINIT;
				printf("���¸�λ����������Ѱ���\r\n");
			}
			break;
		}
		case KEYRUN:
		{//5������״̬
			if(KEY_DOWN_START == tmp || KEY_DOWN_K1 == tmp)
			{//��ͣ����
				Var.Pc.Key = 0;
				Var.BSPwork.Stu = KSYSHOLD;
				printf("������ͣ����\r\n�ٴΰ��»ָ������������\r\n");
			}
			
			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || PC_STOP == Var.Pc.Key)
			{//ֹͣ����
				
				Var.BSPwork.Stu = KEYSTOP;		//�����������в���

				printf("����ֹͣ����\r\n");				
			}
			break;
		}
		case  KSYSHOLD:
		{//��ͣ����
			if(KEY_DOWN_START == tmp || KEY_DOWN_K1 == tmp)
			{//��������
				Var.BSPwork.SecCnt=10;
				Var.BSPwork.Stu = KEYRUN;
				printf("��ʼ��������\r\n");
			}

			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || (PC_STOP == Var.Pc.Key))
			{//ֹͣ����
				
				Var.Pc.Key =0;
				
				Var.BSPwork.Stu = KEYSTOP;		//�����������в���
			
				printf("\r\n����ֹͣ����\r\n");
			}
			
			break;
		}		
		case DOORHOLD:
		{
			if(KEY_DOWN_STOP == tmp || KEY_DOWN_K2 == tmp || (PC_STOP == Var.Pc.Key))
			{//ֹͣ����
				
				Var.Pc.Key =0;
				
				Var.BSPwork.Stu = KEYSTOP;		//�����������в���
			
				printf("\r\n����ֹͣ����\r\n");
			}
			break;
		}
		case SYSEROR:
		{//�����г�������
			
			if(KEY_DOWN_RESET == tmp || KEY_DOWN_K3 == tmp)
			{//��������
				printf("���¸�λ��\r\n");
				Var.Motor1.SerchZero =1;
				Var.BSPwork.Stu = SYSINIT;
			}
			break;
		}
		default :break;
	}
}
