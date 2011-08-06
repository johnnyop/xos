/*
 *  LCD1602低层驱动
 *
 * 0x01; 清显示,光标复位到00H位置.
 * 0X02;或0X03光标复位到00H位置.
 * 0000 1 D C B;
 * 			(然后化为16进制,如0x0f显示闪烁的光标)D=1开显示,D=0关显示,C=1显示光标,C=0不显示光标.B=1光标闪烁,B=0光标不闪烁.
 * 0000 0 1 N S;
 * 			(然后化为16进制)N=1读或写一个字符时地址指针和光标都加一.
 * 						N=0时,读或写一个字符后地址指针和光标都减一.
 * 						S=1,当写一个字符后整屏左移(N=1)或右移(N=0),以得到光标不动整个屏幕在动的效果.
 * 						S=0,写一个字符后整屏不动.
 * 0x80+(0x00~0x27第一行或0x40~0x67第二行),设置数据地址指针,即每行第一字符在屏上的位置,
 * 						例如0x90即第一行的第一个字符在第一行的第十个位置开始写.
 * 0x18;整屏左移一位.
 * 0x1c;整屏右移一位
 * 0x10;光标左移一位
 * 0x14;光标右移一位.
 */
#ifndef _1602_H
#define _1602_H
/**/

#define LINE_0		0x80
#define LINE_1		0xc0

/** 指令*/
#define CLEAR_SHOW 	0x01	//清显示,光标复位到00H位置.
#define ENTER		0x02	//显示回车, 数据指针清零

#define CMD_1		0x08
#define SHOW_BIT 	0x04	// 显示位
#define CURSOR_BIT	0x02	// 光标显示位
#define FLASH_BIT	0x01

#define CMD_2 		0x04
#define	POINT_BIT 	0x02	// 指针加
#define MOVE_DIR	0x01

#define CRYSTAL_1602 24
extern void init_1602();
//-------------------------------------------------------指定位置显示一串字符
extern void prints(unsigned char x,unsigned char y,unsigned char *dat);
/***************************指定位置显示一个字符************************************/
extern void printc(unsigned char x,unsigned char y,unsigned char dat);
#endif //1602_H
