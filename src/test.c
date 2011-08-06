/**
 * \file test.c
 * \brief xos测试代码
 */
#include "xos_opt.h"   
#include "uart.h"
#include "type.h"
#include "xos.h"
#include "sem.h"
#include "xlib.h"
#include "12864.h"
#include <absacc.h> 
#include "stcad.h"	 
/**/				//20.7
#ifdef SEMPHORE
semphore sem = { 0, 0 };
#endif //SEMPHORE

static void t1() { /**/
	while (1) {
		sleep(HZ); 
	} 
	sleep(0);
}
#define send_poll(d) do { \
	SBUF = d;	\
	while (TI);	\
	TI = 0;	\
}while (0)
u8_t adres0, adres1, adres2;
u8_t uc0, uc1;
static 
void t2() { /**/ 
	while (1) { 
		sleep(HZ);
	} 
	sleep(0);
}
static
void delay(int n) {
	int i;
	for (n; n > 0; n--)
		for (i = 4; i > 0; i--)
			;
}
u16_t tcnt = 0;
static 
void t3() { /**/
	//schedule();	  
	while (1) {
		sleep(HZ);
	}			 
	sleep(0);
}
static
void t4() {
	while (1) {
		sleep(10);
	}		
	sleep(0);
}
void main() {  
	ETN = 0;
	add_task((u16_t) t1);
	add_task((u16_t) t2);
	//add_task((u16_t) t3);
	//add_task((u16_t) t4);
	xos_init();
}

void com_uart(void) interrupt 4
{
	/*			接收数据中断						*/
	disable_irq();
	if(RI)	 {
		RI=0;
	}  else if(TI) {/*          发送数据中断                        */
		TI=0;	
	}
	enable_irq();
}
/**/
