#ifndef __MAIN_H
#define __MAIN_H
/*
���������������⣺
1��ȫ�ֱ����ڷ��������������ֱ������ʱ��ʼֵ������0
*/
#include "stdint.h"
//���Ի���ѡ��
#define TEST_ENV	1

#if TEST_ENV
#define VERSION		"���Ի���V0.0"
#else
#define VERSION		"��ʽ����V1.0"
#endif

#define AD7190_DEBUG_INFO	1

#define 	POWERON_DELAY				5			//������ʱ�����������
#define 	XMT_BUFLEN		  80	  
#define 	REC_BUFLEN		  80
#define 	PROGRAM_NUM	10	//�洢��������

#define SYSINIT       0xFF 
#define SYSRST        0 //
#define DOORHOLD			1	//
#define SYSIDLE       2	//
#define PCSYSRUN      3  //
#define PCSYSHOLD     4  //
#define KEYRUN       	5  //��
#define KSYSHOLD      6  //
#define KSYSMOVE      7  //
#define KEYSTOP      	8  //
#define KEYSET      	9  //
#define SYSEROR				10	//����

#define M0_WRACT        0x10   //write action to A0,B1,B2,B3
#define M0_RDACT        0x11   //read current to A0,B1,B2,B3
#define M0_RSTAB        0x12   //reset A0,B1,B2,B3
#define M0_SETMOVE      0x13   //move hor or ver
#define M0_WASTE_ZERO   0x14   //SERCH ZERO
#define M0_INLET_ZERO   0x15   //SERCH ZERO
#define M0_INLET_DEBUG  0x16
#define M0_WASTE_DEBUG  0x17
#define M0_VOTAGE_DEBUG 0x18
#define M0_VOTAGE_SET   0x19
#define M0_BO_DEBUG     0x21
#define M0_FAMEN_DEBUG  0x22
#define INLET_MOVE_TO   0X23
#define WASTE_MOVE_TO   0X24

#define CMD_ANSWER(cmd)       (u8)(cmd|0x80)  //answer command
#define CMD_GETANS(cmd)       (u8)(cmd&(~0x80)) 

#define RECORD_MAXLEN      16  //
#define RECORD_USELEN      16  // 3(step(1 byte) + time(2 byte) )+64(16*4)

#define PCWR_OT  20

#define PCWR_OK  1
#define PCWR_ERR 0

#define CNT_ENABLE    1
#define CNT_DISABLE   0


#define PRON_INIT     0
#define PRON_VALID    1
#define PRON_UNVALID  2

typedef enum
{//���еĳ�����
	IO_BIT2=2,IO_BIT5=5, IO_BIT4=4,IO_BIT7=7,
	IO_BIT1=1,IO_BIT0=0, IO_BIT3=3,IO_BIT6=6,
} te_IO_BIT;

typedef enum
{//���еĳ�����
	SYSPRON_INIT=0,SYSPRON_RUNA=1, SYSPRON_RUNB=2,SYSPRON_RUNC=3,
	SYSPRON_RUND=4,SYSPRON_RUNE=5, SYSPRON_RUNF=6,SYSPRON_RUNG=7,
	SYSPRON_RUNH=8,SYSPRON_RUNI=9, SYSPRON_RUNJ=10,SYSPRON_RUNK,
} te_PRON_ID;

typedef struct
{//ϵͳ
	u8 	Led1Cnt;
	u8 	ResetCnt;
	u8 	KeyCnt;
	u8 	DebugCnt;
	u32	Clock;
	u32 SecCnt;
}ts_SYS;

typedef struct
{//���
	u8	InitDelay;		//������ʱ
	u8	Dir;					//�������
	u8	ZeroPoint;					//����־
	u8	Limit;							//�����ƶ���־
	volatile u8	CurPoint;						//��ǰλ��
	volatile u32	Pulse;		//��ǰ����
	volatile u32	SetPulse;	//�趨����
	u8	ZeroPoint2;	//����־
	volatile u8	SetPoint;	//Ŀ��λ��
	u16	CalibrationPulse;		//���λ����������
	u8	SerchZero;					//�����־
	u8	StopDelay;					//�ߵ�λ�ú�ȴ�ʱ��
	volatile	u16	Pu;									//����ʱ�߹���������
	u8	Stu;								//���״̬
	u8	Busy;								//æ��־
}ts_MOTOR;


typedef struct
{//���
	//out4
	u8	Y00;//������
	u8	Y01;//С����
	u8	Y02;//������
	u8	Y03;//������
	
	//motor3
	u8	Y04;//������
	u8	Y05;//С����
	u8	Y06;//���ظ�ѹ
	
	//motor4
	u8	Y07;//��ʱ������//�л�������ѹ
	u8	Y10;//������
	u8	Y11;//ֹͣ��
	
	//����������
	u8	Y12;//��λ��
	u8	Y13;//����վ��������1
	u8	Y14;//����2
	u8	Y15;
	
	//motor1
	u8	Y16;	
	u8	Y17;
	//motor2
	u8	Y20;
	u8	Y21;
}ts_IO_OUT;

typedef struct
{
	u8	X00;	//������ԭ���ź�
	u8	X01;	//��Һ��ԭ���ź�
	u8	X02;	//��������λ��
	u8	X03;	//С������λ��
	u8	X04;	//������ť
	u8	X05;	//ֹͣ��ť
	u8	X06;	//��λ��ť
	u8	X07;	//�ſ�
	u8	X10;	//KEY1
	u8	X11;	//KEY2
	u8	X12;	//KEY3
	u8	X13;	//KEY4
	u8	X14;	//KEY5
	u8	X15;	//Ԥ��
	u8	X16;	//Ԥ��
	u8	X17;	//Ԥ��
	u8	X20;	//Ԥ��
}ts_IO_IN;


