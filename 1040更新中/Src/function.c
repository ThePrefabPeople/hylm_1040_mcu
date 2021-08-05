#include "function.h"
#include "main.h"
#include "usart.h"
#include "stdlib.h"
#include "bsp_STEPMOTOR.h"
#include "bsp_spiflash.h"
#include "motor.h"
#include "bsp_usartx.h"
#include "string.h"
#include "bsp_beep.h"
#include "bsp_AD5689.h"
#include "key_pro.h"
#include "bsp_AD7190.h"
#include "cmd_queue.h"
#include "hmi_driver.h"
#include "bsp_gpio.h"
void Uart5RecPro(void);

void TestRWFlsh(u32 id)
{
	u32 tmp =id;
	u8 Rx_Buffer[100];
	u8	write_buf[]={"12345667890"};
	u32 write_addr=4096*1;
	if (tmp == SPI_FLASH_ID)//  sFLASH_ID  0XEF4018
	{	
		printf("��⵽�����flash W25Q128 !\n");
		
		/* ����SPI��������д�� */
		SPI_FLASH_SectorErase(write_addr);	 	 
		
		/* �����ͻ�����������д��flash�� */ 	
		SPI_FLASH_BufferWrite(write_buf, write_addr, sizeof(write_buf));
		
		/* ���ո�д������ݶ������ŵ����ջ������� */
		SPI_FLASH_BufferRead(Rx_Buffer, write_addr, sizeof(write_buf));
		printf("����������Ϊ:%s\n", Rx_Buffer);
		
		/* ���д�������������������Ƿ���� */
		if(Buffercmp(write_buf, Rx_Buffer, sizeof(write_buf)))
			printf("���д���������������������\r\n");
			
	}
}
void FlashInfo(void)
{//���FLASH��������Ϣ
	
	u32 tmp[2];
	tmp[0] = SPI_FLASH_ReadDeviceID();
  HAL_Delay(100);
	tmp[1] = SPI_FLASH_ReadID();
	printf("FLASH����ID:0X%X,0X%X\n",tmp[0],tmp[1]);
	
	//TestRWFlsh(tmp[1]);
}

void 	ParmInit(void)
{
	u32 ct;

	//��ȡ�������
	ct = sizeof(ts_SYSRUN)*PROGRAM_NUM;
	memset(Var.Flash.RWBuf,0,ct);	
	SPI_FLASH_BufferRead(Var.Flash.RWBuf,SYSPRON_INIT*4096,ct);
	memcpy(Var.EePro,Var.Flash.RWBuf,ct);
	for(ct =0; ct < PROGRAM_NUM;ct++)
	{
		if(1 == Var.EePro[ct].PronValid)
		{
			Var.Pc.ValidPro |= (1<< ct);
			printf("����ID:%3d,��Ч��־:%3d,�ܲ���:%3d,������ʼλ��:%3d,����λ��:%3d,ѭ������:%3d,B������:%3d,�Ƿ���:%3d;\r\n",\
							Var.EePro[ct].Pron,\
							Var.EePro[ct].PronValid,\
							Var.EePro[ct].TotalStep,\
							Var.EePro[ct].StartId,\
							Var.EePro[ct].EndId,\
							Var.EePro[ct].CycTime,\
							Var.EePro[ct].BNum,\
							Var.EePro[ct].VerFb);
		}
	}
	
	//��ȡ������
	SPI_FLASH_BufferRead((u8 *)&Var.EParam, SYSPRON_RUNK *4096, sizeof(ts_EPARAM));
	Var.Pc.WaitPro = Var.EParam.ProRunId;
	
	printf("ԭ���������壺%d\r\n",Var.EParam.Pulse);

}

