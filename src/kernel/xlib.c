/**
 * \file xlib.c
 * \brief 提供常用函数, 未完善
 */
#include "xlib.h"
/*12MHz 
12T
DJNZ 	24
LCALL	24
MOV	x,x	12~24
1T
DJNZ	4
LCALL	6
MOV	x,x	1~4
**/			   
#include <intrins.h>

void delayus(register u8_t u)
{		   
		   
#if (CPU_T == CPU_1T) 
 	#if	(22 < VFOSC)
 	u <<= 1;
	#endif 
	for (;u;--u) {
		_nop_();
		_nop_();
	}		 
#else //CPU_12T	 

 #if (VFOSC < 22)
	u >>= 1;
 #endif	//FOSC < 24
	if (u < 6) //前前后后的指令已经有12us
		return;
	u -= 6;
	while(--u);	   
#endif // CPU_1T
}

void delayms(register u8_t u)
{
	//1T 18, 12T 108/ 12 = 9
#if	(22 > VFOSC && CPU_T == CPU_12T)
	while(--u) delayus(91);
#elif (22 < VFOSC && CPU_T == CPU_12T)	
	while(--u) { delayus(95); _nop_()};	

#elif	(22 > VFOSC && CPU_T == CPU_1T)
	for (;u;--u) {
		delayus(99);
	}
#elif (22 < VFOSC && CPU_T == CPU_1T)
	for (;u;--u) {
		delayus(99);
		_nop_();_nop_();_nop_();
	}	
#endif

}