typedef struct
{
	u8	SecCnt;	
	u8 	Buf[100];
	u8	Num;
	u8	Busy;
	u32	SetPulse;
	u8 	PCwr_start;    //��ʼд
	u8	PCwr_done;    //д���
	u8 	Uart0_to;			//��һ���ȴ���ʱ������
	u8 	PC_connect;		//����״̬
	u8 	PC_connto;			//����״̬��ʱ������
}ts_COM;


typedef struct
{
	u8	Key;						//���Է��Ͱ���
	u8	WaitPro;				//Ҫ���еĳ���
	u8	ValidPro;				//��Ч����
}ts_PC;


typedef  struct
{//���̿���		
	u8 		Stu;       //����״̬
	u8 		Curstep;   //��ǰ����ID
	u16  	TotalSec;  //�ܹ�����ʱ��
	u16  	Cursec;    //��ǰ����ʱ��
	u8 		Start;   		//���뿪��
	u8 		New;       	//��һ����־
	u8 		NextStep;		//��һ����־
	u8		SecCnt;	
	u8		CycTime;		//����ѭ������
volatile	u8		PointId;		//���Ŀ��λ��
	u8		RunPro;			//Ҫ���еĳ���
	u8		Delay;
	u16		StopCnt;		//ֹͣ�����ʱ��
	
} ts_MBSPWORK;

typedef struct 
{
	u16	SetVoltage;
	u16	Voltage;
	u16	Current;
	u16 RelVo1;
	u16 RelVo2;
	u16	RelCur;
	u16	SetCur;
	u8	DoorFlag;
	
}ts_POWER;

typedef struct 
{//
	u8	RWBuf[4096];	//FLASH ��д������
	u8	ReadBuf[4096];//FLASH ��д������
	u8	Sector;				//������ַ
	u8	UpFlag;				//���³����־
	u8	CaFlag;				//д�������־
	u16	WriteNum;
}ts_FLASH;

typedef  struct
{//�������	
	u8 PronValid;    //
	u8 Pron;         //program A,B,C,D,E,...J
	u8 TotalStep;         //total step number
	u8 StartId;        //
	u8 EndId;        //
	u8 CycTime;      // 
	u8 BNum;         //total B number
	u8 VerFb;        //vertical move feed back
}ts_SYSRUN;

typedef struct
{//
	u8 addr;
	u8 cmd;
	u8 datalen;
	u8 dataBuf[70];
	
}ts_MSG_COM5;//data

typedef struct
{
	u8  	RcvBuf[REC_BUFLEN];   // ������ջ�����
	u8  	devXmtBuf[XMT_BUFLEN];   // ���跢�ͻ�����
	u8  	bDevRcvBuf;
	u8  	devRcvState;    // ͨѶ����״̬
	u8  	devRcvCtr;      // ͨѶ�ֽ�ָ��
	u8  	devRcvLength;   // �����ֽڳ���
	u8  	devRcvChs;      // ͨѶУ���ֽ�
	u16  	devRcvTimer;     // ͨѶ��ʱ������1
	u8  	devXmtCtr;      // �����ֽ�ָ��
	u8  	devXmtLength;   // �����ֽڳ���
	u8  	bDevXmtBuf;     // 1=���跢�ͻ�������һ֡����
}ts_COM_BASE;

typedef struct
{//���籣�����
	u16	Pulse;			//������ԭ����������
	u16	ProRunId;		//�ػ�ǰ���г�������
	
}ts_EPARAM;

typedef struct
{
	u8	Rcnt;	//��ȡ��ѹ���
	u32 RegData[4]; // AD7190ԭʼת�����
	u32 BaseZero[4];   // ����ѹ��ADת�����
	double Voltage[4]; // ��ѹֵ����λ��mV��
	u8 	Flag;         // �����ɼ���־
	u8	Chanel;          // ��ǰ�����ͨ��
}ts_AD7190;

typedef struct
{//����վ
	u8	DeCnt;//��ʱ
	u8	IOF;	//����״̬
}ts_RECORDER;
typedef struct
{
	u8	UpCnt;//������Ϣ
	u8 	UpCnt2;
}ts_MONITOR;

typedef struct
{
	ts_SYS			Sys;
	ts_IO_OUT		Out;
	ts_IO_IN		In;
	ts_MOTOR		Motor1;
	ts_COM			Com5;
	ts_MSG_COM5	AnPc;
	ts_PC				Pc;
	ts_MBSPWORK	BSPwork;
	ts_POWER		Power;
	ts_FLASH		Flash;
	ts_SYSRUN		EePro[PROGRAM_NUM];
	ts_SYSRUN		RunPro;
	ts_SYSRUN		PCPro;  
	te_PRON_ID	ProId;
	u8					run_pro_bit;
	u8 					Parm[4096];	//����д�뻺��
	ts_COM_BASE	Com5Base;
	u8					g_iddevice;     //�����ַ
	ts_AD7190		AD7190;
	ts_RECORDER	Recorder;
	ts_MONITOR	Monitor;
	ts_EPARAM		EParam;
	
}ts_VAR;
extern ts_VAR Var;

#endif /* __MAIN_H */