void FlashWritePro(void)
{//дFLASH 
	u16 tmp;
	if(1 == Var.Flash.UpFlag)
	{//д�������������������ݺ󱣴�
		
		Var.Flash.UpFlag =0;

		tmp = sizeof(ts_SYSRUN)*PROGRAM_NUM;
		SPI_FLASH_SectorErase(0);	 	 
		SPI_FLASH_BufferWrite((u8 *)Var.EePro, 0, tmp);
		SPI_FLASH_BufferRead(Var.Flash.ReadBuf, 0, tmp);
		if(Buffercmp(Var.Flash.ReadBuf, (u8 *)Var.EePro,tmp))
		{
			printf("д��������������ȷ��\n");
		}
		
		tmp = Var.Flash.WriteNum;
		SPI_FLASH_SectorErase(Var.Flash.Sector*4096);	 	 
		SPI_FLASH_BufferWrite(Var.Flash.RWBuf, Var.Flash.Sector *4096, tmp);
		SPI_FLASH_BufferRead(Var.Flash.ReadBuf, Var.Flash.Sector *4096, tmp);
		if(Buffercmp(Var.Flash.RWBuf, Var.Flash.ReadBuf,tmp))
		{
			BEEP_ON;HAL_Delay(200);BEEP_OFF;
			printf("д���������������ȷ��\n");
		}
	}
	
	if(1 == Var.Flash.CaFlag)
	{//д�����в���
		Var.Flash.CaFlag = 0; 	
		
		tmp = sizeof(ts_EPARAM);
		SPI_FLASH_BufferRead(Var.Flash.ReadBuf, SYSPRON_RUNK *4096, tmp);
		
		if(FAILED == Buffercmp(Var.Flash.ReadBuf,(u8 *)&Var.EParam,tmp))
		{
			SPI_FLASH_SectorErase(SYSPRON_RUNK*4096);	 
			printf("��������������%d\r\n",Var.EParam.Pulse);
			SPI_FLASH_BufferWrite((u8 *)&Var.EParam, SYSPRON_RUNK *4096, tmp);
		}
	}
	
}
//u8 Var.Parm[1024];
//[0]	�����
//[1][2]ʱ�䣬��λ��ǰ
//[3] ����
//[4]���ع���վ�����ظ�ѹ���Ƿ���Ľ���λ��
//[5]����λ��
//[6]��Һλ��
//[7][8]��ѹ
//[9][10]����
void OutIo(u8 *p)
{
	//С������
//	typedef enum
//{//���еĳ�����
//Y00-Y03
//	IO_BIT2=2,IO_BIT5=5, IO_BIT4=4,IO_BIT7=7,
	
//Y04-Y05
//	IO_BIT1=1,IO_BIT0=0, IO_BIT3=3,IO_BIT6=6,
//} te_IO_BIT;
	u8 tmp[2];

	memcpy(tmp,p,2);
	//IO ���
	if(tmp[0]& (0x01 << IO_BIT2))Var.Out.Y00 =1;else Var.Out.Y00 =0;
	if(tmp[0]& (0x01 << IO_BIT5))Var.Out.Y01 =1;else Var.Out.Y01 =0;
	if(tmp[0]& (0x01 << IO_BIT4))Var.Out.Y02 =1;else Var.Out.Y02 =0;
	if(tmp[0]& (0x01 << IO_BIT7)){Var.Out.Y03 =1;Var.Out.Y07 =1;}else{ Var.Out.Y03 =0;Var.Out.Y07 =0;}
	
	if(tmp[0]& (0x01 << IO_BIT1))Var.Out.Y04 =1;else Var.Out.Y04 =0;
	if(tmp[0]& (0x01 << IO_BIT0))Var.Out.Y05 =1;else Var.Out.Y05 =0;
	
	//����վ,�ϱ���
	if(tmp[0]& (0x01 << IO_BIT3))Var.Out.Y13 =1;else Var.Out.Y13 =0;
	//�ⲿ�ź�
	if(tmp[0]& (0x01 << IO_BIT6))Var.Out.Y14 =1;else Var.Out.Y14 =0;

//���������	
//	if(tmp[1]& (0x01 << 0))Var.Out.Y10 =1;else Var.Out.Y10 =0;
//	if(tmp[1]& (0x01 << 1))Var.Out.Y11 =1;else Var.Out.Y11 =0;
//	if(tmp[1]& (0x01 << 2))Var.Out.Y12 =1;else Var.Out.Y12 =0;
//	if(tmp[1]& (0x01 << 3))Var.Out.Y13 =1;else Var.Out.Y13 =0;
//	if(tmp[1]& (0x01 << 4))Var.Out.Y14 =1;else Var.Out.Y14 =0;
//	if(tmp[1]& (0x01 << 5))Var.Out.Y15 =1;else Var.Out.Y15 =0;
//	if(tmp[1]& (0x01 << 6))Var.Out.Y16 =1;else Var.Out.Y16 =0;
//	if(tmp[1]& (0x01 << 7))Var.Out.Y17 =1;else Var.Out.Y17 =0;
	

}
void SetStepParam(u8 *p)
{//�������
	u8 tmp[16]={0};
	u8 step;
	u8 io[2]={0};
	u8 sw;
	u16 voltage;
	
	memcpy(tmp,p,16);
	
	step = tmp[0];
	step = step;
	
	Var.BSPwork.Cursec=0;		
	Var.BSPwork.SecCnt=10;
	
	Var.BSPwork.NextStep =1;
	
	Var.BSPwork.TotalSec = tmp[1];
	Var.BSPwork.TotalSec <<=8;
	Var.BSPwork.TotalSec += tmp[2];//�趨ʱ��
	
	io[0] = tmp[3];
	OutIo(io);
	
	voltage = tmp[7];
	voltage <<=8;
	voltage += tmp[8];//�趨��ѹ
	if(voltage)
		Var.Power.SetVoltage = voltage;
//	if(0 == voltage)Var.Power.SetCur =0;
//	if(0 == Var.Power.SetVoltage)  Var.Out.Y06 =0;
	
	sw = tmp[4]%10;
	printf("<SetStepParam>����״̬:%d,�趨��ѹ:%d\r\n",sw,Var.Power.SetVoltage);
	switch(sw)
	{
		case 1: { Var.Out.Y06 =1;break; }//����ѹ
		case 2: { Var.Out.Y06 =0;break; }//����ѹ
		case 3: { Var.Out.Y13 =1; break; }//������վ
		case 4: { Var.Out.Y13 =0; break; }//�ع���վ
		case 0:
		{
			if(tmp[5])
			{
				Var.Motor1.SetPoint = tmp[5]-1;
				Var.BSPwork.NextStep =0;
			}
			break;
		}
		case 5: 
		{//���Ľ�����λ�ã�����
			Var.Motor1.SetPoint = Var.BSPwork.PointId;
			Var.BSPwork.NextStep =0;
			break; 
		}
		case 6:
		{//����ѹ��ʱ������վ,��ʱ����
			Var.Out.Y06 =1;
			Var.Recorder.IOF =1; Var.Recorder.DeCnt =5;
			break;
		}
		default :break;
	}

	
	//Var.Power.SetVoltage = tmp[9];
	//Var.Power.SetVoltage <<=8;
	//Var.Power.SetVoltage += tmp[10];//�趨����
	
}

