/**
 * xos API
 * */
/**
 * v0.1~
 * 做优化,小修改.
 * */
#ifndef XOS_H
#define XOS_H
#include "type.h"
#include "xos_opt.h"
#include <reg52.h>
#include <intrins.h>


extern u8_t data current; 			/* 当前任务*/
extern u8_t data _irq;
extern u8_t data stack_bottom[NR_TASK];
extern u8_t data id_timeslice[NR_TASK];
extern u8_t data task_status[NR_TASK];
extern u16_t data sleep_time[NR_TASK];
//extern _u8 data sem_count[NR_TASK];
/*
typedef struct {
	u8 stack_bottom;		// 堆栈底
	u8 id_timeslice;		// id 和 时间片
	/**
	 * id_timeslice:
	 * [0:3]-id	, 任务id
	 * [4~7]-timeslice ,		时间片
	 *
	u8 task_status;		// 状态
	u16 sleep_time;			// sleep 调用的睡眠时间. 单位为HZ.
	u8	sem_count;		// 在等待信号量的个数.
}task_desc;/**/

/**
 * 把任务添加到XOS.
 * func: 任务的函数地址
 * */
extern void add_task(unsigned int func);
/**
 * XOS初始化,在main函数里,
 * 添加完任务, 初始化任务的工作后调用.
 * */
extern void xos_init();

/* 下面的函数会进入内核, 任务寄存器入栈*/
/**
 * 任务休眠
 * n: 休眠的时间, 单位为HZ,则系统时间片的时间. 如果为HZ(宏)则1秒,
 * 		如果为0, 则任务不再工作.
 * PS: 任务主函数如果不是死循环工作,那么最后应该调用sleep(0),让任务不再工作.否则系统跑飞
 * */
extern void sleep(unsigned int n);
/**
 * 进行任务调度, 任务让出CPU, 让出一个任务运行. time slice 时间片清0
 * */
extern void schedule();
#define is_kernel() (PCON &= 0x04)
#define set_os()   	(PCON |= 0x04)
#define clr_os()	(PCON &= ~0x04)

#define disable_tick() 	TRN = 0
#define enable_tick()	TRN = 1

#define disable_irq()	do{\
			EA = 0;	\
			TRN	= 0;	\
		}while(0)

#define enable_irq()do{	\
			EA = 1;	\
			TRN = 1;	\
		}while(0)

#define disable_irq_save() do{ \
			EA = 0;	\
		}while(0)

#define enable_irq_restore() do {\
			EA = 1;				 \
		}while(0)

			   /**
			   
			_irq = IE;	\
			TR0 = 0;	\  

			IE = _irq;	\
			TR0 = 0;	\
			   
			   */

#define rand()	TLN
//#define 
#endif /* XOS_H*/
