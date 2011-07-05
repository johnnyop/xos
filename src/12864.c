/*****************************************************************************
 /12864液晶驱动程序,
 /
 /*****************************************************************************/
#include "12864.h"
#include "xos.h"
#define C_BUSY_TIME 50
/*****************位定义*************************/
/*******************************************/
static void write_data(unsigned char dc, unsigned char dat); //写数据或命令
static unsigned char read_data(unsigned char b); //读数据或命令
static unsigned char chk_busy(); //检查忙状态
void init_12864(); //初始化12864LCD
void cls(); //全屏清屏
void print(unsigned int x, unsigned int y, unsigned char *chi, unsigned char n); //写汉字或字符
//void printsnake(unsigned int x,unit y,unsigned char color);		//蛇身结点显示
/*******************************************/

/********************纳秒**************************       
 void delay(unsigned int n)
 {
 int s;
 for(;n>0;n--)
 for(s=124;s>0;s--);
 }
 /*********************忙状态检测***************************/
static unsigned char chk_busy() {
	unsigned char  busy;
	unsigned char k;
	en = 0;
	for (k = 0; k < 2; k++) {
		rs = 0;
		rw = 1;
		en = 1;	   
		data0 = 0xff;
		busy = (data0 & 0x80);
		en = 0;
	}
	return busy;
}
/********************写数据或指令(dc==1为数据)*********************/
static void write_data(unsigned char dc, unsigned char dat) {
	//while(chk_busy()){;}
	/**/
	unsigned int i = C_BUSY_TIME;
	while ((i-- == 0) && chk_busy())
		;
	/**/
	rs = dc;
	rw = 0;
	en = 1;
	_nop_();
	data0 = dat;
	en = 0;
}
/*******************读数据.状态.1为数据**************************************/
static unsigned char read_data(unsigned char b) {
	unsigned char da;
	unsigned int i = C_BUSY_TIME;
	while ((i-- == 0) && chk_busy())
		;
	rs = b;
	rw = 1;
	en = 1;
	data0 = 0xff;
	da = P0;
	en = 0;
	return da;
}
/********************初始化LCD***************************************/
void init_12864() {
	PSB = 1;
	RST = 1;
	sleep(HZ / 25);
	EA = 0;
	write_data(0, 0x30); //基本指令
	_nop_();
	write_data(0, 0x0c); //开显示
	_nop_();
	write_data(0, 0x01); //清屏
	_nop_();
	write_data(0, 0x06); //光标的移动方向
	EA = 1;
}

//清屏,全部为空
/*********************************************************
 void cls()
 {
 unsigned char data i,j;
 for(i=0;i<32;i++)
 {
 for(j=0;j<8;j++)
 {
 write_data(0,0x34);		//设置扩展功能指令,且为关绘画
 write_data(0,0x80+i);		//写Y地址
 write_data(0,0x80+j);		//写X地址
 write_data(0,0x30);		//设置基本功能指令
 write_data(1,0x00);	 //连续写入两个8位数据.
 write_data(1,0x00);
 }
 }
 for(i=0;i<32;i++)
 {
 for(j=0;j<8;j++)
 {
 write_data(0,0x34);
 write_data(0,0x80+i);
 write_data(0,0x88+j);
 write_data(0,0x30);
 write_data(1,0x00);
 write_data(1,0x00);
 }
 }
 write_data(0,0x36);
 }
 /****************在X,Y处显示一个(16*16)汉字****************/
/*				0<=x<8,0<=y<4	   					*******/
void print(unsigned char x, unsigned char y, unsigned char n, unsigned char *chi) {
	disable_irq_save();
	switch (y) {
	case 0:
		y = 0x80;
		break;
	case 1:
		y = 0x90;
		break;
	case 2:
		y = 0x88;
		break;
	case 3:
		y = 0x98;
		break;
	}
	y += x; //计算要显示的液晶坐标
	write_data(0, 0x30);
	write_data(0, y);
	if (n == 0)
		n = 16;
	y = 0;
	while (chi[y] != '\0' && n--) {
		write_data(1, chi[y]);
		y++;
	}
	enable_irq_restore();
}

void reverse(unsigned char line) {
	disable_irq_save();
	write_data(CMD, 0x34);
	write_data(CMD, 4 + line);
	write_data(CMD, 0x30);
	enable_irq_restore();
}

void clear_12864() {
	disable_irq_save();
	write_data(0, 0x01); //清屏
	enable_irq_restore();
}