void RunProId(te_PRON_ID id)
{//����ָ������
	Var.ProId =	id;
	Var.RunPro = Var.EePro[Var.ProId-1];
	
	Var.BSPwork.PointId = Var.RunPro.StartId-1;
	Var.BSPwork.CycTime =1;
	Var.BSPwork.Curstep =1;
	Var.BSPwork.New =0;
}

	
u8 StopPro(void)
{//ֹͣ����
	static u8 st=0;
	switch(st)
	{
		case 0:
		{//�رո�ѹ
			if(Var.BSPwork.StopCnt ==0)
			{
				if(1 == Var.Out.Y06)
				{
					Var.Out.Y06=0;
					Var.Power.SetVoltage =0;
					Var.BSPwork.StopCnt=1;
				}
				printf("%d\r\n",st);
				st++;
			}
			break;
		}
		case 1:
		{//�ر�����
			if(Var.BSPwork.StopCnt ==0)
			{
				if(1 == Var.Out.Y00 || 1 == Var.Out.Y01 || 1 == Var.Out.Y02)
				{
					Var.Out.Y00=0;				
					Var.Out.Y01=0;
					Var.Out.Y02=0;
					Var.BSPwork.StopCnt=2;
					printf("%d\r\n",st);
					st++;
				}
				else st=3;
			}
			
			break;
		}
		case 2:
		{//��������
			if(Var.BSPwork.StopCnt ==0)
			{
				if(0 == Var.Out.Y03 || 0 == Var.Out.Y07)
				{
					Var.Out.Y03=1;		
					Var.Out.Y07=1;		
					Var.BSPwork.StopCnt=2;
				}
				printf("%d\r\n",st);
				st++;
			}
			break;
		}
		case 3:
		{//�ر���������
			if(Var.BSPwork.StopCnt ==0)
			{
				Var.Out.Y00=0;
				Var.Out.Y01=0;
				Var.Out.Y02=0;
				Var.Out.Y03=0;
				Var.Out.Y04=0;
				Var.Out.Y05=0;
				Var.Out.Y06=0;
				Var.Out.Y07=0;
				//Var.Motor1.SetPoint =0;
				st=0;
				printf("%d\r\n",st);
				return 1;
			}
			break;
		}
		default:st =0;break;
	}
	return 0;
}
void WorkPro(void)
{
	static u8 run_data[16]={0};
	te_PRON_ID tmp;
	u8 tp;
	switch(Var.BSPwork.Stu)
	{
		case KEYSTOP:
		{
			tp = StopPro();
			if(1 == tp) 
			{	
				Var.BSPwork.Stu = SYSIDLE;
				printf("<WorkPro>ֹͣ����ִ����Ͻ���SYSIDLE\r\n");
			}
			break;
		}
		case KEYRUN:
		{//����״̬
			if(0 == Var.BSPwork.SecCnt)
			{//1��ִ��һ��
				Var.BSPwork.SecCnt=10;
				Var.BSPwork.Cursec++;	
				if((Var.BSPwork.Cursec >= Var.BSPwork.TotalSec) && (1 == Var.BSPwork.NextStep)) 
				{
					Var.BSPwork.New=1;
				}
				
				printf("<WorkPro>ʱ�䣺%d\r\n",Var.BSPwork.Cursec);
				
				if(1==Var.BSPwork.New) 
				{//�²���
					Var.BSPwork.New=0;
					Var.BSPwork.Curstep++;
					Var.BSPwork.Stu = KEYSET;
					
				}
				
				if(Var.BSPwork.Curstep > Var.RunPro.TotalStep)  
				{//һ������ִ����� 
					Var.BSPwork.CycTime++;//ѭ��������һ	
					Var.BSPwork.Curstep =1;//�������´�ѭ��
				}
				
				if(Var.BSPwork.CycTime > Var.RunPro.CycTime)
				{//һ��λ���ϵ�ѭ������
					Var.BSPwork.PointId++;	//��һ������λ��
					Var.BSPwork.CycTime =1;	//����ѭ������
					
				}	
				
				if(Var.BSPwork.PointId >= Var.RunPro.EndId)
				{//�����¸����г���
						tmp = GetProOrder(&Var.BSPwork.RunPro);
						if(tmp)
						{
							RunProId(tmp);
							Var.BSPwork.Stu = KEYSET;	
							Var.BSPwork.Delay =3;
							printf("\r\n<WorkPro>��ʼ���г���%d������ʼ����ͣ����ֹͣ����ֹ����\r\n",Var.ProId);
						}
						else
						{
							Var.BSPwork.Stu = KEYSTOP;		//�����������в���	
							printf("\r\n<WorkPro>���н�������\r\n");
						}
				}
				
			}
			break;
		}
		case KEYSET:
		{//��ȡ�������ò���
			if(Var.BSPwork.Delay)break;
			
			memset(run_data,0,16);
			SPI_FLASH_BufferRead(run_data,((Var.RunPro.Pron+1)*4096+(Var.BSPwork.Curstep)*16),16);
	
			SetStepParam(run_data);
			
			Var.BSPwork.Stu = KEYRUN;
				
			if(Var.BSPwork.Curstep ==1)
				printf("\r\n<WorkPro><<<<����%d��%d��;��%d��%d����;ÿ���ظ�%d��;��ǰ��%d��ѭ����%d��>>>>>\r\n",\
																							Var.RunPro.Pron,\
																							Var.RunPro.TotalStep,\
																							Var.RunPro.StartId,\
																							Var.RunPro.EndId,\
																							Var.RunPro.CycTime,\
																							Var.BSPwork.PointId+1,\
																							Var.BSPwork.CycTime);
			printf("<WorkPro><<��%d����ʱ�䣺%d ���λ��s%d,c%d>>\r\n",\
																					Var.BSPwork.Curstep,\
																					Var.BSPwork.TotalSec,\
																					Var.Motor1.SetPoint,\
																					Var.Motor1.CurPoint);
			break;
		}
		default :break;
	}
}


