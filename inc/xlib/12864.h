#ifndef _12864_H_
#define _12864_H_

#include <intrins.h>
#include "type.h"
//#define _nop_() ;
#define RW_WR	0
#define RW_RD	1
#define RS_CMD	0
#define RS_DATA	1

#define CN_LINE0	0x80
#define CN_LINE1	0x90
#define CN_LINE2	0x88
#define CN_LINE3	0x98

#define UPPER_HALF	0x80
#define LOWER_HALF	0x88


#define CMD_CGRAM(n)	(0x80 + (n))
#define CMD_BASE		(0x30)
#define CMD_BASE_G		(0x32)
#define CMD_EXT			(0x34)
#define CMD_EXT_G		(0x36)

#define SHOW_PIC	0
#define DISPLAY_STR	0
/************************函数声明******************************/
extern void init_12864(void); /*初始化LCD*/
#if DISPLAY_STR
void printstr(register u8_t  x, register u8_t y, unsigned char n, unsigned char *chi); /*在X,Y处显示一个(16*16)汉字*/
#endif /* DISPLAY_STR */
extern void clear_lcd(void);  				/*清屏,全部为空*/
extern void clear_12864(void);
extern void reverse(unsigned char line);
void draw_point(register u8_t  x, register u8_t  y, register u8_t  b);
//void draw_hline(register u8_t x, register u8_t y, register u8_t l);
//void draw_vline(register u8_t x, register u8_t y, register u8_t l);
void draw_line(u8_t data x1, u8_t data y1,u8_t data x2,u8_t data y2, u8_t b);
#endif
