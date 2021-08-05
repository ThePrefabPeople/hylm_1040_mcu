
#ifndef __AVR_USART_H
#define __AVR_USART_H

#include "main.h"
//about PC--Pl2303
#define PC_CONNECT      0x00   //connect  to M0
#define PC_WRACTM0      0x01   //write txt to M0
#define PC_RDACTM0      0x02   //read txt from M0
#define PC_RDSTUM0      0x03   //read current station from M0
#define PC_SETPRO       0x04   //set begin,stop or hold
#define PC_SETMOV       0x05   //set ver or hor move 
#define PC_BRIDGE       0x06
#define PC_SET_PARA     0x07



//about Mainboard
#define M0_WRACT        0x10   //write action to A0,B1,B2,B3
#define M0_RDACT        0x11   //read current to A0,B1,B2,B3
#define M0_RSTAB        0x12   //reset A0,B1,B2,B3
#define M0_SETMOVE      0x13   //move hor or ver
#define M0_WRPLC        0X01
#define M0_RDPLC        0X11
#define M0_R_SETMOVE    0X93


#define UART1_MAXERR      2
#define XMT_BUFLEN1		  40	  
#define REC_BUFLEN1		  32	  

	  
#define DEV_RCV_SYNC   ' '
#define DEV_RCV_END0   0x0d 
#define DEV_RCV_END1   0x0a   //0x0d,0x0a end byte 串口0

#define DEV_RCV1_SYNC  ' '    //int value 
#define DEV_RCV1_END0   0x0d 
#define DEV_RCV1_END1   0x0a  //0x0d,0x0a end byte  串口1

// (unsigned char)' ' + addr +cmd+datalen+ chs+0x0d +0x0a
#define UART1_MINLEN    7  //最小帧长度


#define ADDR_MAIN      0
#define ADDR_CONA0     1
#define ADDR_CONB0     2
#define ADDR_CONB1     3
#define ADDR_CONB2     4
#define ADDR_PLC       5
#define ADDR_CONMAX    32  //max A,B total device
#define ADDR_AB         0xab
#define CMD_VALID       0x04
#define TOTAL_B_ID      1

// 外设通讯状态标志
//uart0
#define REC_INIT		  0
#define REC_ADDR		  1
#define REC_WRRD		  2
#define REC_CMND		  3
#define REC_LEN		    4
#define REC_DATA	    5
#define REC_CHS		    6
#define REC_END0		  7
#define REC_END1		  8


#define REC1_LEN		  4

//时间常数
#define DEV_RCV_TIMEOUT		15		
#define DEV_RCV_TIMEOUT1	15		
#define U1_RCV_TIMEOUT		2		
#define U1_RCV_TIMEOUT1 	20		
#define RD_MSWAIT         3
#define UART1_TIMEWAIT  	50	//500ms
#define UART1_TIMESET   	20	//200ms
//usart1 
#define UART1_RECDATA     1
#define UART1_RECINIT     0


#endif