void Uart1Pro(void)	
{//���������Ϣ
	char *p;
	u8 a,b;
	u8 io[2]={0};
	u16 tmp;
	
	if(1 == Var.Com5.Busy)
	{
		Var.Com5.Busy=0;
		printf("<Uart1Pro>%s",Var.Com5.Buf);
		p = strstr((char *)Var.Com5.Buf,"~~~~");
		if(NULL != p)
		{
			switch((p[4]))
			{
				case '1':
				{//���Ե���ƶ�
					p+=strlen("~~~~1");
					tmp = atoi(p);
					Var.Motor1.SetPoint = tmp;
					break;
				}			
				case '2':
				{//����IO
					p+=strlen("~~~~2");
					a = atoi(p);
					p = strstr(p,",");
					b = atoi(&p[1]);
					if(b)
					{
						if(a < 8 ) io[0] |= (1<<a);
						else io[1] |= (1<<(a-8));
					}
					else	
					{
						if(a < 8 ) io[0] &= ~(1<<a);
						else io[1] &= ~(1<<(a-8));
					}
					OutIo(io);
					printf("<Uart1Pro>a:%d,b:%d,io[0]:%x,io[1]:%x\r\n",a,b,io[0],io[1]);
					break;
				}
				case '3':
				{//���������
					p+=strlen("~~~~3");
					tmp = atoi(p);
					
					printf("<Uart1Pro>����ԭ������ֵ��%d����Ϊ%d\r\n",Var.Motor1.CalibrationPulse,tmp);
					Var.Motor1.CalibrationPulse = tmp;
					Var.Motor1.SerchZero =1;
					break;
				}					
				case '4':
				{//���ȷ��
					Var.Flash.CaFlag =1;
					Var.EParam.Pulse = Var.Motor1.CalibrationPulse;
					break;
				}	
				case '5':
				{//���汾��
					printf("<Uart1Pro>�汾�ţ�%s\r\n",VERSION);
					break;
				}			
				case '6':
				{
					p+=strlen("~~~~6");
					tmp = atoi(p);//������
					Var.Power.SetVoltage = tmp;
					break;
				}
				case '7':
				{
					p+=strlen("~~~~7");
					tmp = atoi(p);//������
					if(tmp ==1)Var.Out.Y06=1;
					else if(tmp ==2) Var.Out.Y06=0;
					break;
				}
				
				default :break;
			}
		}
		memset(Var.Com5.Buf,0,100);
	}
}


