/*
 * uart.c
 */
/**
 * 初始化定时器1和串口通讯
 * 8bit数据,无奇偶校验,1停止位
 * */
#include "uart.h"
#include <reg52.h>
void init_uart(unsigned char th, unsigned char smod)
{
	TMOD = (TMOD & ~0xf0) | 0x20; //定时器1.工作方式2
	TH1 = th;
	TL1 = th; //波特率为9.6K

	SCON = 0x50; //工作方式1.允许接收.SM2=0;
	if (smod)
		PCON |= 0x80;
	else
		PCON &= ~(0x80);

	EA = 1; //全局中断开

	TR1 = 1; //启动定时器1
	ES = 1; //允许串口中断
	PS = 1;
}
/**
void com_uart(void) interrupt 4
{
	uchar str;
	ES=0;
	/*			接收数据中断						*
	if(RI)
	{
		str=SBUF;
		RI=0;
		P0=str;
	}
	/*          发送数据中断                        *
	if(TI)
	{
		TI=0;
	}
	ES=1;
}
 * */
