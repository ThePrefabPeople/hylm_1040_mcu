#ifndef __MAIN_H
#define __MAIN_H
/*
调试中遇到的问题：
1，全局变量在仿真器下载完程序直接运行时初始值不等于0
*/
#include "stdint.h"
//测试环境选择
#define TEST_ENV	1

#if TEST_ENV
#define VERSION		"测试环境V0.0"
#else
#define VERSION		"正式环境V1.0"
#endif

#define AD7190_DEBUG_INFO	1

#define 	POWERON_DELAY				5			//开机延时启动电机找零
#define 	XMT_BUFLEN		  80	  
#define 	REC_BUFLEN		  80
#define 	PROGRAM_NUM	10	//存储程序总数

#define SYSINIT       0xFF 
#define SYSRST        0 //
#define DOORHOLD			1	//
#define SYSIDLE       2	//
#define PCSYSRUN      3  //
#define PCSYSHOLD     4  //
#define KEYRUN       	5  //
#define KSYSHOLD      6  //
#define KSYSMOVE      7  //
#define KEYSTOP      	8  //
#define KEYSET      	9  //
#define SYSEROR				10	//故障

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
{//运行的程序编号
	IO_BIT2=2,IO_BIT5=5, IO_BIT4=4,IO_BIT7=7,
	IO_BIT1=1,IO_BIT0=0, IO_BIT3=3,IO_BIT6=6,
} te_IO_BIT;

typedef enum
{//运行的程序编号
	SYSPRON_INIT=0,SYSPRON_RUNA=1, SYSPRON_RUNB=2,SYSPRON_RUNC=3,
	SYSPRON_RUND=4,SYSPRON_RUNE=5, SYSPRON_RUNF=6,SYSPRON_RUNG=7,
	SYSPRON_RUNH=8,SYSPRON_RUNI=9, SYSPRON_RUNJ=10,SYSPRON_RUNK,
} te_PRON_ID;

typedef struct
{//系统
	u8 	Led1Cnt;
	u8 	ResetCnt;
	u8 	KeyCnt;
	u8 	DebugCnt;
	u32	Clock;
	u32 SecCnt;
}ts_SYS;

typedef struct
{//电机
	u8	InitDelay;		//开机延时
	u8	Dir;					//电机方向
	u8	ZeroPoint;					//零点标志
	u8	Limit;							//限制移动标志
	volatile u8	CurPoint;						//当前位置
	volatile u32	Pulse;		//当前脉冲
	volatile u32	SetPulse;	//设定脉冲
	u8	ZeroPoint2;	//零点标志
	volatile u8	SetPoint;	//目标位置
	u16	CalibrationPulse;		//零点位置修正脉冲
	u8	SerchZero;					//找零标志
	u8	StopDelay;					//走到位置后等待时间
	volatile	u16	Pu;									//加速时走过的脉冲数
	u8	Stu;								//电机状态
	u8	Busy;								//忙标志
}ts_MOTOR;


typedef struct
{//输出
	//out4
	u8	Y00;//大气阀
	u8	Y01;//小气阀
	u8	Y02;//进样阀
	u8	Y03;//放气阀
	
	//motor3
	u8	Y04;//大气缸
	u8	Y05;//小气缸
	u8	Y06;//开关高压
	
	//motor4
	u8	Y07;//临时放气阀//切换正负高压
	u8	Y10;//启动灯
	u8	Y11;//停止灯
	
	//编码器插座
	u8	Y12;//复位灯
	u8	Y13;//工作站开，备用1
	u8	Y14;//备用2
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
	u8	X00;	//进样盘原点信号
	u8	X01;	//废液盘原点信号
	u8	X02;	//大盘气缸位置
	u8	X03;	//小盘气缸位置
	u8	X04;	//启动按钮
	u8	X05;	//停止按钮
	u8	X06;	//复位按钮
	u8	X07;	//门控
	u8	X10;	//KEY1
	u8	X11;	//KEY2
	u8	X12;	//KEY3
	u8	X13;	//KEY4
	u8	X14;	//KEY5
	u8	X15;	//预留
	u8	X16;	//预留
	u8	X17;	//预留
	u8	X20;	//预留
}ts_IO_IN;


typedef struct
{
	u8	SecCnt;	
	u8 	Buf[100];
	u8	Num;
	u8	Busy;
	u32	SetPulse;
	u8 	PCwr_start;    //开始写
	u8	PCwr_done;    //写完成
	u8 	Uart0_to;			//下一步等待超时计数器
	u8 	PC_connect;		//连接状态
	u8 	PC_connto;			//连接状态超时计数器
}ts_COM;


typedef struct
{
	u8	Key;						//电脑发送按键
	u8	WaitPro;				//要运行的程序
	u8	ValidPro;				//有效程序
}ts_PC;


typedef  struct
{//流程控制		
	u8 		Stu;       //工作状态
	u8 		Curstep;   //当前步骤ID
	u16  	TotalSec;  //总共运行时间
	u16  	Cursec;    //当前运行时间
	u8 		Start;   		//读秒开关
	u8 		New;       	//下一步标志
	u8 		NextStep;		//下一步标志
	u8		SecCnt;	
	u8		CycTime;		//程序循环次数
volatile	u8		PointId;		//电机目标位置
	u8		RunPro;			//要运行的程序
	u8		Delay;
	u16		StopCnt;		//停止程序计时器
	
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
	u8	RWBuf[4096];	//FLASH 读写缓冲区
	u8	ReadBuf[4096];//FLASH 读写缓冲区
	u8	Sector;				//扇区地址
	u8	UpFlag;				//更新程序标志
	u8	CaFlag;				//写入参数标志
	u16	WriteNum;
}ts_FLASH;

typedef  struct
{//程序参数	
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
	u8  	RcvBuf[REC_BUFLEN];   // 外设接收缓冲区
	u8  	devXmtBuf[XMT_BUFLEN];   // 外设发送缓冲区
	u8  	bDevRcvBuf;
	u8  	devRcvState;    // 通讯命令状态
	u8  	devRcvCtr;      // 通讯字节指针
	u8  	devRcvLength;   // 接收字节长度
	u8  	devRcvChs;      // 通讯校验字节
	u16  	devRcvTimer;     // 通讯超时计数器1
	u8  	devXmtCtr;      // 发送字节指针
	u8  	devXmtLength;   // 发送字节长度
	u8  	bDevXmtBuf;     // 1=外设发送缓冲区有一帧数据
}ts_COM_BASE;

typedef struct
{//掉电保存参数
	u16	Pulse;			//进样盘原点修正脉冲
	u16	ProRunId;		//关机前运行程序序列
	
}ts_EPARAM;

typedef struct
{
	u8	Rcnt;	//读取电压间隔
	u32 RegData[4]; // AD7190原始转换结果
	u32 BaseZero[4];   // 零点电压的AD转换结果
	double Voltage[4]; // 电压值（单位：mV）
	u8 	Flag;         // 启动采集标志
	u8	Chanel;          // 当前处理的通道
}ts_AD7190;

typedef struct
{//工作站
	u8	DeCnt;//延时
	u8	IOF;	//开关状态
}ts_RECORDER;
typedef struct
{
	u8	UpCnt;//更新信息
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
	u8 					Parm[4096];	//串口写入缓冲
	ts_COM_BASE	Com5Base;
	u8					g_iddevice;     //主板地址
	ts_AD7190		AD7190;
	ts_RECORDER	Recorder;
	ts_MONITOR	Monitor;
	ts_EPARAM		EParam;
	
}ts_VAR;
extern ts_VAR Var;

#endif /* __MAIN_H */