void Uart5Pro(void)
{//���ڲ����������
	
	if(1 == Var.Com5Base.bDevRcvBuf)   
	{//����0�յ�����
		Uart5RecPro();
		Var.Com5Base.bDevRcvBuf=0;
				
		Var.Com5.PC_connect = PCWR_OK;//����Խ�������
		Var.Com5.PC_connto = PCWR_OT;	//20�볬ʱ��������ʼ��
	}
	
	if(0 == Var.Com5.SecCnt)	
	{//���ڼ��1��ִ��һ��
		Var.Com5.SecCnt=1;
		
		if(Var.Com5.PC_connect==PCWR_OK) 
		{//�Ѿ�����
			if(Var.Com5.PC_connto) Var.Com5.PC_connto--;
			else Var.Com5.PC_connect=PCWR_ERR;  //����û�����ӣ������Ѿ��Ͽ�
		}
		
		if(Var.Com5.Uart0_to) Var.Com5.Uart0_to--;
		
		if(Var.Com5.PCwr_start==PCWR_OK) 
		{//��ʼдtxt
			if(Var.Com5.PCwr_done==PCWR_OK)
			{
				Var.Com5.PCwr_done=PCWR_ERR;
				Var.Com5.PCwr_start=PCWR_ERR;
			}
			else 
			{
				if(0==Var.Com5.Uart0_to)
				{
					Var.Com5.PCwr_done=PCWR_ERR;
					Var.Com5.PCwr_start=PCWR_ERR; 
				}
			}
		}
	}
}

//[0]	�����
//[1][2]ʱ�䣬��λ��ǰ
//[3] ����
//[4]���ع���վ�����ظ�ѹ���Ƿ���Ľ���λ��
//[5]����λ��
//[6]��Һλ��
//[7][8]��ѹ
//[9][10]����
void Debug(u8 *d)
{//
	u8 sw;
	u8 io[2]={0};
	u16	pulse;
	sw = d[3];
	switch(sw)
	{
		case M0_WRACT:        
		{//0x10 
			break;
		}
		case M0_RDACT:          		
		{//0x11
			break;
		}
		case M0_RSTAB:         //0x12
		{
			break;
		}
		case M0_SETMOVE:       //0x13
		{
			break;
		}
		case M0_WASTE_ZERO:    //0x14
		{
			break;
		}
		case M0_INLET_ZERO:
		{//0x15,
			Var.Motor1.SerchZero =1;
			Var.BSPwork.Stu = SYSINIT;
			
			//�������
			if(Var.EParam.Pulse != Var.Motor1.CalibrationPulse)
			{
				Var.Flash.CaFlag =1;
				Var.EParam.Pulse = Var.Motor1.CalibrationPulse;
			}
			
			break;
		}
		case M0_INLET_DEBUG:   
		{//0x16,���Խ�����λ��
			//			[3],		[4],			[5]
			//			����	�����λ		�����λ
			pulse = d[4];
			pulse <<= 8;
			pulse |= d[5];
			Var.Motor1.CalibrationPulse = pulse;
			Var.Motor1.SerchZero =1;
			Var.BSPwork.Stu = SYSINIT;
			break;
		}	
		case M0_WASTE_DEBUG:   
		{//0x17 ���Է�Һ��λ��
			break;
		}	
		case INLET_MOVE_TO:    
		{//0X23
			if(d[4])
			{
				printf("<Debug>�ƶ������%d\r\n",d[4]-1);
				
				Var.Motor1.SetPoint = d[4]-1;//waste move to addr
			}
			break;
		}
		case WASTE_MOVE_TO:   //0X24
		{
			break;
		}
		case M0_VOTAGE_DEBUG:  //0x18
		{
			break;
		}
		case M0_VOTAGE_SET:    //0x19
		{
			break;
		}
		case M0_BO_DEBUG:      //0x21
		{
			break;
		}
		case M0_FAMEN_DEBUG:   
		{//0x22  [3],		[4],			[5]
			//			����	оƬλ��	״̬
			io[0] = d[5];
			printf("<Debug>IO=%X\r\n",d[5]);
			OutIo(io);
			break;
		}
		default :break;
	}
}

