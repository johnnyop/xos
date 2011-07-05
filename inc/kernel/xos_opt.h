/**
 * file: xos_opt.h
 * xos的系统配置, 移植时的设置.
 * */
#ifndef XOS_OPT_H
#define XOS_OPT_H
/**
 * kernel.c和xos.c 要勾选Generate Assembler SRC File 和 Assemble SRC File
 * 右键文件->option for file '*.c'
 * 			->Generate Assembler SRC File
 * 			->Assemble SRC File
 * */

#define INCLUDE_MCU <reg52.h>

/* 该系统运行的任务数,静态值. 最大为8, 不过因为系统性能,资源的问题,最好不超来6*/
#define NR_TASK 4	

/**
 * 任务堆栈
 * STACK_SZ = ((ALL_STACK_SZ) / NR_TASK)
 * 任务运行的栈的大小, 单片机留给任务的总栈大小 / 任务数
 * !!!要求
 * 	任务栈的大小最少要:　任务运行时可能嵌套层数*2 + 13(寄存器) +　2(内核)
 *  模拟栈大小(reentrant关键词): 可重入函数的使用大小.
 *  内核栈: 6. 则编译后data的使用大小, 到模拟栈开始地址或任务栈开始地址(没用模拟栈)之间
 * */															 
#define STACK_START	(0x80)		/* 总栈起始地址. 默认为idata开始地址*/
#define STACK_END	(0x100)		/* END + 1*/
/* 任务PDATA的栈大小,自己手算结果,可以加快速度和减少代码体积*/
#define STACK_SZ	((STACK_END - STACK_START) / NR_TASK)	

/**
 * 如何定义了下面这个宏,可以到xos.c文件的customize_stack函数里设置自定义大小的任务栈.
 * 只需在customize_stack函数添加任务的栈起始地址则可.栈地址必须在0x00~0xff范围里.
 * */
//#define CUSTOMIZE_STACK
#define XOS_REENTRANT
#define C_BP ?C_IBP
#define C_BP_SIZE	10
#define C_BP_END	0x80
#define C_BP_START	(C_BP_END - C_BP_SIZE * NR_TASK)
/**
 * 类似CUSTOMIZE_C_BP
 * */
//#define CUSTOMIZE_C_BP




/**
 * 时间片,系统定时,时间点滴, 默认使用定时器0
 * */
#define HZ 100		/* xos系统定时中断频率, 1秒使用时间片的次数*/
#define TICK_TIME 10		/* 一个时间片的时间(单位ms, 则系统定时器定时时长)*/
#define TIME_SLICE 5		/* 任务时间片, 不能大于15*/
// 10ms , 24MHz, 1
#define FOSC 11.0592	/* 晶振频率*/
#define TH 0xDC		/* 系统定时器,计数器装载值*/
#define TL 0x20 
/* 系统定时器用的计数器寄存器*/
#define THN TH0
#define TLN TL0
/**
 * 设置任务调度时,是否首先运行休眠时间已到的任务.(sleep(n),n已到).
 * 如果SLEEPED_FIRST 不等于0, 则设置
 *
 * 设置后,当一个任务的一个时间片用完后,如果有休眠时间已到,但未运行的任务,
 * 则即使该任务还有时间片,还会让出CPU给休眠时间已到的的任务.
 * 该设置是为了休眠时间精度更高.
 * */
#define SLEEPED_FIRST 5


/**
 * 下面是与内核有关的定义,,如果无修改内核,则无需更改
 * */

#define MAX_TASK 8		/* XOS最大任务数*/
#define SAVE_REG 13		/* 任务调度保存的寄存器数*/
#define TASK_SZ 6		//sizeof(task_desc),任务描述符结构体大小


#define NREG  8
#define USEDREG 1

/* 任务描述符各元素的内存偏移值*/
#define STACT_BOTTOM 0
#define ID_TIMESLICE 1
#define TASK_STATUS	2
#define SLEEP_TIME 3
#define SEM_COUNT 5

/* 任务状态标志位*/
#define TASK_SEM_WAIT 0x10
#define TASK_SLEEP 0x20
#define TASK_OVER 0x80


#endif /* XOS_OPT_H**/
