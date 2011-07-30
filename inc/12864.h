#ifndef _12864_H_
#define _12864_H_

#include <intrins.h>
#include <reg52.h>
#include "type.h"
//#define _nop_() ;
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