//����0 ���մ�����
//[0]	�����
//[1][2]ʱ�䣬��λ��ǰ
//[3] ����
//[4]���ع���վ�����ظ�ѹ���Ƿ���Ľ���λ��
//[5]����λ��
//[6]��Һλ��
//[7][8]��ѹ
//[9][10]����
//ʱ�䣬���Ÿߣ����ŵͣ�����λ�ã���Һλ�ã���ѹ������
void Uart5RecPro(void)
{//20 00 cmd len xor 0d 0a
  u8 bufwr[17]={0};
  char rec_cmd;
  u8 rec_pron;
  u8 rec_curstep;
  u8 bufsendlen=0;
  u8 stu_wract;
  u8 bufuse=0;
   //Var.Com5Base.RcvBuf[0]==addr;
	rec_cmd=Var.Com5Base.RcvBuf[1];
	switch(rec_cmd)
	{   
		case PC_CONNECT:
		case PC_RDSTUM0: //03
		case PC_SETMOV:  //05	 
		case PC_RDACTM0:
		case PC_SET_PARA:
		{//��ȡ״̬
			bufuse=1;
			break;
		}
		case PC_SETPRO:
		{//���ڰ���
			Var.Pc.Key 		 = Var.Com5Base.RcvBuf[3];
			if(Var.Pc.Key == PC_STOP)break;
			Var.Pc.WaitPro = Var.Com5Base.RcvBuf[4];	
			Var.EParam.ProRunId = Var.Pc.WaitPro;
			Var.Flash.CaFlag=1;
			bufuse=1;
			break;
		}
		case PC_BRIDGE:
		{//06 ��������Ͳ���
			//3,22,4,1
			Debug(Var.Com5Base.RcvBuf);
			bufuse=1;
			break;
		}
		case PC_WRACTM0:
		{//�������
			rec_pron =   Var.Com5Base.RcvBuf[3];  //�����  PROA=1
			rec_curstep= Var.Com5Base.RcvBuf[4];  //��ǰ�����
			
			if(rec_pron < PROGRAM_NUM)
			{
				Var.Com5.Uart0_to = PCWR_OT;//20s��ʱ����ش���
				if(0==rec_curstep)
				{ //��0������  
					printf("<Uart5RecPro>�������:%d\r\n",rec_pron);
					
					Var.EePro[rec_pron].Pron 			=Var.Com5Base.RcvBuf[3]; 		//������
					Var.EePro[rec_pron].PronValid	=PRON_INIT; 								//������Ч��ʶ
					Var.EePro[rec_pron].BNum			=Var.Com5Base.RcvBuf[5];   	//���ư�B����
					Var.EePro[rec_pron].TotalStep	=Var.Com5Base.RcvBuf[6];   	//�ܲ�����
					Var.EePro[rec_pron].VerFb			=Var.Com5Base.RcvBuf[7];  	//����Ƿ���
					Var.EePro[rec_pron].StartId		=Var.Com5Base.RcvBuf[8];  	// ������ʼλ��
					Var.EePro[rec_pron].EndId			=Var.Com5Base.RcvBuf[9];  	// ��������λ��
					Var.EePro[rec_pron].CycTime		=Var.Com5Base.RcvBuf[10];		//ÿ����Ʒ�ظ�����  
					
					//���Ƶ����ջ�����
					memcpy(&Var.Parm[rec_curstep*16],Var.Com5Base.RcvBuf,16);
					
					Var.PCPro = Var.EePro[rec_pron];	
					Var.Com5.PCwr_start=PCWR_OK;
				}
				else if(PRON_INIT == Var.PCPro.PronValid && rec_pron == Var.PCPro.Pron)
				{
					if(rec_curstep <= Var.PCPro.TotalStep)
					{ 
						memcpy(bufwr,&Var.Com5Base.RcvBuf[4],RECORD_USELEN); //������Ч���ݵ����ջ�����
						memcpy(&Var.Parm[rec_curstep*16],bufwr,16);
						stu_wract =1;
					}	
					
					if(rec_curstep == Var.PCPro.TotalStep)	
					{
							Var.Flash.UpFlag =1;
							Var.Flash.WriteNum = (rec_curstep+1)*16;
							Var.Flash.Sector = rec_pron+1;
							memcpy(Var.Flash.RWBuf,Var.Parm,Var.Flash.WriteNum);
							
							Var.EePro[rec_pron].PronValid=PRON_VALID;//����д����Ч
							Var.Pc.ValidPro |= (1<< rec_pron);
							
							Var.PCPro.PronValid=PRON_UNVALID;    
							Var.PCPro.Pron=0xff;
							
							Var.Com5.PCwr_done=PCWR_OK; 					
					}			
				}   
			}
			Var.AnPc.datalen=2;
			Var.AnPc.dataBuf[0]=Var.BSPwork.Stu;	//����״̬
			Var.AnPc.dataBuf[1]=stu_wract; 			//1,��ʾд��ɹ� 0 ʧ��    
			bufuse=2;
			break;
		} 
		default:break;
	}
	
	if(1==bufuse)//answer PC
	{
		Var.AnPc.datalen=6;
		Var.AnPc.dataBuf[0]	=		Var.BSPwork.Stu;
		Var.AnPc.dataBuf[1]	=		Var.run_pro_bit;
		Var.AnPc.dataBuf[2]	= 	Var.RunPro.Pron;
		Var.AnPc.dataBuf[3]	= 	Var.Pc.ValidPro;
		Var.AnPc.dataBuf[4]	= 	Var.BSPwork.Cursec>>8;
		Var.AnPc.dataBuf[5]	=		Var.BSPwork.Cursec &0xff;
		Var.AnPc.addr				=		ADDR_MAIN;       
		Var.AnPc.cmd				=		CMD_ANSWER(rec_cmd);
		
		bufsendlen=MsgtoPCUart0(&Var.AnPc,Var.Com5Base.devXmtBuf);
		send0_buf(Var.Com5Base.devXmtBuf,bufsendlen);                    
	}
	else if(2==bufuse)
	{//д�������
		Var.AnPc.addr = ADDR_MAIN;       
		Var.AnPc.cmd  = CMD_ANSWER(rec_cmd);
			
		bufsendlen = MsgtoPCUart0(&Var.AnPc,Var.Com5Base.devXmtBuf);	
		send0_buf(Var.Com5Base.devXmtBuf,bufsendlen);
	}
}

