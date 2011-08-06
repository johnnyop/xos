#include <intrins.h>
#include "xos.h"
#include "1602.h"

#define BUSY_TIME 100 
#define OTHERS_BOARD
/**/
#ifndef OTHERS_BOARD
	#define L_data P0
	sbit rs_1602=P3^0;
	sbit rw_1602=P3^1;
	sbit en_1602=P3^2;
#else
	#define L_data P2
	sbit rs_1602=P1^2;
	sbit rw_1602=P1^1;
	sbit en_1602=P1^0;
#endif
//----------------------------------------------定义引脚
static void write_1602(unsigned char da, unsigned char cmd);
static unsigned char check_busy(void) ;

#define writedata(da) 	write_1602(da, 1)
#define writecmd(da)	write_1602(da, 0)
static unsigned char check_busy(void) //检测LCD状态，看它是不是还在忙呢
{
	unsigned char busy;
	en_1602 = 0;
	rs_1602 = 0;
	rw_1602 = 1;
	_nop_();
	en_1602 = 1;
	_nop_();
	busy = L_data & 0x80;
	_nop_();
	en_1602 = 0;
	return busy;
}
//----------------------------------------------------写入数据
static void write_1602(unsigned char da, unsigned char cmd) {
	unsigned int i = BUSY_TIME;
	while (check_busy() || (i-- == 0))
		; //等待LCD忙完,,..
	en_1602 = 0;
	rs_1602 = cmd;
	rw_1602 = 0;
	L_data = da;
	_nop_();
	en_1602 = 1;
	_nop_();
#if CRYSTAL_1602 > 12
	_nop_();
#endif
	en_1602 = 0;
}
//----------------------------------------------------指定位置显示一个字符
/***************************指定位置显示一个字符************************************/
void printc(unsigned char x, unsigned char y, unsigned char dat) {
	disable_irq_save();
	if (y > 2 || x > 39)
		return; //防止位置超了
	if (y == 0) //
		x = 0x80 + x; //计算坐标
	else
		//
		x = 0xc0 + x;
	writecmd(x);
	writedata(dat);
	enable_irq_restore();
}
/****************************************************************************/
//-------------------------------------------------------指定位置显示一串字符
void prints(unsigned char x, unsigned char y, unsigned char *dat) {
	disable_irq_save();
	if (y > 2 || x > 39)
		return; //防止位置超了
	if (y != 0)
		x += 0x40;
	x += 0x80; //计算坐标
	y = 0;
	while (dat[y] != '\0') {
		writecmd(x);
		writedata(dat[y]);
		x++;
		y++;
	}
	EA = 1;
	enable_irq_restore();
}
//---------------------------------------------------初始化LCD
void init_1602() {
	sleep(HZ / 100); //
	EA = 0;
	writecmd(0x38); //
	EA = 1;
	sleep(HZ / 100); //三次模式显示设置
	EA = 0;
	writecmd(0x38); //
	EA = 1;
	sleep(HZ / 100); //
	disable_irq_save();
	writecmd(0x38); //
	writecmd(0x38);
	writecmd(0x08); //关闭显示
	writecmd(0x06); //显示光标移动设置
	writecmd(0x0c); //开屏开及光标加设置
	enable_irq_restore();
}


