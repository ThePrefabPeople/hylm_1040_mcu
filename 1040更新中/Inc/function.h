#ifndef __FUNCTION_H
#define __FUNCTION_H
#include "stm32f4xx_hal.h"

void WorkPro(void);
void Uart1Pro(void);

void ParmInit(void);	//运行参数初始化
void FlashInfo(void);
void TestRWFlsh(u32 id);//测试读写FLASH
void FlashWritePro(void);
void SetStepParam(u8 *p);
void OutIo(u8 *p);
void Uart5Pro(void);
u8 MsgtoPCUart0(ts_MSG_COM5* msg_rs,u8 *wrbuf);
void RunProId(te_PRON_ID id);
te_PRON_ID GetProOrder(u8 *d);
void AD7190Pro(void);
void Ad7190Init(void);
void RecorderPro(void);
void PowerPro(void);

#endif