u8 chsxor(const u8 *data)
{//���У��������
	u8 len;
	char crcvalue=0x00;
	
	data++;
	data++; 
	crcvalue=*data++;
	len=1+(*data);
	while(len--) crcvalue^=*data++;
	
	return crcvalue;	
}
//return datalen 
//20 01 12 00 12 0D 0A 
//start + address + command + datalen + databuf + chs + 0x0D 0x0A
u8 MsgtoPCUart0(ts_MSG_COM5* msg_rs,u8 *wrbuf)
{
	u8 tmpi;
	u8 tmplen;
	u8 chs_xor;
	u8 *chsbuf;
	chsbuf=(u8 *)wrbuf;  //use pointer init
	*wrbuf++=(u8)DEV_RCV_SYNC;  //' '
	*wrbuf++=msg_rs->addr;
	*wrbuf++=(msg_rs->cmd);
	*wrbuf++=msg_rs->datalen;
	tmplen=msg_rs->datalen;
	if(tmplen)
	{
		for(tmpi=0;tmpi<tmplen;tmpi++)
		{
			
			*wrbuf++=msg_rs->dataBuf[tmpi];
		}  
	}
	//wrbuf pointer has changed,so you need use wrbuf value 
	//function pointer parameter: buf[0] 
	chs_xor=chsxor(chsbuf);
	
	*wrbuf++=chs_xor;
	*wrbuf++=(u8)DEV_RCV1_END0;
	*wrbuf++=(u8)DEV_RCV1_END1;
	return (tmplen+UART1_MINLEN);
}

//recdata �� ==1 �Ӵ���1���յ�������    ==0 û���յ�����
//����1���ʹ�����
//[0]	�����
//[1][2]ʱ�䣬��λ��ǰ
//[3] ����
//[4]���ع���վ�����ظ�ѹ���Ƿ���Ľ���λ��
//[5]����λ��
//[6]��Һλ��
//[7][8]��ѹ
//[9][10]����

te_PRON_ID GetProOrder(u8 *d)
{
	u8 ct;
	for(ct=0;ct<8;ct++)
	{
		if(*d & (1<<ct))
		{
			*d &= ~(1<<ct);
			switch(ct)
			{
				case 0: return SYSPRON_RUNA;
				case 1: return SYSPRON_RUNB;
				case 2: return SYSPRON_RUNC;
				case 3: return SYSPRON_RUND;
				case 4: return SYSPRON_RUNE;
				case 5: return SYSPRON_RUNF;
				case 6: return SYSPRON_RUNG;
				case 7: return SYSPRON_RUNH;
				default :break;
			}
		}
	}
	return SYSPRON_INIT;
	
}

static void delay(void)
{
  uint16_t i;
  for(i=0;i<50;i++)
  {
    __NOP(); 
  }  
}



