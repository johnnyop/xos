#include "xos_opt.h"   
#include "uart.h"
#include "def.h"
#include "xos.h"
#include "sem.h"
#include "1602.h"
#include <absacc.h> 
#include <reg52.h>	 
/**/
semphore sem = { 0, 0 };
unsigned char sb = 0, sb2 = 0;
void t1() { /**/
	init_uart(0xfd, 1);	 
	schedule();
	SBUF = 1;
	while (1) {
		//do_stuff(0);	   	
		//sem_down(&sem);
		//P0 = ~1;
		sleep(HZ);
		SBUF = ++sb;

	} /**
	 init_1602();
	 while (1) {
	 prints(0,0, "T1");
	 sleep(HZ);
	 }
	 /**/
}

void t2() { /**/
	while (1) {
		//do_stuff(1); 	   
		sleep(2 * HZ + 12);
		//SBUF = ++sb2;
		//P0 = ~(1 << 2);
		//if (P1 > 2) {
		//	sem_up(&sem, 1);
		//}
		//sleep(HZ);
	} /**
	 while (1) {
	 prints(6,0, "T1");
	 sleep(HZ);
	 }
	 /**/
}

void delay(int n) {
	int i;
	for (n; n > 0; n--)
		for (i = 4; i > 0; i--)
			;
}
void t3() { /**/
	//schedule();	  
	while (1) {
		//do_stuff(2);	  
		//sem_down(&sem2);
		//sem_down(&sem);	 
		//do_t3();
		//P2++;
		//if (P2 > 2)
		//	sem_up(&sem, 1);
		// 
		//SBUF = ++P2;
		//delay(1);  
		sleep(HZ / 3 + 25);
		//P0 = ~(1 << 3);
	}
	/**		
	 while (1) {
	 prints(0,1, "T1");
	 sleep(HZ);
	 }
	 /**/
}
void t4() {
	/**
	 while(1) {
	 do_stuff(3);
	 //schedule();
	 P3++;
	 //if (P3 > 4)
	 //	sem_up(&sem2, 1);
	 sleep(HZ);
	 }
	 /**/
	while (1) {
		//prints(6,1, "T1");
		sleep(HZ / 4 + 39);
		//P0 = ~(1 << 4);
	}
}
void main() {  
		ET0 = 0;
		//init_uart(0xfd, 1);	
		//SBUF = ++P2;
	add_task((u16) t1);
	add_task((u16) t2);
	add_task((u16) t3);
	add_task((u16) t4);
	xos_init();
}

void com_uart(void) interrupt 4
{
	/*			接收数据中断						*/
	disable_irq_save();
	if(RI)	 {
		if (SBUF != 0)
			P0 = SBUF;
		RI=0;
	}  else if(TI) {/*          发送数据中断                        */
		TI=0;	
		//delay(31);
		SBUF = ++sb2;
	}
	enable_irq_restore();
}
/**/
