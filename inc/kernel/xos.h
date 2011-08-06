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
#include MCU_H
#include <intrins.h>


/* 任务描述符各元素的内存偏移值,, 已不使用，不用结构体实现了*/
#define STACT_BOTTOM 0
#define ID_TIMESLICE 1
#define TASK_STATUS	2
#define SLEEP_TIME 3
#define SEM_COUNT 5
extern u8_t data current; 			/* 当前任务*/
//extern u8_t data _irq;
extern u8_t data stack_bottom[NR_TASK];
extern u8_t data id_timeslice[NR_TASK];
extern u8_t data task_status[NR_TASK];
extern u16_t data sleep_time[NR_TASK];
//extern _u8 data sem_count[NR_TASK];

/* 任务状态标志位*/
/** 任务等待信号量 */
#define TASK_SEM_WAIT (1 << 4)
/** 任务睡眠中 */
#define TASK_SLEEP (1 << 5)
/** 任务结束，不再运行 */
#define TASK_OVER (1 << 7)
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
extern void sleep(unsigned int n);
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

#define rand()	TLN
//#define 
#endif /* XOS_H*/