void Ad7190Init(void)
{//��ʼ��
	static u8 ct=0;
	
  if(AD7190_Init()==0)
  {
    while(1)
    {
			ct++;
      HAL_Delay(1000);
      if(AD7190_Init())
        break;
			if(ct > 5)
			{
				ct=0;
				printf("\r\n<Ad7190Init>��ȡ���� AD7190 !\n");
				break;
			}
    }
  }
	
		  /* AD7190��·���˲ɼ����� */
  ad7190_unipolar_multichannel_conf();
	Var.AD7190.BaseZero[0]=BIAS_VOLTAGE_IN1;
  Var.AD7190.BaseZero[1]=BIAS_VOLTAGE_IN2;
  Var.AD7190.BaseZero[2]=BIAS_VOLTAGE_IN3;
  Var.AD7190.BaseZero[3]=BIAS_VOLTAGE_IN4;
  Var.AD7190.Flag=1;
	
  printf("\r\n<Ad7190Init>��⵽  AD7190 !\n");
	

}
void AD7190Pro(void)
{//�ɼ���ѹ
	u8 sample[4];
	static double	old_v[2];
	static u16 old_r[2];
  int8_t temp;
	
	if(Var.AD7190.Rcnt==0)
	{//��ʱ�ɼ���ѹ
		//AD7190_WaitRdyGoLow();
		delay();
		if((Var.AD7190.Flag)&&(AD7190_RDY_STATE==0))
		{
			Var.AD7190.Rcnt=2;
			delay();
			HAL_SPI_Receive(&hspi_AD7190,sample,4,0xFFFFFF); 
			delay();
			if((sample[3]&0x80)==0)
			{
				temp=(sample[3]&0x07)-4;
				if(temp>=0)
				{
	#if ZERO_MODE==1
					Var.AD7190.BaseZero[temp]=((sample[0]<<16)|(sample[1]<<8)|sample[2]);
	#else        
					Var.AD7190.RegData[temp]=((sample[0]<<16) | (sample[1]<<8) | sample[2]);
					Var.AD7190.RegData[temp] -= Var.AD7190.BaseZero[temp];        
	#endif
					Var.AD7190.Chanel=temp;
					Var.AD7190.Flag=2;
				}
				else
				{
					printf("\r\n <AD7190Pro> error:0x%X\n",sample[3]);

					Var.AD7190.Flag++;
					if(Var.AD7190.Flag >= 10)
					{
						Var.AD7190.Flag=0;
						printf("\r\n<AD7190Pro> ʧ�ܣ�����\n"); 
					}       
				}
			}
		}
		
		if(Var.AD7190.Flag==2)   
    {      
#if ZERO_MODE==1
      printf("IN%d. 0x%05X\n",Var.AD7190.Chanel,Var.AD7190.BaseZero[number]);
#else        
      Var.AD7190.Voltage[Var.AD7190.Chanel]=Var.AD7190.RegData[Var.AD7190.Chanel]>>4;
      Var.AD7190.Voltage[Var.AD7190.Chanel]=Var.AD7190.Voltage[Var.AD7190.Chanel]*\
																						REFERENCE_VOLTAGE/OPA_RES_R2*OPA_RES_R1/0xFFFFF;
			if(Var.AD7190.Chanel == 0 || Var.AD7190.Chanel == 1)
			{
				//old_v[0] = (Var.AD7190.Voltage[0]+old_v[0]*2)/3;
				old_v[0] = Var.AD7190.Voltage[0];
				Var.Power.RelVo1  = old_v[0]*3;//10v = 30000V
				Var.Power.RelVo1 /= 5;
				Var.Power.RelVo1 *= 5;
				
				
				//old_v[1] = (Var.AD7190.Voltage[1]+old_v[1]*2)/3;
				old_v[1] = Var.AD7190.Voltage[1];
				Var.Power.RelCur = old_v[1]/10;//10v = 100 uA/10
				//Var.Power.RelCur/=5;
				//Var.Power.RelCur*=5;
				
				if((old_r[0] != Var.Power.RelVo1) || (old_r[1] != Var.Power.RelCur))
				{//��ӡ������Ϣ
					
					printf("<AD7190Pro>ͨ�� %d �Ĵ���ֵ 0x%05X����ѹֵ%0.3fV ��ѹ%d V,����%d ��λ0.1uA\r\n",\
																															Var.AD7190.Chanel,\
																															Var.AD7190.RegData[Var.AD7190.Chanel],\
																															Var.AD7190.Voltage[Var.AD7190.Chanel]/1000,\
																															Var.Power.RelVo1,\
																															Var.Power.RelCur);
					//���жϣ��ⲻ����ѹ�������
					if(old_r[0] != Var.Power.RelVo1) old_r[0] = Var.Power.RelVo1;
					if(old_r[1] != Var.Power.RelCur) old_r[1] = Var.Power.RelCur;

				}
			}		
#endif
      Var.AD7190.Flag=1;
    }
	}
	
}

void PowerPro(void)
{//��ѹ��Դ
	static u8 st=0;
	static u8 old_st;
	static u16 real_v;//��ʾ��ѹ����ʾ����

	//�������ط���
	if(Var.Out.Y06)
	{
		st=1;
		real_v = Var.Power.SetVoltage;
		Var.Power.SetCur = 100;
	}
	else 
	{	
		st=0;
		real_v =0;

	}
	
	if(Var.Out.Y06 && \
		0 == Var.Power.DoorFlag && \
		(Var.BSPwork.Stu == KEYRUN || Var.BSPwork.Stu == KEYSET)) 
	{//�ſ���λ,����
		old_st = Var.BSPwork.Stu;
		Var.BSPwork.Stu = DOORHOLD;
		real_v = 0;
	}
	
	
	if(Var.Out.Y06 && \
		Var.Power.DoorFlag && \
		(Var.BSPwork.Stu ==DOORHOLD)) 
	{//�ſ���λ,�ָ�
		Var.BSPwork.Stu = old_st;
		
		real_v = Var.Power.SetVoltage;//��ѹ�ָ��趨
	}
	
		//�ſ���λ,��ֹ�򿪸�ѹ
	if(0 == Var.Power.DoorFlag)
	{
		st=0;
	}
	
	if(1 == st) {OUTPUT7_ON; } else {OUTPUT7_OFF;}

#if 0 == TEST_ENV
	DacOut(real_v);
#endif

}

